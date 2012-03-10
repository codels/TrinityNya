#include "ScriptPCH.h"
#include "Config.h"
#include "Battleground.h"
#include "Guild.h"
#include "GuildMgr.h"

#define SQL_HISTORY "INSERT INTO world_battleground (MapId, BattleTypeId, BattleIsArena, BattleIsRate, BattleTime, WinnerGuildId, WinnerCount, WinnerData, LooserGuildId, LooserCount, LooserData, WinnerTeam, BattleBracket, BattleLevelMin, BattleLevelMax, WinnerArenaTeamId, LooserArenaTeamId) VALUES ('%u', '%u', '%u', '%u', '%u', '%u', '%u', '%s', '%u', '%u', '%s', '%u', '%u', '%u', '%u', '%u', '%u')"

bool    BGHistoryEnable             = true;
int32   BGHistoryGuildWinText       = 11100;
int32   BGHistoryGuildLosText       = 11101;
int32   BGHistoryGuildWinLosText    = 11102;

class Mod_BattlegroundHistory_WorldScript : public WorldScript
{
    public:
        Mod_BattlegroundHistory_WorldScript() : WorldScript("Mod_BattlegroundHistory_WorldScript") { }

    void OnConfigLoad(bool /*reload*/)
    {
        BGHistoryEnable = ConfigMgr::GetBoolDefault("BattlegroundHistory.Enable", true);
    }

};

class Mod_BattlegroundHistory_AllBattlegroundScript : public AllBattlegroundScript
{
    public:
        Mod_BattlegroundHistory_AllBattlegroundScript() : AllBattlegroundScript("Mod_BattlegroundHistory_AllBattlegroundScript") { }

    void AllBattlegroundEnd(Battleground* bg)
    {
        if (!BGHistoryEnable)
            return;

        uint32 winner = bg->GetWinner() == WINNER_ALLIANCE ? ALLIANCE : HORDE;

        bool winIsGuild = true;
        bool loseIsGuild = true;
        uint32 winGuildId = 0;
        uint32 loseGuildId = 0;
        uint32 winCount = 0;
        uint32 loseCount = 0;
        std::string winTeam;
        std::string loseTeam;
        Guild* winGuild = NULL;
        Guild* loseGuild = NULL;

        uint32 damageDoneMaximum = 0;
        uint32 healingDoneMaximum = 0;
        std::string damageDonePlayerName;
        std::string healingDonePlayerName;

        for (Battleground::BattlegroundScoreMap::const_iterator itr = bg->GetPlayerScoresBegin(); itr != bg->GetPlayerScoresEnd(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(itr->first);
            if (!player)
                return;

            if(itr->second->DamageDone >= damageDoneMaximum)
            {
                damageDoneMaximum = itr->second->DamageDone;
                damageDonePlayerName = player->GetName();
            }

            if(itr->second->HealingDone >= healingDoneMaximum)
            {
                healingDoneMaximum = itr->second->HealingDone;
                healingDonePlayerName = player->GetName();
            }  

            uint32 playerGuildId = player->GetGuildId();

            std::ostringstream PeopleData;
            PeopleData << player->GetGUIDLow() << ":";
            PeopleData << player->GetName() << ":";
            PeopleData << uint32(player->getLevel()) << ":";
            PeopleData << uint32(playerGuildId) << ":";
            PeopleData << itr->second->KillingBlows << ":";
            PeopleData << itr->second->Deaths << ":";
            PeopleData << itr->second->HonorableKills << ":";
            PeopleData << itr->second->DamageDone << ":";
            PeopleData << itr->second->HealingDone << ":";
            PeopleData << itr->second->BonusHonor << ":";
            PeopleData << uint32(player->getRace()) << ":";
            PeopleData << uint32(player->getClass()) << ":";
            PeopleData << uint32(player->getGender()) << " ";

            if (bg->GetPlayerTeam(player->GetGUID()) == winner)
            {
                winCount++;
                winTeam += PeopleData.str();

                if (winIsGuild)
                {
                    if (winGuildId == 0)
                        winGuildId = playerGuildId;

                    winIsGuild = winGuildId != 0 && playerGuildId == winGuildId;
                }
            }
            else
            {
                loseCount++;
                loseTeam += PeopleData.str();

                if (loseIsGuild)
                {
                    if (loseGuildId == 0)
                        loseGuildId = playerGuildId;

                    loseIsGuild = loseGuildId != 0 && playerGuildId == loseGuildId;
                }
            }
        }

        if(winIsGuild && winGuildId != 0)
            winGuild = sGuildMgr->GetGuildById(winGuildId);

        if(loseIsGuild && loseGuildId != 0)
            loseGuild = sGuildMgr->GetGuildById(loseGuildId);

        if(!bg->isArena() && (winGuild || loseGuild))
        {
            if (winGuild)
            {
                if (loseGuild)
                    sWorld->SendWorldText(BGHistoryGuildWinLosText, winGuild->GetName().c_str(), loseGuild->GetName().c_str(), damageDonePlayerName.c_str(), healingDonePlayerName.c_str());
                else
                    sWorld->SendWorldText(BGHistoryGuildWinText, winGuild->GetName().c_str(), damageDonePlayerName.c_str(), healingDonePlayerName.c_str());
            }
            else if (loseGuild)
                sWorld->SendWorldText(BGHistoryGuildLosText, loseGuild->GetName().c_str(), damageDonePlayerName.c_str(), healingDonePlayerName.c_str());
        }

        uint32 winArenaTeam = 0;
        uint32 loseAreaTeam = 0;

        if(bg->isArena() && bg->isRated())
        {
            winArenaTeam = bg->GetArenaTeamIdForTeam(winner);
            loseAreaTeam = bg->GetArenaTeamIdForTeam(bg->GetOtherTeam(winner));
        }

        CharacterDatabase.PExecute(SQL_HISTORY, bg->GetMapId(), bg->GetTypeID(), uint32(bg->isArena()), uint32(bg->isRated()), uint32(bg->GetStartTime()/1000), winGuildId, winCount, winTeam.c_str(), loseGuildId, loseCount, loseTeam.c_str(), winner, bg->GetBracketId(), bg->GetMinLevel(), bg->GetMaxLevel(), winArenaTeam, loseAreaTeam);
    }
};


void AddSC_Mod_BattlegroundHistory()
{
    new Mod_BattlegroundHistory_WorldScript;
    new Mod_BattlegroundHistory_AllBattlegroundScript;
}