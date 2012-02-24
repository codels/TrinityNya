#include "ScriptPCH.h"
#include "Config.h"

#define SPELL_MASK_CLASS        1
#define SPELL_MASK_RIDING       2
#define SPELL_MASK_MOUNT        4
#define SPELL_MASK_WEAPON       8
#define SPELL_MASK_PROFESSION   16

bool AutoLearnEnable        = true;
bool AutoLearnCheckLevel    = true;
bool DualSpec               = true;
bool SpellClass             = true;
bool SpellRiding            = true;
bool SpellMount             = true;
bool SpellWeapon            = true;
bool SpellProfession        = false;

struct LearnSpellForClassInfo
{
    uint32  SpellId;
    uint16  SpellMask;
    uint32  RequiredClassMask;
    uint32  RequiredRaceMask;
    uint8   RequiredLevel;
    uint32  RequiredSpellId;
    uint16  RequiredSkillId;
    uint16  RequiredSkillValue;
};

std::vector<LearnSpellForClassInfo> LearnSpellForClass;

class Mod_AutoLearn_WorldScript : public WorldScript
{
    public:
        Mod_AutoLearn_WorldScript()
            : WorldScript("Mod_AutoLearn_WorldScript")
        {
        }

    void Clear()
    {
        LearnSpellForClass.clear();
    }

    void LoadDataFromDataBase()
    {
        Clear();

        if (!AutoLearnEnable)
            return;

        sLog->outString();
        sLog->outString("Loading AutoLearn...");
        uint32 oldMSTime = getMSTime();

        QueryResult result = WorldDatabase.PQuery("SELECT SpellId, SpellMask, RequiredClassMask, RequiredRaceMask, RequiredLevel, RequiredSpellId, RequiredSkillId, RequiredSkillValue FROM `world_autolearn`");

        if (!result)
            return;

        uint16 count = 0;

        do
        {
            Field* fields = result->Fetch();

            LearnSpellForClassInfo Spell;

            Spell.SpellId               = fields[0].GetUInt32();
            Spell.SpellMask             = fields[1].GetUInt16();
            Spell.RequiredClassMask     = fields[2].GetUInt32();
            Spell.RequiredRaceMask      = fields[3].GetUInt32();
            Spell.RequiredLevel         = fields[4].GetUInt8();
            Spell.RequiredSpellId       = fields[5].GetUInt32();
            Spell.RequiredSkillId       = fields[6].GetUInt16();
            Spell.RequiredSkillValue    = fields[7].GetUInt16();

            if (!sSpellMgr->GetSpellInfo(Spell.SpellId))
            {
                sLog->outErrorDb("AutoLearn: Spell (ID: %u) non-existing", Spell.SpellId);
                continue;
            }

            if (!SpellClass && Spell.SpellMask & SPELL_MASK_CLASS)
                Spell.SpellMask -= SPELL_MASK_CLASS;

            if (!SpellRiding && Spell.SpellMask & SPELL_MASK_RIDING)
                Spell.SpellMask -= SPELL_MASK_RIDING;

            if (!SpellMount && Spell.SpellMask & SPELL_MASK_MOUNT)
                Spell.SpellMask -= SPELL_MASK_MOUNT;

            if (!SpellWeapon && Spell.SpellMask & SPELL_MASK_WEAPON)
                Spell.SpellMask -= SPELL_MASK_WEAPON;

            if (!SpellProfession && Spell.SpellMask & SPELL_MASK_PROFESSION)
                Spell.SpellMask -= SPELL_MASK_PROFESSION;

            // Skip spell
            if (Spell.SpellMask == 0)
                continue;

            if (Spell.RequiredClassMask != 0 && !(Spell.RequiredClassMask & CLASSMASK_ALL_PLAYABLE))
            {
                sLog->outErrorDb("AutoLearn: Spell (ID: %u) RequiredClassMask (Mask: %u) non-existing ", Spell.SpellId, Spell.RequiredClassMask);
                continue;
            }

            if (Spell.RequiredRaceMask != 0 && !(Spell.RequiredRaceMask & RACEMASK_ALL_PLAYABLE))
            {
                sLog->outErrorDb("AutoLearn: Spell (ID: %u) RequiredRaceMask (Mask: %u) non-existing ", Spell.SpellId, Spell.RequiredRaceMask);
                continue;
            }

            if (Spell.RequiredSpellId != 0 && !sSpellMgr->GetSpellInfo(Spell.RequiredSpellId))
            {
                sLog->outErrorDb("AutoLearn: Spell (ID: %u) RequiredSpellId (ID: %u) non-existing", Spell.SpellId, Spell.RequiredSpellId);
                continue;
            }

            LearnSpellForClass.push_back(Spell);
            ++count;
        }
        while (result->NextRow());

        sLog->outString(">> Loaded %u spells for AutoLearn in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        sLog->outString();
    }

    void OnConfigLoad(bool reload)
    {
        AutoLearnEnable         = ConfigMgr::GetBoolDefault("AutoLearn.Enable",             true);

        if (!AutoLearnEnable)
            return;

        AutoLearnCheckLevel     = ConfigMgr::GetBoolDefault("AutoLearn.Check.Level",        true);
        DualSpec                = ConfigMgr::GetBoolDefault("AutoLearn.DualSpec",           true);

        SpellClass              = ConfigMgr::GetBoolDefault("AutoLearn.SpellClass",         true);
        SpellRiding             = ConfigMgr::GetBoolDefault("AutoLearn.SpellRiding",        true);
        SpellMount              = ConfigMgr::GetBoolDefault("AutoLearn.SpellMount",         true);
        SpellWeapon             = ConfigMgr::GetBoolDefault("AutoLearn.SpellWeapon",        true);
        SpellProfession         = ConfigMgr::GetBoolDefault("AutoLearn.SpellProfession",    false);

        LoadDataFromDataBase();
    }
};

class Mod_AutoLearn_PlayerScript : public PlayerScript
{
    public:
        Mod_AutoLearn_PlayerScript()
            : PlayerScript("Mod_AutoLearn_PlayerScript")
        {
        }

