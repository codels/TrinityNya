#include "CreatureTextMgr.h"
#include "Player.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ArenaTeamMgr.h"
#include "ArenaTeam.h"
#include "Config.h"

/*
enum EquipmentSlots                                         // 19 slots
{
    EQUIPMENT_SLOT_START        = 0,
    EQUIPMENT_SLOT_HEAD         = 0,
    EQUIPMENT_SLOT_NECK         = 1,
    EQUIPMENT_SLOT_SHOULDERS    = 2,
    EQUIPMENT_SLOT_BODY         = 3,
    EQUIPMENT_SLOT_CHEST        = 4,
    EQUIPMENT_SLOT_WAIST        = 5,
    EQUIPMENT_SLOT_LEGS         = 6,
    EQUIPMENT_SLOT_FEET         = 7,
    EQUIPMENT_SLOT_WRISTS       = 8,
    EQUIPMENT_SLOT_HANDS        = 9,
    EQUIPMENT_SLOT_FINGER1      = 10,
    EQUIPMENT_SLOT_FINGER2      = 11,
    EQUIPMENT_SLOT_TRINKET1     = 12,
    EQUIPMENT_SLOT_TRINKET2     = 13,
    EQUIPMENT_SLOT_BACK         = 14,
    EQUIPMENT_SLOT_MAINHAND     = 15,
    EQUIPMENT_SLOT_OFFHAND      = 16,
    EQUIPMENT_SLOT_RANGED       = 17,
    EQUIPMENT_SLOT_TABARD       = 18,
    EQUIPMENT_SLOT_END          = 19
};

    Item* item = NULL;
    item = GetUseableItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
    item = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

            case GOSSIP_ACTION_INFO_DEF + 1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE1,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE2,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE3,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE4,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                player->SEND_GOSSIP_MENU(7340, creature->GetGUID());
                break;

    // -1 from client enchantment slot number
    enum EnchantmentSlot
    {
        PERM_ENCHANTMENT_SLOT           = 0,
        TEMP_ENCHANTMENT_SLOT           = 1,
        SOCK_ENCHANTMENT_SLOT           = 2,
        SOCK_ENCHANTMENT_SLOT_2         = 3,
        SOCK_ENCHANTMENT_SLOT_3         = 4,
        BONUS_ENCHANTMENT_SLOT          = 5,
        PRISMATIC_ENCHANTMENT_SLOT      = 6,                    // added at apply special permanent enchantment
        MAX_INSPECTED_ENCHANTMENT_SLOT  = 7,

        PROP_ENCHANTMENT_SLOT_0         = 7,                    // used with RandomSuffix
        PROP_ENCHANTMENT_SLOT_1         = 8,                    // used with RandomSuffix
        PROP_ENCHANTMENT_SLOT_2         = 9,                    // used with RandomSuffix and RandomProperty
        PROP_ENCHANTMENT_SLOT_3         = 10,                   // used with RandomProperty
        PROP_ENCHANTMENT_SLOT_4         = 11,                   // used with RandomProperty
        MAX_ENCHANTMENT_SLOT            = 12
    };

            enchant = sSpellItemEnchantmentStore.LookupEntry(enchantId);
            if (enchant)
                break;
*/

struct ItemUpgradeTemplate
{
    uint32 enchantId;
    uint32 prevEnchantId;
    std::string description;
    uint32 charges;
    uint32 duration;
    uint32 golds;
};

std::vector<ItemUpgradeTemplate> ItemUpgradeInfo;

class Mod_ItemUpgrade_WorldScript : public WorldScript
{
    public:
        Mod_ItemUpgrade_WorldScript() : WorldScript("Mod_ItemUpgrade_WorldScript") { }

    void LoadDataFromDataBase()
    {
        ItemUpgradeInfo.clear();

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading ItemUpgrade...");
        uint32 oldMSTime = getMSTime();

        QueryResult result = WorldDatabase.PQuery("SELECT `enchant_id`, `prev_enchant_id`, `description`, `golds` FROM `world_item_upgrade`");

        if (!result)
            return;

        uint16 count = 0;

        do
        {
            Field* fields = result->Fetch();

            ItemUpgradeTemplate ItemUpgradeTemp;

            ItemUpgradeTemp.enchantId       = fields[0].GetUInt32();
            ItemUpgradeTemp.prevEnchantId   = fields[1].GetUInt32();
            ItemUpgradeTemp.description     = fields[2].GetString();
            ItemUpgradeTemp.golds           = fields[3].GetUInt32();
            ItemUpgradeTemp.charges         = 0;
            ItemUpgradeTemp.duration        = 0;

            ItemUpgradeInfo.push_back(ItemUpgradeTemp);
            ++count;
        }
        while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u count for ItemUpgrade in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    void OnConfigLoad(bool /*reload*/)
    {
        LoadDataFromDataBase();
    }

};

class go_item_upgrade : public GameObjectScript
{
    public:
        go_item_upgrade() : GameObjectScript("go_item_upgrade") { }

    uint16 getSlot(uint32 sender) {
        return (uint16) ((sender - GOSSIP_SENDER_MAIN) >> 16);
    }

    uint16 getEnchant(uint32 sender) {
        return (uint16) ((sender - GOSSIP_SENDER_MAIN) & 0xFFFF);
    }

