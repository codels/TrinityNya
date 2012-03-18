#include "SocialMgr.h"
#include "Config.h"

#define LANG_FC_ON      15000
#define LANG_FC_OFF     15001

struct FriendChatInfo
{
    FriendChatInfo() : enable(false) {}
    bool enable;
};

bool friendChatEnable = false;
typedef std::map<uint32, FriendChatInfo> FriendChatMap;
FriendChatMap FriendChat;

class Mod_FriendChat_WorldScript : public WorldScript
{
    public:
        Mod_FriendChat_WorldScript() : WorldScript("Mod_FriendChat_WorldScript") { }

    // Called after the world configuration is (re)loaded.
    void OnConfigLoad(bool /*reload*/)
    {
        friendChatEnable = ConfigMgr::GetBoolDefault("FriendChat", false);
    }
};

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
        if (!friendChatEnable || !*args)
            return false;

        std::string param = (char*)args;

        if (param == "on")
        {
            FriendChat[handler->GetSession()->GetPlayer()->GetGUID()].enable = true;
            handler->SendSysMessage(LANG_FC_ON);
            return true;
        }
        else if (param == "off")
        {
            FriendChat[handler->GetSession()->GetPlayer()->GetGUID()].enable = false;
            handler->SendSysMessage(LANG_FC_OFF);
            return true;
        }
        else
        {
            if (FriendChat[handler->GetSession()->GetPlayer()->GetGUID()].enable)
                handler->SendSysMessage(LANG_FC_ON);
            else
                handler->SendSysMessage(LANG_FC_OFF);

            return true;
        }
    }
};


class Mod_FriendChat_PlayerScript : public PlayerScript
{
    public:
        Mod_FriendChat_PlayerScript() : PlayerScript("Mod_FriendChat_PlayerScript") { }

    // Called when a player logs in.
    void OnLogin(Player* player)
    {
        if (!friendChatEnable)
            return;

        if (QueryResult result = CharacterDatabase.PQuery("SELECT `guid` FROM `character_friend_chat` WHERE `guid` = '%u'", player->GetGUID()))
            FriendChat[player->GetGUID()].enable = true;
        else
            FriendChat[player->GetGUID()].enable = false;
    }

    // The following methods are called when a player sends a chat message.
    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Player* receiver)
    {
        if (!friendChatEnable || !receiver || !FriendChat[receiver->GetGUID()].enable || receiver->GetSocial()->HasFriend(player->GetGUID()))
            return;

        msg = "";
    }

    // Called when a player logs out.
    void OnLogout(Player* player)
    {
        if (!friendChatEnable)
            return;

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
    new Mod_FriendChat_WorldScript();
    new Mod_FriendChat_CommandScript();
    new Mod_FriendChat_PlayerScript();
}
