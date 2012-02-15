#include "ScriptPCH.h"
#include "Config.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"

bool KillBossEnable = true;

class Mod_KillBoss_WorldScript : public WorldScript
{
    public:
        Mod_KillBoss_WorldScript()
            : WorldScript("Mod_KillBoss_WorldScript")
        {
        }

    void SetInitialWorldSettings()
    {
        KillBossEnable = ConfigMgr::GetBoolDefault("KillBoss.Enable", true);

        if (!KillBossEnable)
            return;
    }
};

class Mod_KillBoss_AllCreatureScript : public AllCreatureScript
{
    public:
        Mod_KillBoss_AllCreatureScript()
            : AllCreatureScript("Mod_KillBoss_AllCreatureScript")
        {
        }

    void AllCreatureJustDied(Creature* creature)
    {
        if (!KillBossEnable)
            return;

        if (creature->GetCreatureInfo()->rank != CREATURE_ELITE_WORLDBOSS)
            return;

/*
            bool NeedLog = true;
            switch(creature->GetEntry())
            {
                case 13020:
                case 31146:
                case 34925:
                case 36731:
                case 31099:
                    NeedLog = false;
                    break;
                case 4949:
                    sWorld->AllCastSpell(35076, ALLIANCE);
                    sWorld->AllCastSpell(16618, ALLIANCE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель орков пал!");
                    break;
                case 10540:
                    sWorld->AllCastSpell(35076, ALLIANCE);
                    sWorld->AllCastSpell(16618, ALLIANCE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель троллей пал!");
                    break;
                case 3057:
                    sWorld->AllCastSpell(35076, ALLIANCE);
                    sWorld->AllCastSpell(16618, ALLIANCE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель тауренов пал!");
                    break;
                case 10181:
                    sWorld->AllCastSpell(35076, ALLIANCE);
                    sWorld->AllCastSpell(16618, ALLIANCE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель нежити пал!");
                    break;
                case 16802:
                    sWorld->AllCastSpell(35076, ALLIANCE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель кровавых эльфов пал!");
                    sWorld->AllCastSpell(16618, ALLIANCE);
                    break;
                case 29611:
                    sWorld->AllCastSpell(16618, HORDE);
                    sWorld->AllCastSpell(35076, HORDE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель людей пал!");
                    break;
                case 7937:
                    sWorld->AllCastSpell(16618, HORDE);
                    sWorld->AllCastSpell(35076, HORDE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель гномов пал!");
                    break;
                case 2784:
                    sWorld->AllCastSpell(16618, HORDE);
                    sWorld->AllCastSpell(35076, HORDE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель дварфов пал!");
                    break;
                case 17468:
                    sWorld->AllCastSpell(16618, HORDE);
                    sWorld->AllCastSpell(35076, HORDE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель дреней пал!");
                    break;
                case 7999:
                    sWorld->AllCastSpell(16618, HORDE);
                    sWorld->AllCastSpell(35076, HORDE);
                    sWorld->SendWorldText(LANG_AUTO_BROADCAST, "Предводитель ночных эльфов пал!");
                    break;
                default:
                    break;
            }
*/
        uint32 GuildId = 0;
        bool IsGuildKill = true;
        uint32 KillerCount = 0;
        std::string TeamKill;

        Player* recipient = creature->GetLootRecipient();

        if (!recipient) return;
        
        if(Group *pGroup = recipient->GetGroup())
        {
            for(GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* Temp = itr->getSource();

                if(!Temp) continue;

                KillerCount++;
                std::ostringstream PeopleData;
                PeopleData << Temp->GetGUIDLow() << ":";
                PeopleData << Temp->GetName() << ":";
                PeopleData << int(Temp->getLevel()) << ":";
                PeopleData << int(Temp->GetGuildId()) << ":";
                PeopleData << int(Temp->isAlive()) << ":";
                PeopleData << int(Temp->isDead()) << ":";
                PeopleData << int(Temp->IsAtGroupRewardDistance(creature)) << " ";
                TeamKill += PeopleData.str();

                if (IsGuildKill)
                {
                    if(GuildId == 0)
                    {
                        GuildId = Temp->GetGuildId();
                        if(GuildId == 0) IsGuildKill = false; 
                    } else if (Temp->GetGuildId() != GuildId) IsGuildKill = false;
                }
            }
        } else {
            KillerCount = 1;
            GuildId = recipient->GetGuildId();
            if(GuildId == 0) IsGuildKill = false;

            std::ostringstream PeopleData;
            PeopleData << recipient->GetGUIDLow() << ":";
            PeopleData << recipient->GetName() << ":";
            PeopleData << int(recipient->getLevel()) << ":";
            PeopleData << int(recipient->GetGuildId()) << ":";
            PeopleData << int(recipient->isAlive()) << ":";
            PeopleData << int(recipient->isDead()) << ":";
            PeopleData << int(1) << " ";
            TeamKill += PeopleData.str();
        }

        if(!IsGuildKill && GuildId != 0) GuildId = 0;
        if(GuildId == 0 && IsGuildKill) IsGuildKill = false;
                        
        uint32 oEntry = creature->GetEntry();
        uint32 Entry = oEntry;
        uint8 spawnMode = creature->GetMap()->GetSpawnMode();
        bool isRaid = creature->GetMap()->IsRaid();

        if(spawnMode > 0 && spawnMode < MAX_DIFFICULTY)
        {
            if(CreatureTemplate const* normalInfo = sObjectMgr->GetCreatureTemplate(Entry))
            {
                if(normalInfo->DifficultyEntry[spawnMode] != 0)
                    Entry = normalInfo->DifficultyEntry[spawnMode];
            }
        }

        if(IsGuildKill)
        {
            Guild* guild = sGuildMgr->GetGuildById(GuildId);
            std::ostringstream strBoss;
            strBoss << creature->GetMap()->GetMapName() << " - ";
            strBoss << creature->GetNameForLocaleIdx(sObjectMgr->GetDBCLocaleIndex());

            if (isRaid)
            {
                switch(spawnMode)
                {
                    case RAID_DIFFICULTY_10MAN_NORMAL:
                        strBoss << " (Сложность на 10 человек)";
                        break;
                    case RAID_DIFFICULTY_25MAN_NORMAL:
                        strBoss << " (Сложность на 25 человек)";
                        break;
                    case RAID_DIFFICULTY_10MAN_HEROIC:
                        strBoss << " (Сложность героический на 10 человек)";
                        break;
                    case RAID_DIFFICULTY_25MAN_HEROIC:
                        strBoss << " (Сложность героический на 25 человек)";
                        break;
                    default:
                        break;
                }
            } else {
                switch(spawnMode)
                {
                    case DUNGEON_DIFFICULTY_NORMAL:
                        break;
                    case DUNGEON_DIFFICULTY_HEROIC:
                        strBoss << " (Героический режим)";
                        break;
                    default:
                        break;
                }
            }

            strBoss << " повержен гильдией \"";
            strBoss << guild->GetName();
            strBoss << "\", составом в ";
            strBoss << KillerCount;
            if(KillerCount > 4)
            {
                strBoss << " человек";
            } else {
                strBoss << " человекa";
            }
            sWorld->SendWorldText(LANG_AUTO_BROADCAST, strBoss.str().c_str());
        }
                        
        std::string boss_name(creature->GetNameForLocaleIdx(sObjectMgr->GetDBCLocaleIndex()));
        std::string instance_name(creature->GetMap()->GetMapName());

        WorldDatabase.EscapeString(boss_name);
        WorldDatabase.EscapeString(instance_name);
        CharacterDatabase.PExecute("INSERT INTO _killLog (guildId, creatureEntry, killDate, killerCount, killData, instanceMode, mapId, isRaid, creatureParent, creatureName, mapName) VALUES ('%u', '%u', NOW(), '%u', '%s', '%u', '%u', '%u', '%u', '%s', '%s')", GuildId, Entry, KillerCount, TeamKill.c_str(), spawnMode, creature->GetMap()->GetId(), int(isRaid), oEntry, boss_name.c_str(), instance_name.c_str());
    }
};

void AddSC_Mod_KillBoss()
{
    new Mod_KillBoss_AllCreatureScript;
    new Mod_KillBoss_WorldScript;
}