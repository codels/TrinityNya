#include "ScriptPCH.h"
#include "Config.h"

#define SQL_HISTORY "INSERT INTO account_history (AccountId, RealmId, SessionIP, HistoryType, CharacterGuid, CharacterName) VALUES ('%d', '%u', '%s', '%u', '%u', '%s')"

enum AccountHistoryType
{
    ACCOUNT_HISTORY_UNKNOWN = 0,
    ACCOUNT_HISTORY_LOGIN   = 1,
    ACCOUNT_HISTORY_LOGOUT  = 2,
    ACCOUNT_HISTORY_DELETE  = 3,
    ACCOUNT_HISTORY_CREATE  = 4
};

bool AccountHistoryEnable = false;
bool AccountHistoryLogin = false;
bool AccountHistoryLogout = false;
bool AccountHistoryCreate = false;
bool AccountHistoryDelete = false;

void WriteToHistory(Player* player, uint8 historyType)
{
    WorldSession* session = player->GetSession();
    LoginDatabase.PExecute(SQL_HISTORY, session->GetAccountId(), realmID, session->GetRemoteAddress().c_str(), historyType, player->GetGUIDLow(), player->GetName());
}

class Mod_AccountHistory_WorldScript : public WorldScript
{
    public:
        Mod_AccountHistory_WorldScript() : WorldScript("Mod_AccountHistory_WorldScript") { }

    void OnConfigLoad(bool /*reload*/)
    {
        AccountHistoryEnable    = ConfigMgr::GetBoolDefault("AccountHistory.Enable", false);
        AccountHistoryLogin     = ConfigMgr::GetBoolDefault("AccountHistory.Login", false);
        AccountHistoryLogout    = ConfigMgr::GetBoolDefault("AccountHistory.Logout", false);
        AccountHistoryCreate    = ConfigMgr::GetBoolDefault("AccountHistory.Create", false);
        AccountHistoryDelete    = ConfigMgr::GetBoolDefault("AccountHistory.Delete", false);
    }
};

class Mod_AccountHistory_PlayerScript : public PlayerScript
{
    public:
        Mod_AccountHistory_PlayerScript() : PlayerScript("Mod_AccountHistory_PlayerScript") { }

    // Called when a player is created.
    void OnCreate(Player* player)
    {
        if (!AccountHistoryEnable || !AccountHistoryCreate)
            return;

        WriteToHistory(player, ACCOUNT_HISTORY_CREATE);
    }

    // Called when a player logs in.
    void OnLogin(Player* player)
    {
        if (!AccountHistoryEnable || !AccountHistoryLogin)
            return;

        WriteToHistory(player, ACCOUNT_HISTORY_LOGIN);
    }

    // Called when a player logs out.
    void OnLogout(Player* player)
    {
        if (!AccountHistoryEnable || !AccountHistoryLogout)
            return;

        WriteToHistory(player, ACCOUNT_HISTORY_LOGOUT);
    }

    // Called when a player is deleted.
    void OnDelete(uint64 guid, const char* name, WorldSession* session)
    {
        if (!AccountHistoryEnable || !AccountHistoryDelete)
            return;

        LoginDatabase.PExecute(SQL_HISTORY, session->GetAccountId(), realmID, session->GetRemoteAddress().c_str(), ACCOUNT_HISTORY_DELETE, GUID_LOPART(guid), name);
    }
};

void AddSC_Mod_AccountHistory()
{
    new Mod_AccountHistory_WorldScript();
    new Mod_AccountHistory_PlayerScript();
}