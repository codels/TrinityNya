#include "SocialMgr.h"

#define LANG_FC_ON      15000
#define LANG_FC_OFF     15001

struct FriendChatInfo
{
    bool enable;
};

typedef std::map<uint32, FriendChatInfo> FriendChatMap;
FriendChatMap FriendChat;

class Mod_FriendChat_CommandScript : public CommandScript
{
public:
    Mod_FriendChat_CommandScript() : CommandScript("Mod_FriendChat_CommandScript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand commandTable[] =
        {
            { "friendchat",     SEC_PLAYER,         false, &HandleFriendChatCommand,           "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleFriendChatCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string param = (char*)args;

        uint32 guid = handler->GetSession()->GetPlayer()->GetGUID();

        if (param == "on")
        {
            FriendChat[guid].enable = true;
            handler->SendSysMessage(LANG_FC_ON);
            return true;
        }

        if (param == "off")
        {
            FriendChat[guid].enable = false;
            handler->SendSysMessage(LANG_FC_OFF);
            return true;
        }

        if (FriendChat[guid].enable)
            handler->SendSysMessage(LANG_FC_ON);
        else
            handler->SendSysMessage(LANG_FC_OFF);

        return true;
    }
};


class Mod_FriendChat_PlayerScript : public PlayerScript
{
    public:
        Mod_FriendChat_PlayerScript()
            : PlayerScript("Mod_FriendChat_PlayerScript")
        {
        }

    void OnLogin(Player* player)
    {
        uint32 guid = player->GetGUID();

        QueryResult rslt = CharacterDatabase.PQuery("SELECT `guid` FROM `character_friend_chat` WHERE `guid` = '%u'", guid);
        if (rslt)
            FriendChat[guid].enable = true;
        else
            FriendChat[guid].enable = false;
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Player* receiver)
    {
        if (!FriendChat[receiver->GetGUID()].enable)
            return;

        if (receiver->GetSocial()->HasFriend(player->GetGUID()))
            return;

        msg = "";
    }

    void OnLogout(Player* player)
    {
        uint32 guid = player->GetGUID();

        if (FriendChat[guid].enable)
            CharacterDatabase.PExecute("REPLACE INTO `character_friend_chat` (`guid`) VALUES ('%u')", guid);
        else
            CharacterDatabase.PExecute("DELETE FROM `character_friend_chat` WHERE `guid` = '%u'", guid);

        if (FriendChat.empty())
            return;

        FriendChatMap::iterator itr;

        for (itr = FriendChat.begin(); itr != FriendChat.end();)
        {
            if (itr->first == guid)
            {
                FriendChat.erase(itr);
                return;
            }
            else
                ++itr;
        }
    }
};

void AddSC_Mod_FriendChat()
{
    new Mod_FriendChat_CommandScript();
    new Mod_FriendChat_PlayerScript();
}
