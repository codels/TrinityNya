#include "ScriptPCH.h"
#include "Config.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"

bool KillBossEnable = true;
bool KillBossSaveDB = true;

class Mod_KillBoss_WorldScript : public WorldScript
{
    public:
        Mod_KillBoss_WorldScript()
            : WorldScript("Mod_KillBoss_WorldScript")
        {
        }

    void OnConfigLoad(bool reload)
    {
        KillBossEnable = ConfigMgr::GetBoolDefault("KillBoss.Enable", true);

        if (!KillBossEnable)
            return;

        KillBossSaveDB = ConfigMgr::GetBoolDefault("KillBoss.SaveDB", true);
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

        Player* recipient = creature->GetLootRecipient();

        if (!recipient) return;

        bool isRaid = creature->GetMap()->IsRaid();
        uint32 oEntry = creature->GetEntry();
        uint32 Entry = oEntry;
        uint8 spawnMode = creature->GetMap()->GetSpawnMode();
        uint32 GuildId = 0;
        bool IsGuildKill = true;
        uint32 KillerCount = 0;
        std::string TeamKill;

        if(spawnMode > 0 && spawnMode < MAX_DIFFICULTY)
            if(CreatureTemplate const* normalInfo = sObjectMgr->GetCreatureTemplate(Entry))
                if(normalInfo->DifficultyEntry[spawnMode] != 0)
                    Entry = normalInfo->DifficultyEntry[spawnMode];

        if(Group *pGroup = recipient->GetGroup())
        {
            for(GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* Temp = itr->getSource();

                if(!Temp) continue;

                KillerCount++;
                if (KillBossSaveDB)
                {
                    std::ostringstream PeopleData;
                    PeopleData << Temp->GetGUIDLow() << ":";
                    PeopleData << Temp->GetName() << ":";
                    PeopleData << Temp->getLevel() << ":";
                    PeopleData << Temp->GetGuildId() << ":";
                    PeopleData << int(Temp->isAlive()) << ":";
                    PeopleData << int(Temp->IsAtGroupRewardDistance(creature)) << " ";
                    TeamKill += PeopleData.str();
                }

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

            if (KillBossSaveDB)
            {
                std::ostringstream PeopleData;
                PeopleData << recipient->GetGUIDLow() << ":";
                PeopleData << recipient->GetName() << ":";
                PeopleData << recipient->getLevel() << ":";
                PeopleData << recipient->GetGuildId() << ":";
                PeopleData << int(recipient->isAlive()) << ":";
                PeopleData << 1 << " ";
                TeamKill += PeopleData.str();
            }
        }

        if(!IsGuildKill && GuildId != 0) GuildId = 0;
        if(GuildId == 0 && IsGuildKill) IsGuildKill = false;

        if(IsGuildKill)
        {
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
            } else if (spawnMode == DUNGEON_DIFFICULTY_HEROIC)
                strBoss << " (Героический режим)";

            strBoss << " повержен гильдией \"";
            strBoss << sGuildMgr->GetGuildById(GuildId)->GetName();
            strBoss << "\", составом в ";
            strBoss << KillerCount;
            strBoss << (KillerCount > 4) ? " человек" : " человекa";

            sWorld->SendWorldText(LANG_AUTO_BROADCAST, strBoss.str().c_str());
        }

        if (!KillBossSaveDB) return;
                        
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