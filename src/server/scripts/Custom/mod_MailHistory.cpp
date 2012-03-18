#include "ScriptPCH.h"
#include "Config.h"

#define SQL_HISTORY "INSERT INTO `world_mail` (`id`, `message_type`, `stationery`, `template_id`, `sender`, `receiver`, `subject`, `body`, `money`, `cod`) VALUES ('%u', '%u', '%u', '%u', '%u', '%u', '%s', '%s', '%u', '%u')"

bool mailHistoryEnable = false;

class Mod_MailHistory_WorldScript : public WorldScript
{
    public:
        Mod_MailHistory_WorldScript() : WorldScript("Mod_MailHistory_WorldScript") { }

    // Called after the world configuration is (re)loaded.
    void OnConfigLoad(bool /*reload*/)
    {
        mailHistoryEnable = ConfigMgr::GetBoolDefault("MailHistory.Enable", false);
    }
};

class Mod_MailHistory_MailScript : public MailScript
{
    public:
        Mod_MailHistory_MailScript() : MailScript("Mod_MailHistory_MailScript") { }

    void OnSendMail(MailDraft* const draft, MailReceiver const& receiver, MailSender const& sender, uint32 mailId, bool& /*needDelete*/)
    {
        if (!mailHistoryEnable)
            return;

        CharacterDatabase.PExecute(SQL_HISTORY,
            mailId,
            uint8(sender.GetMailMessageType()),
            int8(sender.GetStationery()),
            draft->GetMailTemplateId(),
            sender.GetSenderId(),
            receiver.GetPlayerGUIDLow(),
            draft->GetSubject().c_str(),
            draft->GetBody().c_str(),
            draft->GetMoney(),
            draft->GetCOD()
        );
    }
};

void AddSC_Mod_MailHistory()
{
    new Mod_MailHistory_WorldScript();
    new Mod_MailHistory_MailScript();
}