//TRINITY NYA DEV
#include "ScriptPCH.h"
#include "Config.h"
#include <math.h>

#define DI_SQL_GET "SELECT `InstanceId`, `InstanceLevel` FROM `world_instance`"
#define DI_SQL_SAVE "REPLACE INTO `world_instance` (`InstanceId`, `InstanceLevel`) VALUES ('%u', '%u')"
#define DI_SQL_DELETE "DELETE FROM `world_instance` WHERE `InstanceId` = '%u'"

bool    DynamicInstanceEnable   = false;

struct DITemplate
{
    DITemplate() : level(0) {}
    uint8 level;

    bool isValid() const { return level != 0; }
};

typedef std::map<uint32, DITemplate> DIMap;
DIMap DIData;

void DISaveData(uint32 instanceid)
{
    //debug
    sLog->outError("DISaveData: instance %u level %u", instanceid, DIData[instanceid].level);

    if (!DIData[instanceid].isValid())
        return;

    CharacterDatabase.PExecute(DI_SQL_SAVE, instanceid, DIData[instanceid].level);
}

void DIRemoveData(uint32 instanceid)
{
    //debug
    sLog->outError("DIRemoveData: instance %u", instanceid);

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

void DILoadDataFromDB()
{
    DIData.clear();

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

bool DIGetLevel(Map* map)
{
    Map::PlayerList const &players = map->GetPlayers();

    if (!players.isEmpty())
        if (Player* player = players.begin()->getSource())
        {
            DIData[map->GetInstanceId()].level = player->getLevel();
            DISaveData(map->GetInstanceId());
            return true;
        }
    return false;
}

float _GetHealthMod(int32 Rank)
{
    switch (Rank)                                           // define rates for each elite rank
    {
        case CREATURE_ELITE_NORMAL:
            return sWorld->getRate(RATE_CREATURE_NORMAL_HP);
        case CREATURE_ELITE_ELITE:
            return sWorld->getRate(RATE_CREATURE_ELITE_ELITE_HP);
        case CREATURE_ELITE_RAREELITE:
            return sWorld->getRate(RATE_CREATURE_ELITE_RAREELITE_HP);
        case CREATURE_ELITE_WORLDBOSS:
            return sWorld->getRate(RATE_CREATURE_ELITE_WORLDBOSS_HP);
        case CREATURE_ELITE_RARE:
            return sWorld->getRate(RATE_CREATURE_ELITE_RARE_HP);
        default:
            return sWorld->getRate(RATE_CREATURE_ELITE_ELITE_HP);
    }
}

bool DICreatureCalcStats(Creature* creature)
{
    if (!DynamicInstanceEnable)
        return false;

    if (creature->isTotem() || creature->isTrigger() || creature->GetCreatureType() == CREATURE_TYPE_CRITTER || creature->isSpiritService())
        return false;

    Map* map = creature->GetMap();

    if (!map || !map->IsDungeon())
        return false;

    uint32 instanceid = map->GetInstanceId();

    if (!DIData[instanceid].isValid() && !DIGetLevel(map))
        return false;

    const CreatureTemplate* cinfo = creature->GetCreatureTemplate();

    uint8 level = DIData[instanceid].level;

    uint8 clevel = cinfo->maxlevel;
    if (clevel > 80) clevel = 80;

    creature->SetLevel(level);

    uint8 expansion = 0;
    if (level > 59) expansion++;
    if (level > 69) expansion++;

    uint32 rank = creature->isPet()? 0 : cinfo->rank;

    CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(level, cinfo->unit_class);

    //damage
    float damagemod = 1.0f;//_GetDamageMod(rank);

    float healthmod = _GetHealthMod(rank);

    if (rank != CREATURE_ELITE_NORMAL)
    {
        damagemod *= 2;
        healthmod *= 3;
    }
    if (rank == CREATURE_ELITE_RAREELITE) damagemod *= 1.5;

    uint32 basehp = stats->GenerateHealth(expansion, cinfo->ModHealth);
    uint32 health = uint32(basehp * healthmod);

    float dmgbase = (2.5 * pow(1.05, level * 2) / 2) * (cinfo->baseattacktime / 1000);
    float dmgmin = dmgbase * 0.95;
    float dmgmax = dmgbase * 1.05;

    creature->SetCreateHealth(health);
    creature->SetMaxHealth(health);
    creature->SetHealth(health);
    creature->ResetPlayerDamageReq();

    // mana
    uint32 mana = stats->GenerateMana(cinfo);

    creature->SetCreateMana(mana);
    creature->SetMaxPower(POWER_MANA, mana);                          //MAX Mana
    creature->SetPower(POWER_MANA, mana);

    // TODO: set UNIT_FIELD_POWER*, for some creature class case (energy, etc)
    creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)health);
    creature->SetModifierValue(UNIT_MOD_MANA, BASE_VALUE, (float)mana);

    creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, dmgmin * damagemod);
    creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, dmgmax * damagemod);

    creature->SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE, dmgmin * damagemod);
    creature->SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE, dmgmax * damagemod);

    creature->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, cinfo->attackpower * damagemod);

    //ExtraLoot = 6;

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

    void AllInstanceAdd(InstanceSave* /*instanceSave*/)
    {
        if (!DynamicInstanceEnable)
            return;

        //DISaveData(instanceSave->GetInstanceId());
    }

    void AllInstanceDeleteFromDB(uint32 instanceid)
    {
        if (!DynamicInstanceEnable)
            return;

        sLog->outError("DI: AllInstanceDeleteFromDB instance %u", instanceid);

        DIRemoveData(instanceid);
    }

    void AllInstanceOnPlayerEnter(Map* map, Player* /*player*/)
    {
        if (!DynamicInstanceEnable)
            return;

        uint32 instanceid = map->GetInstanceId();

        sLog->outError("DI: AllInstanceOnPlayerEnter instance %u", instanceid);

        if (DIData[instanceid].isValid())
            return;

        Map::PlayerList const &players = map->GetPlayers();

        if (!players.isEmpty())
            if (Player* player = players.begin()->getSource())
                DIData[instanceid].level = player->getLevel();

        DISaveData(instanceid);
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
        if (!DynamicInstanceEnable)
            return;

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
        // temp disabled
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