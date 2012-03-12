//TRINITY NYA DEV
#include "ScriptPCH.h"
#include "Config.h"
#include <math.h>

#define DI_SQL_GET "SELECT `InstanceId`, `InstanceLevel` FROM `world_instance`"
#define DI_SQL_SAVE "REPLACE INTO `world_instance` (`InstanceId`, `InstanceLevel`) VALUES ('%u', '%u')"
#define DI_SQL_DELETE "DELETE FROM `world_instance` WHERE `InstanceId` = '%u'"
#define DI_SQL_CLEANUP "DELETE `w` FROM `world_instance` `w` LEFT JOIN `instance` `i` ON `w`.`InstanceId` = `i`.`id` WHERE `i`.`id` IS NULL"

bool DynamicInstanceEnable = false;

struct DITemplate
{
    DITemplate() : level(0) {}
    uint8 level;

    bool isValid() const { return level != 0; }
};

typedef std::map<uint32, DITemplate> DIMap;
DIMap DIData;

void DILoadDataFromDB()
{
    DIData.clear();

    CharacterDatabase.PExecute(DI_SQL_CLEANUP);

    sLog->outString();
    sLog->outString("Loading DynamicInstance...");
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.PQuery(DI_SQL_GET);

    uint16 count = 0;

    if (!result)
    {
        sLog->outString(">> `world_instance` is empty");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        DIData[fields[0].GetUInt32()].level = fields[1].GetUInt8();
    }
    while (result->NextRow());

    sLog->outString(">> Loaded %u instances for DynamicInstance in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

bool DICreateOrExisted(Map* map)
{
    if (!DynamicInstanceEnable || !map->IsDungeon())
        return false;

    if (DIData[map->GetInstanceId()].isValid())
        return true;

    Map::PlayerList const &players = map->GetPlayers();

    if (!players.isEmpty())
        if (Player* player = players.begin()->getSource())
        {
            DIData[map->GetInstanceId()].level = player->getLevel();
            CharacterDatabase.PExecute(DI_SQL_SAVE, map->GetInstanceId(), DIData[map->GetInstanceId()].level);
            return true;
        }
    return false;
}

void DIRemoveData(uint32 instanceid)
{
    if (!DynamicInstanceEnable)
        return;

    if (DIData.empty())
        return;

    if (!DIData[instanceid].isValid())
        return;

    CharacterDatabase.PExecute(DI_SQL_DELETE, instanceid);

    DIMap::iterator itr;

    for (itr = DIData.begin(); itr != DIData.end();)
    {
        if (itr->first == instanceid)
        {
            DIData.erase(itr);
            return;
        }
        else
            ++itr;
    }
}

bool DICreatureCalcStats(Creature* creature)
{
    if (!DynamicInstanceEnable)
        return false;

    if (creature->isTotem() || creature->isTrigger() || creature->GetCreatureType() == CREATURE_TYPE_CRITTER || creature->isSpiritService())
        return false;

    Map* map = creature->GetMap();

    if (!map || !DICreateOrExisted(map))
        return false;

    uint32 instanceid = map->GetInstanceId();

    const CreatureTemplate* cinfo = creature->GetCreatureTemplate();

    uint8 level = DIData[instanceid].level;

    uint8 clevel = cinfo->maxlevel;
    if (clevel > 80) clevel = 80;

    creature->SetLevel(level);

    uint8 expansion = 0;
    if (level > 59) expansion++;
    if (level > 69) expansion++;

    uint32 rank = creature->isPet() ? 0 : cinfo->rank;

    CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(level, cinfo->unit_class);

    float damagemod = 1.0f;
    float healthmod = 1.0f;

    if (rank == CREATURE_ELITE_NORMAL)
        healthmod *= sWorld->getRate(RATE_CREATURE_NORMAL_HP);
    else if (rank == CREATURE_ELITE_ELITE)
        healthmod *= sWorld->getRate(RATE_CREATURE_ELITE_ELITE_HP);
    else if (rank == CREATURE_ELITE_RAREELITE)
    {
        healthmod *= sWorld->getRate(RATE_CREATURE_ELITE_RAREELITE_HP);
        damagemod *= 1.5;
    }
    else if (rank == CREATURE_ELITE_WORLDBOSS)
        healthmod *= sWorld->getRate(RATE_CREATURE_ELITE_WORLDBOSS_HP);
    else if (rank == CREATURE_ELITE_RARE)
        healthmod *= sWorld->getRate(RATE_CREATURE_ELITE_RARE_HP);
    else
        healthmod *= sWorld->getRate(RATE_CREATURE_ELITE_ELITE_HP);

    if (rank != CREATURE_ELITE_NORMAL)
    {
        damagemod *= 2;
        healthmod *= 3;
    }

    uint32 basehp = stats->GenerateHealth(expansion, cinfo->ModHealth);
    uint32 health = uint32(basehp * healthmod);

    float dmgbase = (2.5 * pow(1.05, level * 2) / 2) * (cinfo->baseattacktime / 1000) * damagemod;
    float dmgmin = dmgbase * 0.95;
    float dmgmax = dmgbase * 1.05;

    creature->SetCreateHealth(health);
    creature->SetMaxHealth(health);
    creature->SetHealth(health);
    creature->ResetPlayerDamageReq();

    // mana
    uint32 mana = uint32(stats->GenerateMana(cinfo) * healthmod);

    creature->SetCreateMana(mana);
    creature->SetMaxPower(POWER_MANA, mana);                          //MAX Mana
    creature->SetPower(POWER_MANA, mana);

    // TODO: set UNIT_FIELD_POWER*, for some creature class case (energy, etc)
    creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)health);
    creature->SetModifierValue(UNIT_MOD_MANA, BASE_VALUE, (float)mana);

    creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, dmgmin);
    creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, dmgmax);

    creature->SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE, dmgmin);
    creature->SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE, dmgmax);

    creature->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, cinfo->attackpower * damagemod);

    creature->UpdateAllStats();
    return true;
}

