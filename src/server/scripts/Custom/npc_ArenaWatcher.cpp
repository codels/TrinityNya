/*######
## Arena Watcher by @FrozenSouL
## for creature:
## UPDATE `creature_template` SET `npcflag` = 1, `ScriptName` = 'npc_arena_watcher' WHERE `entry` = XXXX;
######*/

#include "CreatureTextMgr.h"
#include "Player.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ArenaTeamMgr.h"
#include "ArenaTeam.h"
#include "Config.h"

enum WatcherData
{
    GOSSIP_OFFSET = GOSSIP_ACTION_INFO_DEF + 10,
};

enum WatcherTexts
{
    SAY_NOT_FOUND_BRACKET       = 0,
    SAY_ARENA_NOT_IN_PROGRESS   = 1,
    SAY_TARGET_NOT_IN_WORLD     = 2,
    SAY_TARGET_NOT_IN_ARENA     = 3,
    SAY_TARGET_IS_GM            = 4,
};

enum WatcherStrings
{
    STRING_ARENA_2v2            = 11200,
    STRING_ARENA_3v3            = 11201,
    STRING_ARENA_5v5            = 11202,
    STRING_FOLLOW               = 11203,
};

bool ArenaWatcherEnable = false;
bool ArenaWatcherOnlyGM = false;
bool ArenaWatcherShowNoGames = false;
bool ArenaWatcherOnlyRated = false;
bool ArenaWatcherToPlayers = false;

std::vector<uint32> ArenaWatcherList;

void ArenaWatcherStart(Player* player)
{
    player->setDeathState(CORPSE);
    player->SetMovement(MOVE_WATER_WALK);
    player->SetMovement(MOVE_UNROOT);
    player->SetGMVisible(false);

    ArenaWatcherList.push_back(player->GetGUIDLow());
}

void ArenaWatcherAfterTeleport(Player* player)
{
    player->SetSpeed(MOVE_WALK, 3.0f, true);
    player->SetSpeed(MOVE_RUN, 3.0f, true);
    player->SetSpeed(MOVE_SWIM, 3.0f, true);
    player->SetSpeed(MOVE_FLIGHT, 3.0f, true);
    
    Battleground* bg = player->GetBattleground();
    if (!bg)
        return;

    WorldPacket status;
    BattlegroundQueueTypeId bgQueueTypeId = sBattlegroundMgr->BGQueueTypeId(bg->GetTypeID(), bg->GetArenaType());
    uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);
    sBattlegroundMgr->BuildBattlegroundStatusPacket(&status, bg, queueSlot, STATUS_IN_PROGRESS, 0, bg->GetStartTime(), bg->GetArenaType(), bg->isArena() ? 0 : 1);
    player->GetSession()->SendPacket(&status);
}

void ArenaWatcherEnd(Player* player, bool clear = false)
{
    if (!ArenaWatcherList.empty())
    {
        std::vector<uint32>::iterator iter = ArenaWatcherList.begin();
        while(iter != ArenaWatcherList.end())
        {
            if (player->GetGUIDLow() == *iter)
            {
                clear = true;
                iter = ArenaWatcherList.erase(iter);
            }
            else
                ++iter;
        }
    }
        
    if (!clear)
        return;

    player->ResurrectPlayer(100.0f, false);
    player->SetMovement(MOVE_LAND_WALK);
    player->SetMovement(MOVE_UNROOT);
    player->SetGMVisible(true);
    player->SetGameMaster(false);
    player->SetAcceptWhispers(true);

    player->SetSpeed(MOVE_WALK, 1.0f, true);
    player->SetSpeed(MOVE_RUN, 1.0f, true);
    player->SetSpeed(MOVE_SWIM, 1.0f, true);
    player->SetSpeed(MOVE_FLIGHT, 1.0f, true);
}

class npc_ArenaWatcher_WorldScript : public WorldScript
{
    public:
        npc_ArenaWatcher_WorldScript() : WorldScript("npc_ArenaWatcher_WorldScript") { }

    void OnConfigLoad(bool reload)
    {
        ArenaWatcherEnable = ConfigMgr::GetBoolDefault("ArenaWatcher.Enable", false);
        ArenaWatcherOnlyGM = ConfigMgr::GetBoolDefault("ArenaWatcher.OnlyGM", false);
        ArenaWatcherShowNoGames = ConfigMgr::GetBoolDefault("ArenaWatcher.ShowNoGames", false);
        ArenaWatcherOnlyRated = ConfigMgr::GetBoolDefault("ArenaWatcher.OnlyRated", false);
        ArenaWatcherToPlayers = ConfigMgr::GetBoolDefault("ArenaWatcher.ToPlayers", false);

        if (!reload)
            ArenaWatcherList.clear();
    }
};

