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

class npc_arena_watcher : public CreatureScript
{
    public:
        npc_arena_watcher() : CreatureScript("npc_arena_watcher") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        BattlegroundSet arenasSet = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BATTLEGROUND_AA);
        uint32 arenasQty[3] = {0, 0, 0};
        for (BattlegroundSet::const_iterator itr = arenasSet.begin(); itr != arenasSet.end(); ++itr)
            if (Battleground* bg = itr->second)
                switch (bg->GetArenaType())
                {
                    case ARENA_TYPE_2v2:  arenasQty[0]++; break;
                    case ARENA_TYPE_3v3:  arenasQty[1]++; break;
                    case ARENA_TYPE_5v5:  arenasQty[2]++; break;
                }

        std::string gossip2 = fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(STRING_ARENA_2v2), arenasQty[0]);
        std::string gossip3 = fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(STRING_ARENA_3v3), arenasQty[1]);
        std::string gossip5 = fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(STRING_ARENA_5v5), arenasQty[2]);
        std::string strFollow = sObjectMgr->GetTrinityStringForDBCLocale(STRING_FOLLOW);

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip2.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + ARENA_TYPE_2v2);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip3.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + ARENA_TYPE_3v3);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip5.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + ARENA_TYPE_5v5);
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, strFollow.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "", 0, true);

        player->PlayerTalkClass->SendGossipMenu(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        uint8 mode = ARENA_TYPE_2v2;
        if (action == (GOSSIP_ACTION_INFO_DEF + 3))
            mode = ARENA_TYPE_3v3;
        if (action == (GOSSIP_ACTION_INFO_DEF + 5))
            mode = ARENA_TYPE_5v5;

        if (action <= GOSSIP_OFFSET)
        {
            BattlegroundSet arenasSet = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BATTLEGROUND_AA);

            bool bracketMatchs = false;
            for (BattlegroundSet::const_iterator itr = arenasSet.begin(); itr != arenasSet.end(); ++itr)
            {
                if (Battleground* bg = itr->second)
                {
                    if (bg->GetArenaType() == mode)
                    {
                        bracketMatchs = true;
                        break;
                    }
                }
            }

            if (!bracketMatchs)
            {
                sCreatureTextMgr->SendChat(creature, SAY_NOT_FOUND_BRACKET, player->GetGUID());
                player->PlayerTalkClass->ClearMenus();
                player->CLOSE_GOSSIP_MENU();
            }
            else
            {
                for (BattlegroundSet::const_iterator itr = arenasSet.begin(); itr != arenasSet.end(); ++itr)
                {
                    if (Battleground* bg = itr->second)
                    {
                        ArenaTeam* teamOne = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(0));
                        ArenaTeam* teamTwo = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(1));

                        if (teamOne && teamTwo)
                        {
                            std::stringstream gossipItem;
                            gossipItem << teamOne->GetName() << " (";
                            gossipItem << teamOne->GetRating() << ") VS ";
                            gossipItem << teamTwo->GetName() << " (";
                            gossipItem << teamTwo->GetRating() << ")";
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipItem.str(), GOSSIP_SENDER_MAIN + 1, itr->first + GOSSIP_OFFSET);
                        }
                    }
                }
                player->PlayerTalkClass->SendGossipMenu(player->GetGossipTextId(creature), creature->GetGUID());
            }
        }
        else
        {
            uint32 arenaId = action - GOSSIP_OFFSET;
            BattlegroundSet arenasSet = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BATTLEGROUND_AA);

            if (arenasSet[arenaId] != NULL)
            {
                Battleground* arenaChosen = arenasSet[arenaId];

                if (arenaChosen->GetStatus() != STATUS_NONE && arenaChosen->GetStatus() != STATUS_IN_PROGRESS)
                {
                    sCreatureTextMgr->SendChat(creature, SAY_ARENA_NOT_IN_PROGRESS, player->GetGUID());
                    player->PlayerTalkClass->ClearMenus();
                    player->CLOSE_GOSSIP_MENU();
                    return false;
                }

                player->SetBattlegroundId(arenaChosen->GetInstanceID(), arenaChosen->GetTypeID());
                player->SetBattlegroundEntryPoint();
                float x = 0.0f, y = 0.0f, z = 0.0f;
                switch (arenaChosen->GetMapId())
                {
                    case 617: x = 1299.046f;    y = 784.825f;     z = 9.338f;     break;
                    case 618: x = 763.5f;       y = -284;         z = 28.276f;    break;
                    case 572: x = 1285.810547f; y = 1667.896851f; z = 39.957642f; break;
                    case 562: x = 6238.930176f; y = 262.963470f;  z = 0.889519f;  break;
                    case 559: x = 4055.504395f; y = 2919.660645f; z = 13.611241f; break;
                }
                player->SetGMVisible(false);
                player->TeleportTo(arenaChosen->GetMapId(), x, y, z, player->GetOrientation());
            }
        }
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction, const char* code)
    {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();
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
                            player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target));
                            player->SetGMVisible(false);
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
    new npc_arena_watcher();
}