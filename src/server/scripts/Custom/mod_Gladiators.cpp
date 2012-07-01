/*
** Experimental!!!
*/

#include "ScriptPCH.h"
#include "Config.h"
#include "MapManager.h"
#include "CreatureTextMgr.h"
#include "SmartScriptMgr.h"

#define SQL_TEMPLATE "SELECT `level`, `time_limit`, `say_start`, `say_win`, `say_lose`, `chest_id`, `point_id`, `req_quest_id`, `kill_credit`, `menu_string` FROM `_gladiator_template` ORDER BY `level` DESC"
#define SQL_CREATURE "SELECT `id`, `level`, `entry`, `count`, `from_point_id`, `to_point_id`, `time` FROM `_gladiator_creature` ORDER BY `level`, `time`"
#define SQL_POINTS "SELECT `id`, `map_id`, `x`, `y`, `z`, `o` FROM `_gladiator_points`"

struct GladPoint
{
    uint32 id;
    uint32 map;
    float x;
    float y;
    float z;
    float o;
};

struct GladCreature
{
    uint32 id;
    uint32 level;
    uint32 entry;
    uint32 count;
    uint32 point;
    uint32 move;
    uint32 time;
    bool spawn;
};

typedef std::list<GladCreature*> GladCreatureList;

struct GladTemplate
{
    uint32 level;
    uint32 timeLimit;
    uint32 time;
    uint32 sayStart; //string_id
    uint32 sayWin; //string_id
    uint32 sayLose; //string_id
    uint32 chest;
    uint32 point;
    uint32 reqQuest;
    uint32 killCredit;
    uint32 menuString;
    GladCreatureList creatures;
};

typedef std::map<uint32, GladPoint const*> GladPointMap;
typedef std::map<uint32, GladTemplate*> GladTemplates;
typedef std::list<Creature*> CreatureList;

bool GladEnable = false;
bool GladDebug = false;
bool GladLadder = false;
int  GladLadderText = 0;

class Gladiators
{
    public:
        Gladiators() { _inProgress = false; _level = 0; _loaded = false; }
        ~Gladiators() { clear(); }
        
        void load(bool reload = false);
        
        void start(uint32 level, Player* player = NULL);
        void stop(bool win = false);
        bool checkEnd();
        void addGossip(Player* player);
        
        void update(uint32 diff);

        void clear();
        void reset();
        
        bool pointExists(uint32 id);
        bool templateExists(uint32 level);
        bool existsAlive();
        
        GladPoint const* getPoint(uint32 id);
        
        bool isProgress() { return _inProgress; }
        uint32 getLevel() { return _level; }
        
        void setOrganizer(Creature* npc) { _organizer = npc; }

    private:
        GladTemplates _templates;
        GladPointMap _points;
        uint32 _level;
        bool _inProgress;
        GladTemplate* _currentLevel;
        CreatureList _creatureList;
        Creature* _organizer;
        bool _loaded;
        Player* _playerStart;
};


GladPoint const* Gladiators::getPoint(uint32 id)
{
    if (!pointExists(id))
        return NULL;
        
    return _points[id];
}

bool Gladiators::pointExists(uint32 id)
{
    GladPointMap::const_iterator itr = _points.find(id);
    if (itr != _points.end())
        return true;
    return false;
}

bool Gladiators::templateExists(uint32 level)
{
    GladTemplates::const_iterator itr = _templates.find(level);
    if (itr != _templates.end())
        return true;
    return false;
}

