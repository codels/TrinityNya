#include "ScriptPCH.h"
#include "Config.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"

#define SQL_KILLBOSS_LOG "INSERT INTO `world_killboss` (`GuildId`, `CreatureEntry`, `KillerCount`, `KillData`, `MapSpawnMode`, `MapId`, `MapRaid`, `CreatureName`, `MapName`) VALUES ('%u', '%u', '%u', '%s', '%u', '%u', '%u', '%s', '%s')"
#define DATA_SEPARATOR ":"
#define PEOPLE_SEPARATOR " "

bool    KillBossEnable      = true;
bool    KillBossLog         = true;
int32  KillBossNormalText   = 11006;
int32  KillBossHeroicText   = 11007;

class Mod_KillBoss_WorldScript : public WorldScript
{
    public:
        Mod_KillBoss_WorldScript() : WorldScript("Mod_KillBoss_WorldScript") { }

    void OnConfigLoad(bool /*reload*/)
    {
        KillBossEnable      = ConfigMgr::GetBoolDefault("KillBoss.Enable", true);

        if (!KillBossEnable)
            return;

        KillBossLog         = ConfigMgr::GetBoolDefault("KillBoss.Log", true);
        KillBossNormalText  = ConfigMgr::GetIntDefault("KillBoss.NormalTextId", 11006);
        KillBossHeroicText  = ConfigMgr::GetIntDefault("KillBoss.HeroicTextId", 11007);
    }
};

class Mod_KillBoss_AllCreatureScript : public AllCreatureScript
{
    public:
        Mod_KillBoss_AllCreatureScript() : AllCreatureScript("Mod_KillBoss_AllCreatureScript") { }

    void AllCreatureJustDied(Creature* creature)
    {
        if (!KillBossEnable)
            return;

        if (creature->GetCreatureTemplate()->rank != CREATURE_ELITE_WORLDBOSS)
            return;

        Player* recipient = creature->GetLootRecipient();
        if (!recipient)
            return;

        Map* map = creature->GetMap();
        if (!map)
            return;

        uint32 Entry = creature->GetEntry();
        InstanceMap* instance = map->ToInstanceMap();
        std::string bossName(creature->GetNameForLocaleIdx(sObjectMgr->GetDBCLocaleIndex()));
        std::string mapName(creature->GetMap()->GetMapName());
        
        uint8 spawnMode = map->GetSpawnMode();
        uint32 GuildId = 0;
        bool IsGuildKill = true;
        uint32 KillerCount = 0;
        std::string TeamKill;

        if(Group *pGroup = recipient->GetGroup())
        {
            for(GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* Temp = itr->getSource();

                if(!Temp)
                    continue;

                KillerCount++;
                uint32 playerGuildId = Temp->GetGuildId();
                if (KillBossLog)
                {
                    std::ostringstream PeopleData;
                    PeopleData << Temp->GetGUIDLow() << DATA_SEPARATOR;
                    PeopleData << Temp->GetName() << DATA_SEPARATOR;
                    PeopleData << Temp->getLevel() << DATA_SEPARATOR;
                    PeopleData << playerGuildId << DATA_SEPARATOR;
                    PeopleData << int(Temp->isAlive()) << DATA_SEPARATOR;
                    PeopleData << int(Temp->IsAtGroupRewardDistance(creature)) << PEOPLE_SEPARATOR;
                    TeamKill += PeopleData.str();
                }

                if (IsGuildKill)
                {
                    if (GuildId == 0)
                        GuildId = playerGuildId;

                    IsGuildKill = GuildId != 0 && playerGuildId == GuildId;
                }
            }
        }
        else
        {
            KillerCount = 1;
            GuildId = recipient->GetGuildId();
            IsGuildKill = GuildId != 0;

            if (KillBossLog)
            {
                std::ostringstream PeopleData;
                PeopleData << recipient->GetGUIDLow() << DATA_SEPARATOR;
                PeopleData << recipient->GetName() << DATA_SEPARATOR;
                PeopleData << recipient->getLevel() << DATA_SEPARATOR;
                PeopleData << recipient->GetGuildId() << DATA_SEPARATOR;
                PeopleData << int(recipient->isAlive()) << DATA_SEPARATOR;
                PeopleData << 1 << PEOPLE_SEPARATOR;
                TeamKill += PeopleData.str();
            }
        }

        if (GuildId == 0 && IsGuildKill)
            IsGuildKill = false;

        if (!IsGuildKill)
            GuildId = 0;

        if (IsGuildKill)
        {
            int32 TextId = KillBossNormalText;
            uint32 mapMaxPlayers = instance ? instance->GetMaxPlayers() : 40;
            std::string guildName(sGuildMgr->GetGuildById(GuildId)->GetName());

            if (map->IsRaid())
            {
                if (spawnMode == RAID_DIFFICULTY_25MAN_HEROIC || spawnMode == RAID_DIFFICULTY_10MAN_HEROIC)
                    TextId = KillBossHeroicText;
            }
            else if (spawnMode == DUNGEON_DIFFICULTY_HEROIC)
                    TextId = KillBossHeroicText;

            sWorld->SendWorldText(TextId, mapName.c_str(), bossName.c_str(), mapMaxPlayers, guildName.c_str(), KillerCount);
        }

        if (!KillBossLog) return;

        CharacterDatabase.EscapeString(TeamKill);
        CharacterDatabase.EscapeString(bossName);
        CharacterDatabase.EscapeString(mapName);
        CharacterDatabase.PExecute(SQL_KILLBOSS_LOG, GuildId, Entry, KillerCount, TeamKill.c_str(), spawnMode, map->GetId(), uint8(map->IsRaid()), bossName.c_str(), mapName.c_str());
    }
};

void AddSC_Mod_KillBoss()
{
    new Mod_KillBoss_AllCreatureScript;
    new Mod_KillBoss_WorldScript;
}