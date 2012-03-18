#include "ScriptPCH.h"
#include "Config.h"
#include "AuctionHouseMgr.h"

#define AUCTIONEER_GUID 23442
#define AUCTION_TIME 172800
#define AUNCTION_FID 120

struct AHItemInfo
{
    AHItemInfo() : ItemId(0), ItemCount(0), CurrentCount(0), ItemStack(0), StartBind(0), BuyOut(0) { }
    uint32  ItemId;
    uint32  ItemCount;
    uint32  CurrentCount;
    uint32  ItemStack;
    uint32  StartBind;
    uint32  BuyOut;
};

bool AHEnable = false;
int AHInterval = 5;
uint32 AHPlayerGuid = 0;
uint32 AHAccountId = 0;
uint32 AHItemsPerCycle = 100;
uint32 AHItemCountCheck = 0;
IntervalTimer AuctionHouseTimer;
std::vector<AHItemInfo> AHItems;

Player* AHPlayer = NULL;
AuctionHouseEntry const* AHEntry = NULL;
AuctionHouseObject* AuctionHouse = NULL;

std::map<uint32, uint32> AHItemList;

void AHLoadFromDB()
{
    AHItems.clear();

    sLog->outString();
    sLog->outString("Loading AuctionBot...");
    uint32 oldMSTime = getMSTime();

    QueryResult resultAu = CharacterDatabase.PQuery("SELECT `i`.`itemEntry`, COUNT(*) FROM `auctionhouse` `a`, `item_instance` `i` WHERE `a`.`itemguid` = `i`.`guid` GROUP BY `i`.`itemEntry`");
    if (resultAu)
    {
        do
        {
            Field* fields = resultAu->Fetch();
            AHItemList[fields[0].GetUInt32()] = fields[1].GetUInt32();
        }
        while (resultAu->NextRow());
    }


    QueryResult result = WorldDatabase.PQuery("SELECT `ItemId`, `ItemCount`, `ItemStack`, `StartBind`, `BuyOut` FROM `world_auction`");

    if (!result)
        return;

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        AHItemInfo info;

        info.ItemId         = fields[0].GetUInt32();
        info.ItemCount      = fields[1].GetUInt32();
        info.ItemStack      = fields[2].GetUInt32();
        info.StartBind      = fields[3].GetUInt32();
        info.BuyOut         = fields[4].GetUInt32();
        info.CurrentCount   = AHItemList[info.ItemId];

        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(info.ItemId);
        if (!itemTemplate)
        {
            sLog->outError("MOD: AHBot item proto not found for item %u", info.ItemId);
            continue;
        }

        if (info.ItemStack > itemTemplate->GetMaxStackSize())
        {
            sLog->outError("MOD: AHBot item stack %u > max stack %u for item %u", info.ItemStack, itemTemplate->GetMaxStackSize(), info.ItemId);
            info.ItemStack = itemTemplate->GetMaxStackSize();
        }

        if (info.ItemStack == 0)
        {
            sLog->outError("MOD: AHBot item stack 0 for item %u", info.ItemId);
            continue;
        }

        AHItems.push_back(info);
        ++count;
    }
    while (result->NextRow());

    sLog->outString(">> Loaded %u items for AuctionBot in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void AHAddItem(AHItemInfo& info)
{
    //sLog->outError("MOD: AHAddItem() item %u count %u stack %u bind %u buy %u", info.ItemId, info.ItemCount, info.ItemStack, info.StartBind, info.BuyOut);
    Item* item = Item::CreateItem(info.ItemId, 1, AHPlayer);

    if (!item)
    {
        sLog->outError("MOD: AHAddItem() returned NULL");
        return;
    }

    item->AddToUpdateQueueOf(AHPlayer);

    uint32 randomPropertyId = Item::GenerateItemRandomPropertyId(info.ItemId);
    if (randomPropertyId != 0)
        item->SetItemRandomProperties(randomPropertyId);

    item->SetCount(info.ItemStack);
    uint32 deposit =  sAuctionMgr->GetAuctionDeposit(AHEntry, AUCTION_TIME, item, info.ItemStack);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    AuctionEntry* auctionEntry = new AuctionEntry;
    auctionEntry->Id = sObjectMgr->GenerateAuctionID();
    auctionEntry->auctioneer = AUCTIONEER_GUID;
    auctionEntry->item_guidlow = item->GetGUIDLow();
    auctionEntry->item_template = item->GetEntry();
    auctionEntry->owner = AHPlayerGuid;
    auctionEntry->startbid = info.StartBind;
    auctionEntry->buyout = info.BuyOut;
    auctionEntry->bidder = 0;
    auctionEntry->bid = 0;
    auctionEntry->deposit = deposit;
    auctionEntry->expire_time = (time_t) AUCTION_TIME + time(NULL);
    auctionEntry->auctionHouseEntry = AHEntry;
    item->SaveToDB(trans);
    item->RemoveFromUpdateQueueOf(AHPlayer);
    sAuctionMgr->AddAItem(item);
    AuctionHouse->AddAuction(auctionEntry);
    auctionEntry->SaveToDB(trans);
    CharacterDatabase.CommitTransaction(trans);

    return;
}

void AuctionHouseCheck()
{
    if (!AHEnable || !AHEntry || !AuctionHouse || !AHPlayer || AHItems.empty())
        return;

    AuctionHouseTimer.Reset();
    AHItemCountCheck = 0;

    for (uint32 i = 0; i < AHItems.size(); ++i)
        for (uint32 j = AHItems[i].CurrentCount; j < AHItems[i].ItemCount; j++)
        {
            AHAddItem(AHItems[i]);
            ++AHItemCountCheck;

            if (AHItemCountCheck >= AHItemsPerCycle)
                return;
        }
}

class Mod_AuctionHouseBot_AuctionHouseScript : public AuctionHouseScript
{
    public:
        Mod_AuctionHouseBot_AuctionHouseScript() : AuctionHouseScript("Mod_AuctionHouseBot_AuctionHouseScript") { }

        // Called when an auction is added to an auction house.
        void OnAuctionAdd(AuctionHouseObject* /*ah*/, AuctionEntry* entry)
        {
            if (!AHEnable || AHItems.empty())
                return;

            for (uint32 i = 0; i < AHItems.size(); ++i)
                if (AHItems[i].ItemId == entry->item_template)
                {
                    ++AHItems[i].CurrentCount;
                    return;
                }
        }

        // Called when an auction is removed from an auction house.
        void OnAuctionRemove(AuctionHouseObject* /*ah*/, AuctionEntry* entry)
        {
            if (!AHEnable || AHItems.empty())
                return;

            for (uint32 i = 0; i < AHItems.size(); ++i)
                if (AHItems[i].ItemId == entry->item_template)
                {
                    if (AHItems[i].CurrentCount > 0)
                        --AHItems[i].CurrentCount;
                    return;
                }
        }
};

class Mod_AuctionHouseBot_WorldScript : public WorldScript
{
    public:
        Mod_AuctionHouseBot_WorldScript() : WorldScript("Mod_AuctionHouseBot_WorldScript") { }

    // Called after the world configuration is (re)loaded.
    void OnConfigLoad(bool reload)
    {
        AHEnable        = ConfigMgr::GetBoolDefault("AuctionHouseBot.Enable", false);
        if (!AHEnable)
            return;

        AHInterval      = ConfigMgr::GetIntDefault("AuctionHouseBot.Interval", 5);
        AHPlayerGuid    = ConfigMgr::GetIntDefault("AuctionHouseBot.PlayerGuid", 0);
        AHAccountId     = ConfigMgr::GetIntDefault("AuctionHouseBot.AccountId", 0);
        AHItemsPerCycle = ConfigMgr::GetIntDefault("AuctionHouseBot.ItemsPerCycle", 100);

        AuctionHouseTimer.SetInterval(AHInterval * MINUTE * IN_MILLISECONDS);
        AuctionHouseTimer.Reset();

        if (!AHEntry)
        {
            AHEntry = sAuctionMgr->GetAuctionHouseEntry(AUNCTION_FID);
            if (!AHEntry)
            {
                AHEnable = false;
                return;
            }
        }

        if (!AuctionHouse)
        {
            AuctionHouse =  sAuctionMgr->GetAuctionsMap(AUNCTION_FID);
            if (!AuctionHouse)
            {
                AHEnable = false;
                return;
            }
        }

        if (AHPlayerGuid != 0 && AHAccountId != 0)
        {
            if (!AHPlayer)
            {
                WorldSession _session(AHAccountId, NULL, SEC_PLAYER, 0, 0, LOCALE_enUS, 0, false);
                Player NewPlayer(&_session);
                NewPlayer.Initialize(AHPlayerGuid);
                sObjectAccessor->AddObject(&NewPlayer);
                AHPlayer = &NewPlayer;
                if (!AHPlayer)
                {
                    AHEnable = false;
                    return;
                }
                //sLog->outError("AHPlayer " UI64FMTD " %u %s", AHPlayer->GetGUID(), AHPlayer->GetGUIDLow(), AHPlayer->GetName());
            }
        }
        else
        {
            AHEnable = false;
            return;
        }

        if (!reload)
            AHLoadFromDB();

        if (AHItems.empty())
        {
            AHEnable = false;
            return;
        }

        AuctionHouseCheck();
    }

    void OnUpdate(uint32 diff)
    {
        if (!AHEnable)
            return;

        if (AuctionHouseTimer.GetCurrent() >= 0)
            AuctionHouseTimer.Update(diff);
        else
            AuctionHouseTimer.SetCurrent(0);

        if (AuctionHouseTimer.Passed())
            AuctionHouseCheck();
    }

    void OnSendMail(MailReceiver const& receiver, MailSender const& /*sender*/, bool& needDelete)
    {
        if (!needDelete && receiver.GetPlayerGUIDLow() == AHPlayerGuid)
            needDelete = true;
    }
};

void AddSC_Mod_AuctionHouseBot()
{
    new Mod_AuctionHouseBot_WorldScript();
    new Mod_AuctionHouseBot_AuctionHouseScript();
}