void Gladiators::load(bool reload)
{
    if (_loaded && !reload)
        return;
        
    clear();
    
    sLog->outString();
    sLog->outString("Loading Gladitors...");
    uint32 oldMSTime;
    QueryResult result;
    uint16 count;
    
    /*============ POINTS ============*/
    if (GladDebug)
        sLog->outError("Start loadig points");
    oldMSTime = getMSTime();
    result = WorldDatabase.PQuery(SQL_POINTS);
    count = 0;
    if (!result)
    {
        sLog->outString(">> `_gladiator_points` is empty");
        sLog->outString();
    }
    do
    {
        Field* fields = result->Fetch();
        
        GladPoint* point    = new GladPoint();
        point->id           = fields[0].GetUInt32();
        point->map          = fields[1].GetUInt32();
        point->x            = fields[2].GetFloat();
        point->y            = fields[3].GetFloat();
        point->z            = fields[4].GetFloat();
        point->o            = fields[5].GetFloat();
        
        if (!MapManager::IsValidMapCoord(point->map, point->x, point->y, point->z, point->o))
        {
            sLog->outErrorDb("Wrong position for point id %u in `_gladiator_points` table, ignoring.", point->id);
            delete point;
            continue;
        }

        _points[point->id] = point;
        ++count;
    }
    while (result->NextRow());
    sLog->outString(">> Loaded %u points for Gladiators in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
    if (GladDebug)
        sLog->outError("End loadig points");
     /*============ POINTS ============*/
    
    /*============ TEMPLATES ============*/
    if (GladDebug)
        sLog->outError("Start loadig templates");
    oldMSTime = getMSTime();
    result = WorldDatabase.PQuery(SQL_TEMPLATE);
    count = 0;
    if (!result)
    {
        sLog->outString(">> `_gladiator_template` is empty");
        sLog->outString();
    }
    do
    {
        Field* fields = result->Fetch();
        
        GladTemplate* gladTemplate = new GladTemplate();
        gladTemplate->level        = fields[0].GetUInt32();
        gladTemplate->timeLimit    = fields[1].GetUInt32();
        gladTemplate->sayStart     = fields[2].GetUInt32();
        gladTemplate->sayWin       = fields[3].GetUInt32();
        gladTemplate->sayLose      = fields[3].GetUInt32();
        gladTemplate->chest        = fields[5].GetUInt32();
        gladTemplate->point        = fields[6].GetUInt32();
        gladTemplate->reqQuest     = fields[7].GetUInt32();
        gladTemplate->killCredit   = fields[8].GetUInt32();
        gladTemplate->menuString   = fields[9].GetUInt32();
        
        if (gladTemplate->point && !pointExists(gladTemplate->point))
        {
            sLog->outErrorDb("Wrong point_id %u for level %u in `_gladiator_template` table, ignoring.", gladTemplate->point, gladTemplate->level);
            gladTemplate->point = 0;
        }
        
        if (gladTemplate->chest && !sObjectMgr->GetGameObjectTemplate(gladTemplate->chest))
        {
            sLog->outErrorDb("Wrong chest %u for level %u in `_gladiator_template` table, ignoring.", gladTemplate->chest, gladTemplate->level);
            gladTemplate->chest = 0;
        }
        
        if (gladTemplate->reqQuest && !sObjectMgr->GetQuestTemplate(gladTemplate->reqQuest))
        {
            sLog->outErrorDb("Wrong reqQuest %u for level %u in `_gladiator_template` table, ignoring.", gladTemplate->reqQuest, gladTemplate->level);
            gladTemplate->reqQuest = 0;
        }
        
        if (gladTemplate->menuString && !sObjectMgr->GetTrinityStringLocale(gladTemplate->menuString))
        {
            sLog->outErrorDb("Wrong menuString %u for level %u in `_gladiator_template` table, ignoring.", gladTemplate->menuString, gladTemplate->level);
            gladTemplate->menuString = 0;
        }
        
        _templates[gladTemplate->level] = gladTemplate;
        ++count;
    }
    while (result->NextRow());
    sLog->outString(">> Loaded %u templates for Gladiators in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
    if (GladDebug)
        sLog->outError("End loadig templates");
     /*============ TEMPLATES ============*/

    
    /*============ CREATURES ============*/
    if (GladDebug)
        sLog->outError("Start loadig creatures");
    oldMSTime = getMSTime();
    result = WorldDatabase.PQuery(SQL_CREATURE);
    count = 0;
    if (!result)
    {
        sLog->outString(">> `_gladiator_creature` is empty");
        sLog->outString();
    }
    do
    {
        Field* fields = result->Fetch();
        
        GladCreature* gladCreature  = new GladCreature();
        gladCreature->id            = fields[0].GetUInt32();
        gladCreature->level         = fields[1].GetUInt32();
        gladCreature->entry         = fields[2].GetUInt32();
        gladCreature->count         = fields[3].GetUInt32();
        gladCreature->point         = fields[4].GetUInt32();
        gladCreature->move          = fields[5].GetUInt32();
        gladCreature->time          = fields[6].GetUInt32();
        gladCreature->spawn         = false;
        
        if (GladDebug)
            sLog->outError("Gladiators::load -> check creature %u", gladCreature->id);
        
        if (!templateExists(gladCreature->level))
        {
            sLog->outErrorDb("Wrong level gladiator %u for creature id %u in `_gladiator_creature` table, ignoring.", gladCreature->level, gladCreature->id);
            delete gladCreature;
            continue;
        }
        
        if (!pointExists(gladCreature->point))
        {
            sLog->outErrorDb("Wrong from point %u for creature id %u in `_gladiator_creature` table, ignoring.", gladCreature->point, gladCreature->id);
            delete gladCreature;
            continue;
        }
        
        if (!pointExists(gladCreature->move))
        {
            sLog->outErrorDb("Wrong to point %u for creature id %u in `_gladiator_creature` table, ignoring.", gladCreature->move, gladCreature->id);
            gladCreature->move = 0;
        }
        
        if (!sObjectMgr->GetCreatureTemplate(gladCreature->entry))
        {
            sLog->outErrorDb("Wrong entry %u for creature id %u in `_gladiator_creature` table, ignoring.", gladCreature->entry, gladCreature->id);
            delete gladCreature;
            continue;
        }
        
        if (GladDebug)
            sLog->outError("Gladiators::load -> end check creature %u", gladCreature->id);

        _templates[gladCreature->level]->creatures.push_back(gladCreature);
        
        if (GladDebug)
            sLog->outError("Gladiators::load -> creature %u added", gladCreature->id);
            
        ++count;
    }
    while (result->NextRow());
    sLog->outString(">> Loaded %u creatures for Gladiators in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
    if (GladDebug)
        sLog->outError("end loadig creatures");
     /*============ CREATURES ============*/
}

void Gladiators::clear()
{
    if (GladDebug)
        sLog->outError("Gladiators::clear");
        
    for (GladPointMap::const_iterator itr = _points.begin(); itr != _points.end(); ++itr)
        delete (*itr).second;
    _points.clear();
    
    if (!_creatureList.empty())
        for (CreatureList::const_iterator itr = _creatureList.begin(); itr != _creatureList.end(); ++itr)
            (*itr)->DespawnOrUnsummon();
    _creatureList.clear();
    
    for (GladTemplates::const_iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
    {
        for (GladCreatureList::const_iterator it = itr->second->creatures.begin(); it != itr->second->creatures.end(); ++it)
        {
            delete *it;
        }
        itr->second->creatures.clear();
        delete (*itr).second;
    }
    _templates.clear();
}

void Gladiators::reset()
{
    if (GladDebug)
        sLog->outError("Gladiators::reset");
        
    // reset spawn
    if (_currentLevel && !_currentLevel->creatures.empty())
        for (GladCreatureList::const_iterator itr = _currentLevel->creatures.begin(); itr != _currentLevel->creatures.end(); ++itr)
            (*itr)->spawn = false;
    
    if (!_creatureList.empty())
        for (CreatureList::const_iterator itr = _creatureList.begin(); itr != _creatureList.end(); ++itr)
            (*itr)->DespawnOrUnsummon();
        
    _creatureList.clear();
}

void Gladiators::start(uint32 level, Player* player)
{
    if (GladDebug)
        sLog->outError("Gladiators::start");
        
    if (_inProgress)
        return;
        
    _playerStart = player;
    
    if (_organizer)
    {
        _organizer->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        _organizer->SetVisible(false);
    }
        
    _currentLevel = _templates[level];
    _level = level;
    
    reset();
    
    _currentLevel->time = 0;
    _inProgress = true;
    
    if (_organizer && _currentLevel->sayStart)
        sCreatureTextMgr->SendChat(_organizer, _currentLevel->sayStart);
}

void Gladiators::update(uint32 diff)
{
    if (!_inProgress)
        return;

    /*if (GladDebug)
        sLog->outError("Gladiators::update %u", diff);*/
        
    _currentLevel->time += diff;
        
    if (checkEnd())
        return;
        
    /*if (GladDebug)
        sLog->outError("Gladiators::update after checkEnd");*/
        
    for (GladCreatureList::const_iterator itr = _currentLevel->creatures.begin(); itr != _currentLevel->creatures.end(); ++itr)
        if (!(*itr)->spawn && _currentLevel->time >= (*itr)->time)
        {
            if (GladDebug)
                sLog->outError("Gladiators::update -> spawn %u", (*itr)->entry);
                
            GladPoint const* point = getPoint((*itr)->point);
            GladPoint const* move = NULL;
            if ((*itr)->move)
                move = getPoint((*itr)->move);
                
            if (_organizer && point)
            {
                for (uint32 i = 0; i < (*itr)->count; ++i)
                {
                    Creature* summon = _organizer->SummonCreature((*itr)->entry, point->x, point->y, point->z, point->o, TEMPSUMMON_TIMED_DESPAWN, _currentLevel->timeLimit + 10000);
                    if (move)
                    {
                        summon->SetHomePosition(move->x, move->y, move->z, move->o);
                        summon->GetMotionMaster()->MovePoint(0, move->x, move->y, move->z);
                    }
                    else if (_playerStart && _playerStart->isAlive())
                    {
                        summon->SetHomePosition(point->x, point->y, point->z, point->o);
                        summon->AI()->AttackStart(_playerStart);
                    }
                    _creatureList.push_back(summon);
                }
            }
            (*itr)->spawn = true;
        }
}

void Gladiators::stop(bool win)
{
    if (GladDebug)
        sLog->outError("Gladiators::stop win: %i", int(win));
                
    if (_organizer)
    {
        _organizer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        _organizer->SetVisible(true);
    }
    
    _inProgress = false;
    reset();
    
    if (!win)
    {
        if (_organizer && _currentLevel->sayLose)
            sCreatureTextMgr->SendChat(_organizer, _currentLevel->sayLose);
            
        if (GladLadder)
            _level = 0;
        return;
    }
    
    if (_organizer && _currentLevel->sayWin)
        sCreatureTextMgr->SendChat(_organizer, _currentLevel->sayWin);
    
    if (_currentLevel->chest && _currentLevel->point)
    {
        GladPoint const* point = getPoint(_currentLevel->point);
        if (_organizer && point)
            _organizer->SummonGameObject(_currentLevel->chest, point->x, point->y, point->z, point->o, 0, 0, 0, 0, 300);
    }
    
    if (_organizer && _currentLevel->killCredit)
    {
        ObjectList* units = new ObjectList();
        Trinity::AllWorldObjectsInRange u_check(_organizer, 60.0f);
        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(_organizer, *units, u_check);
        _organizer->VisitNearbyObject(60.0f, searcher);

        if (!units->empty())
            for (ObjectList::const_iterator itr = units->begin(); itr != units->end(); ++itr)
                if ((*itr)->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)(*itr))->KilledMonsterCredit(_currentLevel->killCredit, 0);

        delete units;
    }
}

bool Gladiators::existsAlive()
{
    for (CreatureList::const_iterator itr = _creatureList.begin(); itr != _creatureList.end(); ++itr)
        if ((*itr)->isAlive())
            return true;
            
    return false;
}

bool Gladiators::checkEnd()
{
    if (_currentLevel->time >= _currentLevel->timeLimit)
    {
        stop();
        return true;
    }
    
    for (GladCreatureList::const_iterator itr = _currentLevel->creatures.begin(); itr != _currentLevel->creatures.end(); ++itr)
        if (!(*itr)->spawn)
            return false;
            
    if (existsAlive())
        return false;

    stop(true);

    return true;
}

void Gladiators::addGossip(Player* player)
{
    if (_inProgress)
        return;
        
    if (GladDebug)
        sLog->outError("Gladiators::addGossip");
        
    char gossipTextFormat[100];
        
    if (GladLadder)
    {
        if (GladDebug)
            sLog->outError("Gladiators::addGossip -> GladLadder: _level %u", _level);
            
        uint32 newLevel = _level + 1;
            
        if (!templateExists(newLevel))
            newLevel = 1;
        
        if (GladLadderText)
            snprintf(gossipTextFormat, 100, sObjectMgr->GetTrinityStringForDBCLocale(GladLadderText), newLevel);
        else
            snprintf(gossipTextFormat, 100, "[%u] Next level", newLevel);
                
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipTextFormat, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + newLevel);
        return;
    }
        
    uint8 i = 0;
    uint8 limit = 9;
        
    for (GladTemplates::const_iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
    {
        if (i >= limit)
            return;
            
        GladTemplate* levels = itr->second;
        
        if (levels->reqQuest && player->GetQuestStatus(levels->reqQuest) != QUEST_STATUS_COMPLETE)
            continue;
        
        if (levels->menuString)
            snprintf(gossipTextFormat, 100, sObjectMgr->GetTrinityStringForDBCLocale(levels->menuString), levels->level);
        else
            snprintf(gossipTextFormat, 100, "Start level: %u", levels->level);
            
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipTextFormat, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + levels->level);
        
        ++i;
    }
}

Gladiators GladManager;

class npc_gladiators_organizer : public CreatureScript
{
    public:
        npc_gladiators_organizer() : CreatureScript("npc_gladiators_organizer") { }

    struct npc_gladiators_organizerAI : public ScriptedAI
    {
        npc_gladiators_organizerAI(Creature* creature) : ScriptedAI(creature)
        {
            GladManager.setOrganizer(creature);
        }
    };
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_gladiators_organizerAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (GladEnable)
            GladManager.addGossip(player);
        
        player->PlayerTalkClass->SendGossipMenu(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        
        if (GladEnable && action > GOSSIP_ACTION_INFO_DEF)
            GladManager.start(action - GOSSIP_ACTION_INFO_DEF, player);

        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};


class Mod_Gladiators_WorldScript : public WorldScript
{
    public:
        Mod_Gladiators_WorldScript() : WorldScript("Mod_Gladiators_WorldScript") { }

    void OnConfigLoad(bool /*reload*/)
    {
        GladEnable  = ConfigMgr::GetBoolDefault("Gladiators.Enable", false);
        GladDebug  = ConfigMgr::GetBoolDefault("Gladiators.Debug", false);
        GladLadder  = ConfigMgr::GetBoolDefault("Gladiators.Ladder", false);
        GladLadderText =  ConfigMgr::GetIntDefault("Gladiators.LadderText", 0);

        if (!GladEnable)
            return;

        GladManager.load();
        
        if (!GladManager.templateExists(1))
            GladEnable = false;
    }
    
    void OnUpdate(uint32 diff)
    {
        if (!GladEnable)
            return;

        GladManager.update(diff);
    }
};

class Mod_Gladiators_CommandScript : public CommandScript
{
public:
    Mod_Gladiators_CommandScript() : CommandScript("Mod_Gladiators_CommandScript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand commandTable[] =
        {
            { "gladiator",      SEC_ADMINISTRATOR,  false, &HandleGladiatorsChatCommand,       "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleGladiatorsChatCommand(ChatHandler* /*handler*/, char const* args)
    {
        if (!GladEnable || !*args)
            return false;

        std::string param = (char*)args;

        if (param == "reload")
        {
            GladManager.load(true);
            return true;
        }

        return true;
    }
};

void AddSC_Mod_Gladiators()
{
    new Mod_Gladiators_WorldScript();
    new Mod_Gladiators_CommandScript();
    new npc_gladiators_organizer();
}