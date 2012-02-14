#include "ScriptPCH.h"
#include "Config.h"

bool KillBossEnable = true;

class Mod_KillBoss_WorldScript : public WorldScript
{
    public:
        Mod_KillBoss_WorldScript()
            : WorldScript("Mod_KillBoss_WorldScript")
        {
        }

    void SetInitialWorldSettings()
    {
        KillBossEnable = ConfigMgr::GetBoolDefault("KillBoss.Enable", true);

        if (!KillBossEnable)
            return;
    }
};

class Mod_KillBoss_PlayerScript : public PlayerScript
{
    public:
        Mod_KillBoss_PlayerScript()
            : PlayerScript("Mod_KillBoss_PlayerScript")
        {
        }

    void OnCreatureKill(Player* killer, Creature* killed)
    {
        if (!KillBossEnable)
			return;

        if (killed->GetCreatureInfo()->rank != CREATURE_ELITE_WORLDBOSS)
			return;

		sLog->outError("KILL BOSS!!!");
    }
};

void AddSC_Mod_KillBoss()
{
    new Mod_KillBoss_PlayerScript;
    new Mod_KillBoss_WorldScript;
}