class Mod_DynamicInstance_WorldScript : public WorldScript
{
    public:
        Mod_DynamicInstance_WorldScript() : WorldScript("Mod_DynamicInstance_WorldScript") { }

    void OnConfigLoad(bool /*reload*/)
    {
        DynamicInstanceEnable = ConfigMgr::GetBoolDefault("DynamicInstance.Enable", false);

        if (!DynamicInstanceEnable)
            return;

        DILoadDataFromDB();
    }
};

class Mod_DynamicInstance_AllInstanceScript : public AllInstanceScript
{
    public:
        Mod_DynamicInstance_AllInstanceScript() : AllInstanceScript("Mod_DynamicInstance_AllInstanceScript") { }

    void AllInstanceDeleteFromDB(uint32 instanceid)
    {
        DIRemoveData(instanceid);
    }

    void AllInstanceOnPlayerEnter(Map* map, Player* /*player*/)
    {
        DICreateOrExisted(map);
    }
};

class Mod_DynamicInstance_AllCreatureScript : public AllCreatureScript
{
    public:
        Mod_DynamicInstance_AllCreatureScript() : AllCreatureScript("Mod_DynamicInstance_AllCreatureScript") { }

    void AllCreatureSelectLevel(Creature* creature, bool& needSetStats)
    {
        if (!DynamicInstanceEnable)
            return;

        needSetStats = !DICreatureCalcStats(creature);
    }

    void AllCreatureCreate(Creature* creature)
    {
        DICreatureCalcStats(creature);
    }

    void AllCreatureCreateLoot(Creature* /*creature*/, uint32& lootid)
    {
        if (!DynamicInstanceEnable || lootid != 0)
            return;

        // new lootid
    }

    void AllCreatureSpellDamageMod(Creature* /*creature*/, float& /*doneTotalMod*/)
    {
        if (!DynamicInstanceEnable)
            return;
        // temp disabled, need for spells with const damage or deleted all spells with dynamic damage (dependence on the level)
        /*Map* map = creature->GetMap();

        if (!map || !map->IsDungeon())
            return;

        uint32 instanceid = map->GetInstanceId();

        if (!DIData[instanceid].isValid())
            return;

        uint8 level = creature->GetCreatureTemplate()->maxlevel;
        if (level > 80) level = 80;

        doneTotalMod *= pow(1.05, (1.55 * (DIData[instanceid].level - level)));*/
    }
};

void AddSC_Mod_DynamicInstance()
{
    new Mod_DynamicInstance_WorldScript;
    new Mod_DynamicInstance_AllInstanceScript;
    new Mod_DynamicInstance_AllCreatureScript;
}