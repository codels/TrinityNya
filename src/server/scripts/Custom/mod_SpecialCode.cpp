#include "ScriptPCH.h"
#include "Config.h"

#define SQL_CODE "SELECT `CodeId`, `MailMoney`, `MaxCountForCharacter`, `MaxCountForAccount`, `MaxCountTotal`, `MailSubject`, `MailText` FROM `world_coded` WHERE `CodeStart` >= CURRENT_TIMESTAMP AND `CodeEnd` <= CURRENT_TIMESTAMP AND `CodeData` = '%s' LIMIT 1"
#define SQL_CODE_COUNT "SELECT COUNT(*) FROM `world_coded_history` WHERE `CodeId` = '%u'"
#define SQL_CODE_COUNT_ACCOUNT "SELECT COUNT(*) FROM `world_coded_history` WHERE `CodeId` = '%u' AND `AccountId` = '%u'"
#define SQL_CODE_COUNT_CHARACTER "SELECT COUNT(*) FROM `world_coded_history` WHERE `CodeId` = '%u' AND `CharacterGuid` = '%u'"
#define SQL_CODE_HISTORY "INSERT INTO `world_coded_history` (`CodeId`, `CharacterGuid`, `AccountId`) VALUES ('%u', '%u', '%u')"
#define SQL_CODE_ITEMS "SELECT `ItemId`, `ItemCount` FROM `world_coded_items` WHERE `CodeId` = '%u' AND (`ItemClassMask` & %u OR `ItemClassMask` = 0) AND (`ItemRaceMask` & %u OR `ItemRaceMask` = 0)"

bool SCEnable = false;

class Mod_SpecialCode_WorldScript : public WorldScript
{
    public:
        Mod_SpecialCode_WorldScript() : WorldScript("Mod_SpecialCode_WorldScript") { }

    void OnConfigLoad(bool /*reload*/)
    {
        SCEnable = ConfigMgr::GetBoolDefault("SpecialCode.Enable", false);
    }
};

class Mod_SpecialCode_AllCreatureScript : public AllCreatureScript
{
    public:
        Mod_SpecialCode_AllCreatureScript() : AllCreatureScript("Mod_SpecialCode_AllCreatureScript") { }

    void AllCreatureCode(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/, const char* code)
    {
        if (!SCEnable)
            return;

        QueryResult result = CharacterDatabase.PQuery(SQL_CODE, code);
        if (!result)
            return;

        uint32 codeId = (*result)[0].GetUInt32();
        std::string subject = fields[5].GetString();
        std::string text = fields[6].GetString();
        int32 mailMoney = (*result)[1].GetInt32();
        uint8 maxCharacters = (*result)[2].GetUInt8();
        uint8 maxAccount = (*result)[3].GetUInt8();
        uint8 maxTotal = (*result)[4].GetUInt8();
        uint16 currentTotal = 0;
        uint16 currentAccount = 0;
        uint16 currentCharacter = 0;

        if (maxTotal > 0)
        {
            result = CharacterDatabase.PQuery(SQL_CODE_COUNT, codeId);
            if (result)
                currentTotal = (*result)[0].GetUInt16();

            if (currentTotal >= maxTotal)
                return;
        }

        uint32 accountId = player->GetSession()->GetAccountId();

        if (maxAccount > 0)
        {
            result = CharacterDatabase.PQuery(SQL_CODE_COUNT_ACCOUNT, codeId, accountId);
            if (result)
                currentAccount = (*result)[0].GetUInt16();

            if (currentAccount >= maxAccount)
                return;
        }

        uint32 guid = player->GetGUID();

        if (maxCharacters > 0)
        {
            result = CharacterDatabase.PQuery(SQL_CODE_COUNT_CHARACTER, codeId, guid);
            if (result)
                currentCharacter = (*result)[0].GetUInt16();

            if (currentCharacter >= maxCharacters)
                return;
        }

        CharacterDatabase.PExecute(SQL_CODE_HISTORY, codeId, guid, accountId);

        QueryResult result = CharacterDatabase.PQuery(SQL_CODE_ITEMS);

        if (!result)
            return;

        ItemPairs items;

        do
        {
            Field* fields = result->Fetch();

            itemId = fields[0].GetUInt32();
            itemCount = fields[1].GetUInt16();

            ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemId);
            if (!item_proto)
                continue;

            if (itemCount < 1 || (itemProto->MaxCount > 0 && itemCount > uint32(itemProto->MaxCount)))
                continue;

            while (itemCount > itemProto->GetMaxStackSize())
            {
                items.push_back(ItemPair(itemId, itemProto->GetMaxStackSize()));
                itemCount -= itemProto->GetMaxStackSize();
            }

            items.push_back(ItemPair(itemId, itemCount));

            if (items.size() > MAX_MAIL_ITEMS)
                return;
        }
        while (result->NextRow());

        MailSender sender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);
        MailDraft draft(subject, text);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        for (ItemPairs::const_iterator itr = items.begin(); itr != items.end(); ++itr)
        {
            if (Item* item = Item::CreateItem(itr->first, itr->second, 0))
            {
                item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted
                draft.AddItem(item);
            }
        }

        draft.SendMailTo(trans, MailReceiver(player, GUID_LOPART(guid)), sender);
        CharacterDatabase.CommitTransaction(trans);
    }

};

void AddSC_Mod_SpecialCode()
{
    new Mod_SpecialCode_WorldScript();
    new Mod_SpecialCode_AllCreatureScript();
}