    uint32 senderValue(uint16 slot, uint16 enchant) {
        return (uint32) (GOSSIP_SENDER_MAIN + ((slot << 16) | (enchant & 0xFFFF)));
    }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

        for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
        {
            Item* item = player->GetUseableItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (item)
            {
                ItemTemplate const *itemTemplate = item->GetTemplate();

                std::string Name = itemTemplate->Name1;
                if (loc_idx >= 0)
                    if (ItemLocale const* il = sObjectMgr->GetItemLocale(itemTemplate->ItemId))
                        ObjectMgr::GetLocaleString(il->Name, loc_idx, Name);

                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, Name.c_str(), senderValue(i, 0), GOSSIP_ACTION_INFO_DEF);
            }
        }
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        uint16 itemSlot = getSlot(sender);
        uint16 itemEnchantSlot = getEnchant(sender);

        if (itemSlot < EQUIPMENT_SLOT_START || itemSlot >= EQUIPMENT_SLOT_END) {
            player->CLOSE_GOSSIP_MENU();
            return false;
        }

        Item* item = player->GetUseableItemByPos(INVENTORY_SLOT_BAG_0, itemSlot);

        if (!item) {
            player->CLOSE_GOSSIP_MENU();
            return false;
        }

        if (itemEnchantSlot < PROP_ENCHANTMENT_SLOT_0 || itemEnchantSlot >= MAX_ENCHANTMENT_SLOT) {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Выбрать другой предмет", senderValue(0, 0), GOSSIP_ACTION_INFO_DEF);

            for (uint8 i = PROP_ENCHANTMENT_SLOT_0; i < MAX_ENCHANTMENT_SLOT; ++i) {
                std::string oldEffect = "эффекта нет или он не известен";
                uint32 enchantId = item->GetEnchantmentId(EnchantmentSlot(i));
                for (uint32 j = 0; j < ItemUpgradeInfo.size(); ++j)
                {
                    if (ItemUpgradeInfo[j].enchantId != enchantId)
                        continue;

                    oldEffect = ItemUpgradeInfo[j].description;
                }
                char gossipTextFormat[100];
                snprintf(gossipTextFormat, 100, "Эффект № %i (Сейчас: %s)", i - PROP_ENCHANTMENT_SLOT_0 + 1, oldEffect.c_str());
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipTextFormat, senderValue(itemSlot, i), GOSSIP_ACTION_INFO_DEF);
            }
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
            return true;
        }

        uint32 currentEnchantId = item->GetEnchantmentId(EnchantmentSlot(itemEnchantSlot));

        if (action <= GOSSIP_ACTION_INFO_DEF)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Выбрать другой слот для эффекта", senderValue(itemSlot, 0), GOSSIP_ACTION_INFO_DEF);

            if (currentEnchantId != 0)
                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Удалить эффект", senderValue(itemSlot, itemEnchantSlot), GOSSIP_ACTION_INFO_DEF + 1, "Вы уверены?", 1000000, 0);

            for (uint32 i = 0; i < ItemUpgradeInfo.size(); ++i)
            {
                if (ItemUpgradeInfo[i].prevEnchantId != currentEnchantId)
                    continue;

                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, ItemUpgradeInfo[i].description.c_str(), senderValue(itemSlot, itemEnchantSlot), GOSSIP_ACTION_INFO_DEF + ItemUpgradeInfo[i].enchantId, "Вы уверены?", ItemUpgradeInfo[i].golds * 10000, 0);
            }

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
            return true;
        }

        uint32 golds = 0;
        uint32 enchantId = action - GOSSIP_ACTION_INFO_DEF;

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        if (enchantId == 1)
        {
            golds = 100;
            if (!player->HasEnoughMoney(golds * 10000))
            {
                player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                player->CLOSE_GOSSIP_MENU();
                return false;
            }
            player->ApplyEnchantment(item, EnchantmentSlot(itemEnchantSlot), false);
            item->ClearEnchantment(EnchantmentSlot(itemEnchantSlot));
        } else {
            for (uint32 i = 0; i < ItemUpgradeInfo.size(); ++i)
            {
                if (ItemUpgradeInfo[i].prevEnchantId != currentEnchantId)
                    continue;

                if (ItemUpgradeInfo[i].enchantId != enchantId)
                    continue;

                golds = ItemUpgradeInfo[i].golds;

                if (!player->HasEnoughMoney(golds * 10000))
                {
                    player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                    player->CLOSE_GOSSIP_MENU();
                    return false;
                }
                player->ApplyEnchantment(item, EnchantmentSlot(itemEnchantSlot), false);
                item->SetEnchantment(EnchantmentSlot(itemEnchantSlot), ItemUpgradeInfo[i].enchantId, ItemUpgradeInfo[i].duration, ItemUpgradeInfo[i].charges);
                player->ApplyEnchantment(item, EnchantmentSlot(itemEnchantSlot), true);
            }
        }

        if (golds > 0)
        {
            player->ModifyMoney(-golds * 10000);
            player->SaveInventoryAndGoldToDB(trans);
        }
        item->SaveToDB(trans);
        CharacterDatabase.CommitTransaction(trans);
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

void AddSC_Mod_ItemUpgrade()
{
    new Mod_ItemUpgrade_WorldScript();
    new go_item_upgrade();
}