#include "ScriptPCH.h"
#include "Config.h"
#include "Battleground.h"

#define GOLD 10000
#define MIN_KILLS 2
#define MAX_KILLS 9


bool multikill_enable = true;
bool only_bg = true;
bool log_db = true;
int time_between_kills = 15;

uint32  reward_gold_count   =   50;
uint32  reward_gold_kills   =   2;
bool    reward_gold_scale   =   true;

uint32  reward_buff_one_id      =   2379;
uint32  reward_buff_one_kills   =   2;

uint32  reward_buff_two_id      =   23505;
uint32  reward_buff_two_kills   =   3;

uint32  reward_sound_id     =   14808;
uint32  reward_sound_kills  =   6;

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
        multikill_enable = ConfigMgr::GetBoolDefault("MultiKill.Enable", true);

        if (!multikill_enable)
            return;

        only_bg = ConfigMgr::GetBoolDefault("MultiKill.BattleGroundOnly", true);
        log_db = ConfigMgr::GetBoolDefault("MultiKill.Log", true);
        time_between_kills = ConfigMgr::GetIntDefault("MultiKill.Time.Between", 15);

        reward_gold_count = ConfigMgr::GetIntDefault("MultiKill.Reward.Gold.Count", 50);
        reward_gold_kills = ConfigMgr::GetIntDefault("MultiKill.Reward.Gold.Kills", 2);
        reward_gold_scale = ConfigMgr::GetBoolDefault("MultiKill.Reward.Gold.Scale", true);

        reward_buff_one_id = ConfigMgr::GetIntDefault("MultiKill.Reward.Buff.One.Spell", 2379);
        reward_buff_one_kills = ConfigMgr::GetIntDefault("MultiKill.Reward.Buff.One.Kills", 2);

        reward_buff_two_id = ConfigMgr::GetIntDefault("MultiKill.Reward.Buff.Two.Spell", 23505);
        reward_buff_two_kills = ConfigMgr::GetIntDefault("MultiKill.Reward.Buff.Two.Kills", 3);
        
        reward_sound_id = ConfigMgr::GetIntDefault("MultiKill.Reward.Sound.Id", 14808);
        reward_sound_kills = ConfigMgr::GetIntDefault("MultiKill.Reward.Sound.Kills", 6);
    }
};

class Mod_MultiKill_PlayerScript : public PlayerScript
{
    public:
        Mod_MultiKill_PlayerScript()
            : PlayerScript("Mod_MultiKill_PlayerScript")
        {
        }

    void RewardSound(Player *player)
    {
        if (reward_sound_id == 0)
            return;

        if (MultiKill[player->GetGUID()].count < reward_sound_kills)
            return;

		if (player->InBattleground())
		{
			if (Battleground* bg = player->GetBattleground())
				bg->PlaySoundToAll(reward_sound_id);
		}
    }

    void RewardBuff(uint32 buff_id, uint32 buff_kills, Player *player)
    {
        if (buff_id == 0)
            return;

        if (MultiKill[player->GetGUID()].count < buff_kills)
            return;

        player->CastSpell(player, buff_id, true);
    }

    void RewardGold(Player *player)
    {
        if (reward_gold_count == 0)
            return;

        if (MultiKill[player->GetGUID()].count < reward_gold_kills)
            return;

        uint32 gold = reward_gold_count;
        if (reward_gold_scale)
            gold *= MultiKill[player->GetGUID()].count - 1;

        player->ModifyMoney(gold * GOLD);

		if (player->InBattleground())
		{
			if (Battleground* bg = player->GetBattleground())
				bg->SendWarningToAll(LANG_MULTIKILL_GOLD, player->GetName(), gold);
		}
    }

    void Announce(Player *player)
    {
		if (player->InBattleground())
		{
			if (Battleground* bg = player->GetBattleground())
			{	bg->SendWarningToAll(
					LANG_MULTIKILL_KILL,
					player->GetName(),
					sObjectMgr->GetTrinityStringForDBCLocale(LANG_MULTIKILL_START + MultiKill[player->GetGUID()].count - 2)
				);
			}
		}

        if (!log_db)
            return;

        CharacterDatabase.PExecute(
            "INSERT INTO `character_multikill` (`guid`, `count`, `time`) VALUES ('%u', '%u', CURRENT_TIMESTAMP())",
                player->GetGUID(),
                MultiKill[player->GetGUID()].count
        );
    }
        
    void OnPVPKill(Player *killer, Player *killed)
    {
        if (!multikill_enable)
            return;

        if (killer->GetGUID() == killed->GetGUID())
            return;

        if (only_bg && !killer->InBattleground())
            return;

        time_t now = time(NULL);
        uint32 guid = killer->GetGUID();

        if (now - MultiKill[guid].last > time_between_kills)
            MultiKill[guid].count = 0;

        MultiKill[guid].last = now;

        if (MultiKill[guid].count < MAX_KILLS)
            MultiKill[guid].count++;

        if (MultiKill[guid].count < MIN_KILLS)
            return;

        RewardBuff(reward_buff_one_id, reward_buff_one_kills, killer);
		RewardBuff(reward_buff_two_id, reward_buff_two_kills, killer);
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