    void OnLevelChanged(Player* Player, uint8 oldLevel)
    {
        if (!AutoLearnEnable || !AutoLearnCheckLevel) return;

        uint16 SpellMask = 0;

        if (SpellClass)         SpellMask += SPELL_MASK_CLASS;
        if (SpellRiding)        SpellMask += SPELL_MASK_RIDING;
        if (SpellMount)         SpellMask += SPELL_MASK_MOUNT;
        if (SpellWeapon)        SpellMask += SPELL_MASK_WEAPON;
        if (SpellProfession)    SpellMask += SPELL_MASK_PROFESSION;

        AutoLearnSpell(SpellMask, Player);
        learnDualSpec(Player);
    }

    void OnPlayerSkillUpdate(Player* Player, uint16 SkillId, uint16 /*SkillValue*/, uint16 SkillNewValue)
    {
        if (AutoLearnEnable && SpellProfession)
            AutoLearnSpell(SPELL_MASK_PROFESSION, Player, SkillId, SkillNewValue);
    }

    void AutoLearnSpell(uint16 SpellMask, Player* Player, uint16 SkillId = 0, uint16 SkillValue = 0)
    {
        if (SpellMask == 0) return;

        uint32  PlayerClassMask = Player->getClassMask();
        uint32  PlayerRaceMask  = Player->getRaceMask();
        uint8   PlayerLevel     = Player->getLevel();

        for (uint16 i = 0; i < LearnSpellForClass.size(); ++i)
        {
            LearnSpellForClassInfo &Spell = LearnSpellForClass[i];
            if (!(Spell.SpellMask & SpellMask)) continue;
            if (Spell.RequiredClassMask != 0 && !(Spell.RequiredClassMask & PlayerClassMask)) continue;
            if (Spell.RequiredRaceMask != 0 && !(Spell.RequiredRaceMask & PlayerRaceMask)) continue;
            if (Spell.RequiredLevel > PlayerLevel) continue;
            if (Spell.RequiredSkillId != SkillId) continue;
            if (Spell.RequiredSkillValue > SkillValue) continue;
            if (Player->HasSpell(Spell.SpellId)) continue;
            if (Spell.RequiredSpellId != 0 && !Player->HasSpell(Spell.RequiredSpellId)) continue;

            Player->learnSpell(Spell.SpellId, false);
        }
    }

    void learnDualSpec(Player* Player)
    {
        if (!DualSpec) return;

        if (Player->getLevel() < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL)) return;

        if (Player->GetSpecsCount() != 1) return;

        Player->CastSpell(Player, 63680, true, NULL, NULL, Player->GetGUID());
        Player->CastSpell(Player, 63624, true, NULL, NULL, Player->GetGUID());
    }
};

void AddSC_Mod_AutoLearn()
{
    new Mod_AutoLearn_PlayerScript;
    new Mod_AutoLearn_WorldScript;
}