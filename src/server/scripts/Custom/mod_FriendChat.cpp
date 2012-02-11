#include "SocialMgr.h"

#define LANG_FC_ON      15000
#define LANG_FC_OFF     15001

struct FriendChatInfo
{
    bool enable;
};

static std::map<uint32, FriendChatInfo> FriendChat;

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

        if (param == "on")
        {
            FriendChat[handler->GetSession()->GetPlayer()->GetGUID()].enable = true;
            handler->SendSysMessage(LANG_FC_ON);
            return true;
        }

        if (param == "off")
        {
            FriendChat[handler->GetSession()->GetPlayer()->GetGUID()].enable = false;
            handler->SendSysMessage(LANG_FC_OFF);
            return true;
        }

        if (FriendChat[handler->GetSession()->GetPlayer()->GetGUID()].enable)
        {
            handler->SendSysMessage(LANG_FC_ON);
        } else {
            handler->SendSysMessage(LANG_FC_OFF);
        }

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
        FriendChat[player->GetGUID()].enable = false;
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver)
    {
        if (!FriendChat[receiver->GetGUID()].enable)
            return;

        if (receiver->GetSocial()->HasFriend(player->GetGUID()))
            return;

        msg = "";
    }
};

void AddSC_Mod_FriendChat()
{
    new Mod_FriendChat_CommandScript();
    new Mod_FriendChat_PlayerScript();
}