class npc_ArenaWatcher_PlayerScript : public PlayerScript
{
    public:
        npc_ArenaWatcher_PlayerScript() : PlayerScript("npc_ArenaWatcher_PlayerScript") { }

    void OnPlayerRemoveFromBattleground(Player* player, Battleground* /*bg*/)
    {
        if (!ArenaWatcherEnable)
            return;

        ArenaWatcherEnd(player);
    }

    void OnLogout(Player* player)
    {
        if (!ArenaWatcherEnable)
            return;

        ArenaWatcherEnd(player);
    }
};

class npc_arena_watcher : public CreatureScript
{
    public:
        npc_arena_watcher() : CreatureScript("npc_arena_watcher") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (ArenaWatcherEnable && (!ArenaWatcherOnlyGM || player->isGameMaster()))
        {
            uint32 arenasCount[MAX_ARENA_SLOT] = {0, 0, 0};

            for (uint32 bgTypeId = 0; bgTypeId < MAX_BATTLEGROUND_TYPE_ID; ++bgTypeId)
            {
                if (!BattlegroundMgr::IsArenaType(BattlegroundTypeId(bgTypeId)))
                    continue;
                
                BattlegroundSet arenas = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BattlegroundTypeId(bgTypeId));

                if (arenas.empty())
                    continue;
                    
                for (BattlegroundSet::const_iterator itr = arenas.begin(); itr != arenas.end(); ++itr)
                {
                    Battleground* bg = itr->second;
                    if (!bg)
                        continue;
                        
                    if (bg->GetStatus() == STATUS_NONE || bg->GetStatus() == STATUS_WAIT_LEAVE)
                        continue;
                        
                    if (bg->GetArenaType() == 0)
                        continue;
                        
                    if (ArenaWatcherOnlyRated && !bg->isRated())
                        continue;

                    ++arenasCount[ArenaTeam::GetSlotByType(bg->GetArenaType())];
                }
            }

            std::string gossipText;
            
