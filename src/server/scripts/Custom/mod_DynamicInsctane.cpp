//TRINITY NYA DEV
#include "ScriptPCH.h"
#include "Config.h"

bool    DynamicInstanceEnable   = false;

class Mod_DynamicInstance_WorldScript : public WorldScript
{
    public:
        Mod_DynamicInstance_WorldScript() : WorldScript("Mod_DynamicInstance_WorldScript") { }

    void OnConfigLoad(bool reload)
    {
        DynamicInstanceEnable = ConfigMgr::GetBoolDefault("DynamicInstance.Enable", false);

        if (!DynamicInstanceEnable)
            return;
    }
};
/*
void Creature::DynamicInstanceUpdate(uint32 level)
{
    
    DynamicInstanceLevel = level;

    CreatureTemplate const* cinfo = GetCreatureInfo();

    if (cinfo->maxlevel == 1)
        return;

    uint32 clevel = cinfo->maxlevel;
    if (clevel > 80) clevel = 80;

    DynamicInstanceSpellBonus = pow(1.05, (1.55 * (level - clevel)));

    SetLevel(level);

    uint8 expansion = 0;
    if (level > 59) expansion++;
    if (level > 69) expansion++;

    uint32 rank = isPet()? 0 : cinfo->rank;

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

    float dmgbase = 2.5 * pow(1.05, level * 2) / 2;
    //sLog->outError("Creature::DynamicInstanceLevel dmgbase: %f", dmgbase);
    float dmgmin = dmgbase * 0.95;
    float dmgmax = dmgbase * 1.05;

    SetCreateHealth(health);
    SetMaxHealth(health);
    SetHealth(health);
    ResetPlayerDamageReq();

    // mana
    uint32 mana = stats->GenerateMana(cinfo);

    SetCreateMana(mana);
    SetMaxPower(POWER_MANA, mana);                          //MAX Mana
    SetPower(POWER_MANA, mana);

    // TODO: set UNIT_FIELD_POWER*, for some creature class case (energy, etc)

    SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)health);
    SetModifierValue(UNIT_MOD_MANA, BASE_VALUE, (float)mana);



    SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, dmgmin * damagemod);
    SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, dmgmax * damagemod);

    SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE, dmgmin * damagemod);
    SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE, dmgmax * damagemod);

    SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, cinfo->attackpower * damagemod);

    ExtraLoot = 6;

    UpdateAllStats();
}*/

/*
void Creature::SelectLevel(const CreatureTemplate* cinfo)
{
    if (DynamicInstanceLevel != 0)
    {
        DynamicInstanceUpdate(DynamicInstanceLevel);
        return;
    }
*/

//, DynamicInstanceSpellBonus(0.0f), ExtraLoot(0), DynamicInstanceLevel(0)

/*
        uint32 DynamicInstanceLevel;
        float DynamicInstanceSpellBonus;
        void DynamicInstanceUpdate(uint32 level);
        uint32 ExtraLoot;
*/

/*
uint32 Unit::SpellDamageBonus(Unit* victim, SpellInfo const* spellProto, uint32 pdamage, DamageEffectType damagetype, uint32 stack)
    // ..done
    // Pet damage?
    if (GetTypeId() == TYPEID_UNIT && !ToCreature()->isPet())
    {
        DoneTotalMod *= ToCreature()->GetSpellDamageMod(ToCreature()->GetCreatureInfo()->rank);

        if (ToCreature()->DynamicInstanceSpellBonus != 0.0f)
        {
            DoneTotalMod *= ToCreature()->DynamicInstanceSpellBonus;
        }
    }

void Unit::Kill(Unit* victim, bool durabilityLoss)
            loot->clear();
            if (uint32 lootid = creature->GetCreatureInfo()->lootid)
            {
                if (creature->ExtraLoot == 0)
                {
                    loot->FillLoot(lootid, LootTemplates_Creature, looter, false, false, creature->GetLootMode());
                }
                else
                {
                    loot->FillLoot(creature->ExtraLoot, LootTemplates_Creature, looter, false, false, creature->GetLootMode());
                }
            }


            void OnCreatureCreate(Creature* creature)
            {
                Map::PlayerList const &players = instance->GetPlayers();
                uint32 instanceLevel = 20;

                if (!players.isEmpty())
                {
                    if (Player* player = players.begin()->getSource())
                        instanceLevel = player->getLevel();
                }

                if (creature && creature->getLevel() != instanceLevel)
                {
                    creature->DynamicInstanceUpdate(instanceLevel);
                }
            }
*/

void AddSC_Mod_DynamicInstance()
{
    //new Mod_DynamicInstance_WorldScript;
}