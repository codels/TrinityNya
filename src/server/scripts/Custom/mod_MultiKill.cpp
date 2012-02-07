#include "ScriptPCH.h"
#include "Config.h"
#include "Battleground.h"

#define GOLD 10000
#define MIN_KILLS 2
#define MAX_KILLS 9


bool    MultiKillEnable                 = true;
bool    MultiKillBattleGround           = true;
bool    MultiKillLog                    = true;
int     MultiKillTimeBetween            = 15;

uint32  MultiKillRewardGold             = 50;
uint32  MultiKillRewardGoldKills        = 2;
bool    MultiKillRewardGoldScale        = true;

uint32  MultiKillRewardBuffFirst        = 2379;
uint32  MiltuKillRewardBuffFirstKills   = 2;

uint32  MultiKillRewardBuffSecond       = 23505;
uint32  MultiKillRewardBuffSecondKills  = 3;

uint32  MultiKillRewardSound            = 14808;
uint32  MultiKillRewardSoundKills       = 6;

struct MultiKillInfo
{
    time_t last;
    uint8 count;
};

static std::map<uint32, MultiKillInfo> MultiKill;

class Mod_MultiKill_WorldScript : public WorldScript
{
    public:
        Mod_MultiKill_WorldScript()
            : WorldScript("Mod_MultiKill_WorldScript")
        {
        }

    void SetInitialWorldSettings()
    {
        MultiKillEnable = ConfigMgr::GetBoolDefault("MultiKill.Enable", true);

        if (!MultiKillEnable)
            return;

        MultiKillBattleGround = ConfigMgr::GetBoolDefault("MultiKill.BattleGroundOnly", true);
        MultiKillLog = ConfigMgr::GetBoolDefault("MultiKill.Log", true);
        MultiKillTimeBetween = ConfigMgr::GetIntDefault("MultiKill.Time.Between", 15);

        MultiKillRewardGold = ConfigMgr::GetIntDefault("MultiKill.Reward.Gold.Count", 50);
        MultiKillRewardGoldKills = ConfigMgr::GetIntDefault("MultiKill.Reward.Gold.Kills", 2);
        MultiKillRewardGoldScale = ConfigMgr::GetBoolDefault("MultiKill.Reward.Gold.Scale", true);

        MultiKillRewardBuffFirst = ConfigMgr::GetIntDefault("MultiKill.Reward.Buff.One.Spell", 2379);
        MiltuKillRewardBuffFirstKills = ConfigMgr::GetIntDefault("MultiKill.Reward.Buff.One.Kills", 2);

        MultiKillRewardBuffSecond = ConfigMgr::GetIntDefault("MultiKill.Reward.Buff.Two.Spell", 23505);
        MultiKillRewardBuffSecondKills = ConfigMgr::GetIntDefault("MultiKill.Reward.Buff.Two.Kills", 3);
        
        MultiKillRewardSound = ConfigMgr::GetIntDefault("MultiKill.Reward.Sound.Id", 14808);
        MultiKillRewardSoundKills = ConfigMgr::GetIntDefault("MultiKill.Reward.Sound.Kills", 6);
    }
};

class Mod_MultiKill_PlayerScript : public PlayerScript
{
    public:
        Mod_MultiKill_PlayerScript()
            : PlayerScript("Mod_MultiKill_PlayerScript")
        {
        }

    void RewardSound(Player* player)
    {
        if (MultiKillRewardSound == 0)
            return;

        if (MultiKill[player->GetGUID()].count < MultiKillRewardSoundKills)
            return;

        if (player->InBattleground())
        {
            if (Battleground* bg = player->GetBattleground())
                bg->PlaySoundToAll(MultiKillRewardSound);
        }
    }

    void RewardBuff(uint32 spell, uint32 kills, Player* player)
    {
        if (spell == 0)
            return;

        if (MultiKill[player->GetGUID()].count < kills)
            return;

        player->CastSpell(player, spell, true);
    }

    void RewardGold(Player* player)
    {
        if (MultiKillRewardGold == 0)
            return;

        if (MultiKill[player->GetGUID()].count < MultiKillRewardGoldKills)
            return;

        uint32 gold = MultiKillRewardGold;
        if (MultiKillRewardGoldScale)
            gold *= MultiKill[player->GetGUID()].count - 1;

        player->ModifyMoney(gold * GOLD);

        if (player->InBattleground())
        {
            if (Battleground* bg = player->GetBattleground())
                bg->SendWarningToAll(LANG_MULTIKILL_GOLD, player->GetName(), gold);
        }
    }

    void Announce(Player* player)
    {
        if (player->InBattleground())
        {
            if (Battleground* bg = player->GetBattleground())
            {   bg->SendWarningToAll(
                    LANG_MULTIKILL_KILL,
                    player->GetName(),
                    sObjectMgr->GetTrinityStringForDBCLocale(LANG_MULTIKILL_START + MultiKill[player->GetGUID()].count - 2)
                );
            }
        }

        if (!MultiKillLog)
            return;

        CharacterDatabase.PExecute(
            "INSERT INTO `character_multikill` (`guid`, `count`, `time`) VALUES ('%u', '%u', CURRENT_TIMESTAMP())",
                player->GetGUID(),
                MultiKill[player->GetGUID()].count
        );
    }
        
    void OnPVPKill(Player* killer, Player* killed)
    {
        if (!MultiKillEnable)
            return;

        if (killer->GetGUID() == killed->GetGUID())
            return;

        if (MultiKillBattleGround && !killer->InBattleground())
            return;

        time_t now = time(NULL);
        uint32 guid = killer->GetGUID();

        if (now - MultiKill[guid].last > MultiKillTimeBetween)
            MultiKill[guid].count = 0;

        MultiKill[guid].last = now;

        if (MultiKill[guid].count < MAX_KILLS)
            MultiKill[guid].count++;

        if (MultiKill[guid].count < MIN_KILLS)
            return;

        RewardBuff(MultiKillRewardBuffFirst, MiltuKillRewardBuffFirstKills, killer);
        RewardBuff(MultiKillRewardBuffSecond, MultiKillRewardBuffSecondKills, killer);
        Announce(killer);
        RewardGold(killer);
        RewardSound(killer);
    }
};

void AddSC_Mod_Multikill()
{
    new Mod_MultiKill_PlayerScript;
    new Mod_MultiKill_WorldScript;
}