            for (uint8 i = 0; i < MAX_ARENA_SLOT; ++i)
            {
                // skip arena type with 0 games
                if (!ArenaWatcherShowNoGames && arenasCount[i] == 0)
                    continue;

                gossipText = fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(STRING_ARENA_2v2 + i), arenasCount[i]);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipText.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + ArenaTeam::GetTypeBySlot(i));
            }

            if (ArenaWatcherToPlayers)
            {
                gossipText = sObjectMgr->GetTrinityStringForDBCLocale(STRING_FOLLOW);
                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, gossipText.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "", 0, true);
            }
        }
        
        player->PlayerTalkClass->SendGossipMenu(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (!ArenaWatcherEnable && (!ArenaWatcherOnlyGM || player->isGameMaster()))
            return true;
        
        if (action <= GOSSIP_OFFSET)
        {
            bool bracketExists = false;

			uint8 playerCount = action - GOSSIP_ACTION_INFO_DEF;
            
            for (uint32 bgTypeId = 0; bgTypeId < MAX_BATTLEGROUND_TYPE_ID; ++bgTypeId)
            {
                if (!BattlegroundMgr::IsArenaType(BattlegroundTypeId(bgTypeId)))
                    continue;
                
                BattlegroundSet arenas = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BattlegroundTypeId(bgTypeId));

                if (arenas.empty())
                    continue;
                    
                for (BattlegroundSet::const_iterator itr = arenas.begin(); itr != arenas.end(); ++itr)
                {
                    Battleground* bg = itr->second;
                    if (!bg)
                        continue;
                        
                    if (bg->GetStatus() == STATUS_NONE || bg->GetStatus() == STATUS_WAIT_LEAVE)
                        continue;
                        
                    if (bg->GetArenaType() != playerCount)
                        continue;

                    if (ArenaWatcherOnlyRated && !bg->isRated())
                        continue;
                        
                    if (bg->isRated())
                    {
                        ArenaTeam* teamOne = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(0));
                        ArenaTeam* teamTwo = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(1));

                        if (teamOne && teamTwo)
                        {
                            std::stringstream gossipItem;
                            gossipItem << bg->GetBgMap()->GetMapName() << " : ";
                            gossipItem << teamOne->GetName() << " (";
                            gossipItem << teamOne->GetRating() << ") vs. ";
                            gossipItem << teamTwo->GetName() << " (";
                            gossipItem << teamTwo->GetRating() << ")";
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipItem.str(), GOSSIP_SENDER_MAIN + bgTypeId, itr->first + GOSSIP_OFFSET);
                        }
                    }
                    else
                    {
                        std::string gossipItem = fmtstring("[%u] %s : %u vs. %u", itr->first, bg->GetBgMap()->GetMapName(), playerCount, playerCount);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipItem.c_str(), GOSSIP_SENDER_MAIN + bgTypeId, itr->first + GOSSIP_OFFSET);
                    }

                    bracketExists = true;
                }
            }
            
            if (bracketExists)
                player->PlayerTalkClass->SendGossipMenu(player->GetGossipTextId(creature), creature->GetGUID());
            else
            {
                sCreatureTextMgr->SendChat(creature, SAY_NOT_FOUND_BRACKET, player->GetGUID());
                player->PlayerTalkClass->ClearMenus();
                player->CLOSE_GOSSIP_MENU();
            }
        }
        else
        {
            uint32 arenaId = action - GOSSIP_OFFSET;
            uint32 bgTypeId = sender - GOSSIP_SENDER_MAIN;
            BattlegroundSet arenas = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BattlegroundTypeId(bgTypeId));

            if (arenas[arenaId])
            {
                Battleground* bg = arenas[arenaId];

                if (bg->GetStatus() == STATUS_NONE)
                {
                    sCreatureTextMgr->SendChat(creature, SAY_ARENA_NOT_IN_PROGRESS, player->GetGUID());
                    player->PlayerTalkClass->ClearMenus();
                    player->CLOSE_GOSSIP_MENU();
                    return false;
                }

                player->SetBattlegroundId(bg->GetInstanceID(), bg->GetTypeID());
                player->SetBattlegroundEntryPoint();
                float x = 0.0f, y = 0.0f, z = 0.0f;
                switch (bg->GetMapId())
                {
                    case 617: x = 1299.046f;    y = 784.825f;     z = 9.338f;     break;
                    case 618: x = 763.5f;       y = -284;         z = 28.276f;    break;
                    case 572: x = 1285.810547f; y = 1667.896851f; z = 39.957642f; break;
                    case 562: x = 6238.930176f; y = 262.963470f;  z = 0.889519f;  break;
                    case 559: x = 4055.504395f; y = 2919.660645f; z = 13.611241f; break;
                }
                ArenaWatcherStart(player);
                player->TeleportTo(bg->GetMapId(), x, y, z, player->GetOrientation());
                ArenaWatcherAfterTeleport(player);
            }
        }
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction, const char* code)
    {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();

        if (!ArenaWatcherToPlayers || !ArenaWatcherEnable || (ArenaWatcherOnlyGM && !player->isGameMaster()))
            return true;
            
        if (uiSender == GOSSIP_SENDER_MAIN)
        {
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF + 4:
                {
                    const char* plrName = code;
                    if (Player* target = sObjectAccessor->FindPlayerByName(plrName))
                    {
                        if (!target->IsInWorld())
                        {
                            sCreatureTextMgr->SendChat(creature, SAY_TARGET_NOT_IN_WORLD, player->GetGUID());
                            return true;
                        }
                        else if (!target->InArena())
                        {
                            sCreatureTextMgr->SendChat(creature, SAY_TARGET_NOT_IN_ARENA, player->GetGUID());
                            return true;
                        }
                        else if (target->isGameMaster())
                        {
                            sCreatureTextMgr->SendChat(creature, SAY_TARGET_IS_GM, player->GetGUID());
                            return true;
                        }
                        else
                        {
                            player->SetBattlegroundId(target->GetBattleground()->GetInstanceID(), target->GetBattleground()->GetTypeID());
                            player->SetBattlegroundEntryPoint();
                            float x, y, z;
                            target->GetContactPoint(player, x, y, z);
                            ArenaWatcherStart(player);
                            player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target));
                            ArenaWatcherAfterTeleport(player);
                        }
                    }
                    return true;
                }
            }
        }

        return false;
    }
};

void AddSC_NPC_ArenaWatcher()
{
    new npc_ArenaWatcher_WorldScript();
    new npc_ArenaWatcher_PlayerScript();
    new npc_arena_watcher();
}