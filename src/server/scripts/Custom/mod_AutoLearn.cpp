#include "ScriptPCH.h"
#include "Config.h"

bool AutoLearnEnable        = true;
bool SpellClass             = true; // IN DEV
bool SpellRiding            = true;
bool DualSpec               = true;
bool AutoLearnCheckLevel    = true;
//IN DEV
//bool SpellWeapon = true;
//bool SpellMount = true;

class Mod_AutoLearn_WorldScript : public WorldScript
{
    public:
        Mod_AutoLearn_WorldScript()
            : WorldScript("Mod_AutoLearn_WorldScript")
        {
        }

    void SetInitialWorldSettings()
    {
        AutoLearnEnable = ConfigMgr::GetBoolDefault("AutoLearn.Enable", true);

        if (!AutoLearnEnable)
            return;

        AutoLearnCheckLevel = ConfigMgr::GetBoolDefault("AutoLearn.Check.Level", true);

        SpellClass = ConfigMgr::GetBoolDefault("AutoLearn.SpellClass", true);
        SpellRiding = ConfigMgr::GetBoolDefault("AutoLearn.SpellRiding", true);
        DualSpec = ConfigMgr::GetBoolDefault("AutoLearn.DualSpec", true);
        //SpellWeapon = ConfigMgr::GetBoolDefault("AutoLearn.SpellWeapon", true);
        //SpellMount = ConfigMgr::GetBoolDefault("AutoLearn.SpellMount", true);
    }
};

class Mod_AutoLearn_PlayerScript : public PlayerScript
{
    public:
        Mod_AutoLearn_PlayerScript()
            : PlayerScript("Mod_AutoLearn_PlayerScript")
        {
        }

    Player* character;
    uint32 level;


    void OnLevelChanged(Player* player, uint8 oldLevel)
    {
        if (!AutoLearnEnable || !AutoLearnCheckLevel) return;

        CheckSpell(player);
    }

    void CheckSpell(Player* player)
    {
        level = player->getLevel();
        character = player;

        learnSpellRiding();
        learnDualSpec();
        learnSpellClass();
    }

    void learn(uint32 spell, uint32 required = 0)
    {
        if (spell == 0) return;

        if (character->HasSpell(spell)) return;

        if (required != 0 && !character->HasSpell(required)) return;

        character->learnSpell(spell, false);
    }

    void learnSpellRiding()
    {
        if (!SpellRiding) return;

        if (level > 19) learn(33388);
        if (level > 39) learn(33391);
        if (level > 59) learn(34090);
        if (level > 69) learn(34091);
        if (level > 76) learn(54197);// air northland
    }

    void learnDualSpec()
    {
        if (!DualSpec) return;

        if (level < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL)) return;

        if (character->GetSpecsCount() != 1) return;

        character->CastSpell(character, 63680, true, NULL, NULL, character->GetGUID());
        character->CastSpell(character, 63624, true, NULL, NULL, character->GetGUID());
    }

    void learnSpellClass()
    {
        if (!SpellClass) return;

        if (character->getClass() == CLASS_ROGUE)
            learnSpellRogue();
        else if (character->getClass() == CLASS_HUNTER)
            learnSpellHunter();
        else if (character->getClass() == CLASS_SHAMAN)
            learnSpellShaman();
        else if (character->getClass() == CLASS_WARRIOR)
            learnSpellWarrior();
        else if (character->getClass() == CLASS_PALADIN)
            learnSpellPaladin();
        else if (character->getClass() == CLASS_PRIEST)
            learnSpellPriest();
        else if (character->getClass() == CLASS_DEATH_KNIGHT)
            learnSpellDeathKnight();
        else if (character->getClass() == CLASS_MAGE)
            learnSpellMage();
        else if (character->getClass() == CLASS_WARLOCK)
            learnSpellWarlock();
        else if (character->getClass() == CLASS_DRUID)
            learnSpellDruid();
    }

    void learnSpellRogue()
    {
        learn(1784); //Незаметность

        if (level < 4) return;

        learn(53); //Удар в спину Уровень 1
        learn(921); //Обшаривание карманов

        if (level < 6) return;

        learn(1757, 1752); //Коварный удар Уровень 2
        learn(1776); //Парализующий удар

        if (level < 8) return;

        learn(6760, 2098); //Потрошение Уровень 2
        learn(5277); //Ускользание Уровень 1

        if (level < 10) return;

        learn(2983); //Спринт Уровень 1
        learn(6770); //Ошеломление Уровень 1
        learn(5171); //Мясорубка Уровень 1
        learn(674); //Бой двумя оружиями Пассивная

        if (level < 12) return;

        learn(2589, 53); //Удар в спину Уровень 2
        learn(1766); //Пинок 
        learn(3127); //Парирование Пассивная

        if (level < 14) return;

        learn(1758, 1752); //Коварный удар Уровень 3
        learn(703); //Гаррота Уровень 1
        learn(8647); //Ослабление доспеха

        if (level < 16) return;

        learn(1804); //Взлом замка
        learn(6761, 2098); //Потрошение Уровень 3
        learn(1966); //Ложный выпад Уровень 1

        if (level < 18) return;

        learn(8676); //Внезапный удар Уровень 1

        if (level < 20) return;

        learn(2590, 53); //Удар в спину Уровень 3
        learn(1943); //Рваная рана Уровень 1
        learn(51722); //unk

        if (level < 22) return;

        learn(1759, 1752); //Коварный удар Уровень 4
        learn(8631, 703); //Гаррота Уровень 2
        learn(1856); //Исчезновение Уровень 1
        learn(1725); //Отвлечение

        if (level < 24) return;

        learn(6762, 2098); //Потрошение Уровень 4
        learn(2836); //Обнаружение ловушек Пассивная

        if (level < 26) return;

        learn(1833); //Подлый трюк
        learn(8724, 8676); //Внезапный удар Уровень 2

        if (level < 28) return;

        learn(6768, 1966); //Ложный выпад Уровень 2
        learn(2070, 6770); //Ошеломление Уровень 2
        learn(8639, 1943); //Рваная рана Уровень 2
        learn(2591, 53); //Удар в спину Уровень 4

        if (level < 30) return;

        learn(8632, 703); //Гаррота Уровень 3
        learn(408); //Удар по почкам Уровень 1
        learn(1842); //Обезвреживание ловушки
        learn(1760, 1752); //Коварный удар Уровень 5

        if (level < 32) return;

        learn(8623, 2098); //Потрошение Уровень 5

        if (level < 34) return;

        learn(8696, 2983); //Спринт Уровень 2
        learn(2094); //Ослепление
        learn(8725, 8676); //Внезапный удар Уровень 3

        if (level < 36) return;

        learn(8640, 1943); //Рваная рана Уровень 3
        learn(8721, 53); //Удар в спину Уровень 5

        if (level < 38) return;

        learn(8621, 1752); //Коварный удар Уровень 6
        learn(8633, 703); //Гаррота Уровень 4

        if (level < 40) return;

        learn(8637, 1966); //Ложный выпад Уровень 3
        learn(1860); //Безопасное падение Пассивная
        learn(8624, 2098); //Потрошение Уровень 6

        if (level < 42) return;

        learn(11267, 8676); //Внезапный удар Уровень 4
        learn(6774, 5171); //Мясорубка Уровень 2
        learn(1857, 1856); //Исчезновение Уровень 2

        if (level < 44) return;

        learn(11273, 1943); //Рваная рана Уровень 4
        learn(11279, 53); //Удар в спину Уровень 6

        if (level < 46) return;

        learn(17347, 16511); //Кровоизлияние Уровень 2
        learn(11289, 703); //Гаррота Уровень 5
        learn(11293, 1752); //Коварный удар Уровень 7

        if (level < 48) return;

        learn(11297, 6770); //Ошеломление Уровень 3
        learn(11299, 2098); //Потрошение Уровень 7

        if (level < 50) return;

        learn(8643, 408); //Удар по почкам Уровень 2
        learn(26669, 5277); //Ускользание Уровень 2
        learn(34411, 1329); //Расправа Уровень 2
        learn(11268, 8676); //Внезапный удар Уровень 5

        if (level < 52) return;

        learn(11303, 1966); //Ложный выпад Уровень 4
        learn(11274, 1943); //Рваная рана Уровень 5
        learn(11280, 53); //Удар в спину Уровень 7

        if (level < 54) return;

        learn(11290, 703); //Гаррота Уровень 6
        learn(11294, 1752); //Коварный удар Уровень 8

        if (level < 56) return;
        
        learn(11300, 2098); //Потрошение Уровень 8

        if (level < 58) return;

        learn(11305, 2983); //Спринт Уровень 3
        learn(11269, 8676); //Внезапный удар Уровень 6
        learn(17348, 16511); //Кровоизлияние Уровень 3

        if (level < 60) return;

        learn(31016, 2098); //Потрошение Уровень 9
        learn(25302, 1966); //Ложный выпад Уровень 5
        learn(11281, 53); //Удар в спину Уровень 8
        learn(25300, 53); //Удар в спину Уровень 9
        learn(34412, 1329); //Расправа Уровень 3
        learn(11275, 1943); //Рваная рана Уровень 6

        if (level < 61) return;

        learn(26839, 703); //Гаррота Уровень 7

        if (level < 62) return;

        learn(26861, 1752); //Коварный удар Уровень 9
        learn(32645); //Отравление Уровень 1
        learn(26889, 1856); //Исчезновение Уровень 3

        if (level < 64) return;

        learn(26865, 2098); //Потрошение Уровень 10
        learn(26679); //Смертельный бросок Уровень 1
        learn(27448, 1966); //Ложный выпад Уровень 6

        if (level < 66) return;

            if (!character->HasSpell(31224)) character->learnSpell(31224, false); //Плащ Теней 
            if (!character->HasSpell(27441) && character->HasSpell(8676)) character->learnSpell(27441, false); //Внезапный удар Уровень 7
            //if (!character->HasSpell(26866)) character->learnSpell(26866, false); //Ослабление доспеха Уровень 6

            if (level < 68) return;

            if (!character->HasSpell(26867) && character->HasSpell(1943)) character->learnSpell(26867, false); //Рваная рана Уровень 7
            if (!character->HasSpell(26863) && character->HasSpell(53)) character->learnSpell(26863, false); //Удар в спину Уровень 10

            if (level < 69) return;

            if (!character->HasSpell(32684) && character->HasSpell(32645)) character->learnSpell(32684, false); //Отравление Уровень 2

            if (level < 70) return;

            if (!character->HasSpell(48689) && character->HasSpell(8676)) character->learnSpell(48689, false); // unk
            if (!character->HasSpell(34413) && character->HasSpell(1329)) character->learnSpell(34413, false); //Расправа Уровень 4
            if (!character->HasSpell(48673) && character->HasSpell(26679)) character->learnSpell(48673, false); // unk
            if (!character->HasSpell(5938)) character->learnSpell(5938, false); //Отравляющий укол 
            if (!character->HasSpell(26884) && character->HasSpell(703)) character->learnSpell(26884, false); //Гаррота Уровень 8
            if (!character->HasSpell(26864) && character->HasSpell(16511)) character->learnSpell(26864, false); //Кровоизлияние Уровень 4
            if (!character->HasSpell(26862) && character->HasSpell(1752)) character->learnSpell(26862, false); //Коварный удар Уровень 10

            if (level < 71) return;
            
            if (!character->HasSpell(51724) && character->HasSpell(6770)) character->learnSpell(51724, false); // unk

            if (level < 72) return;

            if (!character->HasSpell(48658) && character->HasSpell(1966)) character->learnSpell(48658, false); // unk

            if (level < 73) return;

            if (!character->HasSpell(48667) && character->HasSpell(2098)) character->learnSpell(48667, false); // unk

            if (level < 74) return;

            if (!character->HasSpell(57992) && character->HasSpell(32645)) character->learnSpell(57992, false); //Отравление Уровень 3
            if (!character->HasSpell(48671) && character->HasSpell(1943)) character->learnSpell(48671, false); // unk
            if (!character->HasSpell(48656) && character->HasSpell(53)) character->learnSpell(48656, false); // unk

            if (level < 75) return;

            if (!character->HasSpell(57934)) character->learnSpell(57934, false); //Маленькие хитрости 
            if (!character->HasSpell(48690) && character->HasSpell(8676)) character->learnSpell(48690, false); // unk
            if (!character->HasSpell(48663) && character->HasSpell(1329)) character->learnSpell(48663, false); // unk
            if (!character->HasSpell(48675) && character->HasSpell(703)) character->learnSpell(48675, false); // unk

            if (level < 76) return;

            if (!character->HasSpell(48637) && character->HasSpell(1752)) character->learnSpell(48637, false); // unk
            if (!character->HasSpell(48674) && character->HasSpell(26679)) character->learnSpell(48674, false); // unk

            //if (level < 77) return;
            //if (!character->HasSpell(48669)) character->learnSpell(48669, false); //Ослабление доспеха уровень 7

            if (level < 78) return;

            if (!character->HasSpell(48659) && character->HasSpell(1966)) character->learnSpell(48659, false); // unk

            if (level < 79) return;

            if (!character->HasSpell(48672) && character->HasSpell(1943)) character->learnSpell(48672, false); // unk
            if (!character->HasSpell(48668) && character->HasSpell(2098)) character->learnSpell(48668, false); // unk

            if (level < 80) return;

            if (!character->HasSpell(48676) && character->HasSpell(703)) character->learnSpell(48676, false); // unk
            if (!character->HasSpell(48638) && character->HasSpell(1752)) character->learnSpell(48638, false); // unk
            if (!character->HasSpell(48657) && character->HasSpell(53)) character->learnSpell(48657, false); // unk
            if (!character->HasSpell(48660) && character->HasSpell(16511)) character->learnSpell(48660, false); // unk
            if (!character->HasSpell(51723)) character->learnSpell(51723, false); //Веер клинков
            if (!character->HasSpell(48666) && character->HasSpell(1329)) character->learnSpell(48666, false); // unk
            if (!character->HasSpell(48691) && character->HasSpell(8676)) character->learnSpell(48691, false); // unk
            if (!character->HasSpell(57993) && character->HasSpell(32645)) character->learnSpell(57993, false); //Отравление Уровень 4
    }

    void learnSpellHunter()
    {

            if (!character->HasSpell(1494)) character->learnSpell(1494, false); //Выслеживание животных 

            if (level < 4) return;

            if (!character->HasSpell(1978)) character->learnSpell(1978, false); //Укус змеи Уровень 1
            if (!character->HasSpell(13163)) character->learnSpell(13163, false); //Дух обезьяны 

            if (level < 6) return;

            if (!character->HasSpell(1130)) character->learnSpell(1130, false); //Метка охотника Уровень 1
            if (!character->HasSpell(3044)) character->learnSpell(3044, false); //Чародейский выстрел Уровень 1

            if (level < 8) return;

            if (!character->HasSpell(5116)) character->learnSpell(5116, false); //Контузящий выстрел 
            if (!character->HasSpell(14260) && character->HasSpell(2973)) character->learnSpell(14260, false); //Удар ящера Уровень 2
            if (!character->HasSpell(3127)) character->learnSpell(3127, false); //Парирование Пассивная

            if (level < 10) return;

            if (!character->HasSpell(1462)) character->learnSpell(1462, false); //Знание зверя
            if (!character->HasSpell(34026)) character->learnSpell(34026, false); //Команда "Взять!"
            if (!character->HasSpell(6991)) character->learnSpell(6991, false); //Кормление питомца
            if (!character->HasSpell(1515)) character->learnSpell(1515, false); //Приручение зверя
            if (!character->HasSpell(2641)) character->learnSpell(2641, false); //Прогнать питомца

            if (!character->HasSpell(19883)) character->learnSpell(19883, false); //Выслеживание гуманоидов 
            if (!character->HasSpell(13165)) character->learnSpell(13165, false); //Дух ястреба Уровень 1
            if (!character->HasSpell(13549) && character->HasSpell(1978)) character->learnSpell(13549, false); //Укус змеи Уровень 2
            
            if (level >= 12 && !character->HasSpell(14281) && character->HasSpell(3044)) character->learnSpell(14281, false); //Чародейский выстрел Уровень 2
            if (level >= 12 && !character->HasSpell(20736)) character->learnSpell(20736, false); //Отвлекающий выстрел Уровень 1
            if (level >= 12 && !character->HasSpell(136)) character->learnSpell(136, false); //Лечение питомца Уровень 1
            if (level >= 12 && !character->HasSpell(2974)) character->learnSpell(2974, false); //Подрезать крылья 
            if (level >= 14 && !character->HasSpell(6197)) character->learnSpell(6197, false); //Орлиный глаз 
            if (level >= 14 && !character->HasSpell(1002)) character->learnSpell(1002, false); //Звериный глаз 
            if (level >= 14 && !character->HasSpell(1513)) character->learnSpell(1513, false); //Отпугивание зверя Уровень 1
            if (level >= 16 && !character->HasSpell(1495)) character->learnSpell(1495, false); //Укус мангуста Уровень 1
            if (level >= 16 && !character->HasSpell(13795)) character->learnSpell(13795, false); //Обжигающая ловушка Уровень 1
            if (level >= 16 && !character->HasSpell(14261) && character->HasSpell(2973)) character->learnSpell(14261, false); //Удар ящера Уровень 3
            if (level >= 16 && !character->HasSpell(5118)) character->learnSpell(5118, false); //Дух гепарда 
            if (level >= 18 && !character->HasSpell(14318) && character->HasSpell(13165)) character->learnSpell(14318, false); //Дух ястреба Уровень 2
            if (level >= 18 && !character->HasSpell(13550) && character->HasSpell(1978)) character->learnSpell(13550, false); //Укус змеи Уровень 3
            if (level >= 18 && !character->HasSpell(19884)) character->learnSpell(19884, false); //Выслеживание нежити 
            if (level >= 18 && !character->HasSpell(2643)) character->learnSpell(2643, false); //Залп Уровень 1
            if (level >= 20 && !character->HasSpell(781)) character->learnSpell(781, false); //Отрыв 
            if (level >= 20 && !character->HasSpell(1499)) character->learnSpell(1499, false); //Замораживающая ловушка Уровень 1
            if (level >= 20 && !character->HasSpell(14282) && character->HasSpell(3044)) character->learnSpell(14282, false); //Чародейский выстрел Уровень 3
            if (level >= 20 && !character->HasSpell(3111) && character->HasSpell(136)) character->learnSpell(3111, false); //Лечение питомца Уровень 2
            if (level >= 20 && !character->HasSpell(34074)) character->learnSpell(34074, false); //Дух гадюки 
            if (level >= 20 && !character->HasSpell(674)) character->learnSpell(674, false); //Бой двумя оружиями Пассивная
            if (level >= 22 && !character->HasSpell(3043)) character->learnSpell(3043, false); //Укус скорпида 
            if (level >= 22 && !character->HasSpell(14323) && character->HasSpell(1130)) character->learnSpell(14323, false); //Метка охотника Уровень 2
            if (level >= 24 && !character->HasSpell(1462)) character->learnSpell(1462, false); //Знание зверя 
            if (level >= 24 && !character->HasSpell(19885)) character->learnSpell(19885, false); //Выслеживание скрытных 
            if (level >= 24 && !character->HasSpell(14262) && character->HasSpell(2973)) character->learnSpell(14262, false); //Удар ящера Уровень 4
            if (level >= 26 && !character->HasSpell(13551) && character->HasSpell(1978)) character->learnSpell(13551, false); //Укус змеи Уровень 4
            if (level >= 26 && !character->HasSpell(19880)) character->learnSpell(19880, false); //Выслеживание элементалей 
            if (level >= 26 && !character->HasSpell(14302) && character->HasSpell(13795)) character->learnSpell(14302, false); //Обжигающая ловушка Уровень 2
            if (level >= 26 && !character->HasSpell(3045)) character->learnSpell(3045, false); //Быстрая стрельба 
            if (level >= 28 && !character->HasSpell(13809)) character->learnSpell(13809, false); //Ледяная ловушка 
            if (level >= 28 && !character->HasSpell(14283) && character->HasSpell(3044)) character->learnSpell(14283, false); //Чародейский выстрел Уровень 4
            if (level >= 28 && !character->HasSpell(14319) && character->HasSpell(13165)) character->learnSpell(14319, false); //Дух ястреба Уровень 3
            if (level >= 28 && !character->HasSpell(20900) && character->HasSpell(19434)) character->learnSpell(20900, false); //Прицельный выстрел Уровень 2
            if (level >= 28 && !character->HasSpell(3661) && character->HasSpell(136)) character->learnSpell(3661, false); //Лечение питомца Уровень 3
            if (level >= 30 && !character->HasSpell(14288) && character->HasSpell(2643)) character->learnSpell(14288, false); //Залп Уровень 2
            if (level >= 30 && !character->HasSpell(13161)) character->learnSpell(13161, false); //Дух зверя 
            if (level >= 30 && !character->HasSpell(14326) && character->HasSpell(1513)) character->learnSpell(14326, false); //Отпугивание зверя Уровень 2
            if (level >= 30 && !character->HasSpell(14269) && character->HasSpell(1495)) character->learnSpell(14269, false); //Укус мангуста Уровень 2
            if (level >= 30 && !character->HasSpell(5384)) character->learnSpell(5384, false); //Притвориться мертвым 
            if (level >= 32 && !character->HasSpell(1543)) character->learnSpell(1543, false); //Осветительная ракета 
            if (level >= 32 && !character->HasSpell(14263) && character->HasSpell(2973)) character->learnSpell(14263, false); //Удар ящера Уровень 5
            if (level >= 32 && !character->HasSpell(19878)) character->learnSpell(19878, false); //Выслеживание демонов 
            if (level >= 34 && !character->HasSpell(13813)) character->learnSpell(13813, false); //Взрывная ловушка Уровень 1
            if (level >= 34 && !character->HasSpell(13552) && character->HasSpell(1978)) character->learnSpell(13552, false); //Укус змеи Уровень 5
            if (level >= 36 && !character->HasSpell(14284) && character->HasSpell(3044)) character->learnSpell(14284, false); //Чародейский выстрел Уровень 5
            if (level >= 36 && !character->HasSpell(14303) && character->HasSpell(13795)) character->learnSpell(14303, false); //Обжигающая ловушка Уровень 3
            if (level >= 36 && !character->HasSpell(3034)) character->learnSpell(3034, false); //Укус гадюки 
            if (level >= 36 && !character->HasSpell(3662) && character->HasSpell(136)) character->learnSpell(3662, false); //Лечение питомца Уровень 4
            if (level >= 36 && !character->HasSpell(20901) && character->HasSpell(19434)) character->learnSpell(20901, false); //Прицельный выстрел Уровень 3
            if (level >= 38 && !character->HasSpell(14320) && character->HasSpell(13165)) character->learnSpell(14320, false); //Дух ястреба Уровень 4
            if (level >= 40 && !character->HasSpell(19882)) character->learnSpell(19882, false); //Выслеживание великанов 
            if (level >= 40 && !character->HasSpell(8737)) character->learnSpell(8737, false); //Кольчужные доспехи 
            if (level >= 40 && !character->HasSpell(13159)) character->learnSpell(13159, false); //Дух стаи 
            if (level >= 40 && !character->HasSpell(1510)) character->learnSpell(1510, false); //Град стрел Уровень 1
            if (level >= 40 && !character->HasSpell(14310) && character->HasSpell(1499)) character->learnSpell(14310, false); //Замораживающая ловушка Уровень 2
            if (level >= 40 && !character->HasSpell(14264) && character->HasSpell(2973)) character->learnSpell(14264, false); //Удар ящера Уровень 6
            if (level >= 40 && !character->HasSpell(14324) && character->HasSpell(1130)) character->learnSpell(14324, false); //Метка охотника Уровень 3
            if (level >= 42 && !character->HasSpell(20909) && character->HasSpell(19306)) character->learnSpell(20909, false); //Контратака Уровень 2
            if (level >= 42 && !character->HasSpell(14289) && character->HasSpell(2643)) character->learnSpell(14289, false); //Залп Уровень 3
            if (level >= 42 && !character->HasSpell(13553) && character->HasSpell(1978)) character->learnSpell(13553, false); //Укус змеи Уровень 6
            if (level >= 44 && !character->HasSpell(14285) && character->HasSpell(3044)) character->learnSpell(14285, false); //Чародейский выстрел Уровень 6
            if (level >= 44 && !character->HasSpell(14270) && character->HasSpell(1495)) character->learnSpell(14270, false); //Укус мангуста Уровень 3
            if (level >= 44 && !character->HasSpell(20902) && character->HasSpell(19434)) character->learnSpell(20902, false); //Прицельный выстрел Уровень 4
            if (level >= 44 && !character->HasSpell(13542) && character->HasSpell(136)) character->learnSpell(13542, false); //Лечение питомца Уровень 5
            if (level >= 44 && !character->HasSpell(14316) && character->HasSpell(13813)) character->learnSpell(14316, false); //Взрывная ловушка Уровень 2
            if (level >= 46 && !character->HasSpell(14304) && character->HasSpell(13795)) character->learnSpell(14304, false); //Обжигающая ловушка Уровень 4
            if (level >= 46 && !character->HasSpell(14327) && character->HasSpell(1513)) character->learnSpell(14327, false); //Отпугивание зверя Уровень 3
            if (level >= 46 && !character->HasSpell(20043)) character->learnSpell(20043, false); //Дух дикой природы Уровень 1
            if (level >= 48 && !character->HasSpell(14321) && character->HasSpell(13165)) character->learnSpell(14321, false); //Дух ястреба Уровень 5
            if (level >= 48 && !character->HasSpell(14265) && character->HasSpell(2973)) character->learnSpell(14265, false); //Удар ящера Уровень 7
            if (level >= 50 && !character->HasSpell(19879)) character->learnSpell(19879, false); //Выслеживание драконов 
            if (level >= 50 && !character->HasSpell(24132) && character->HasSpell(19386)) character->learnSpell(24132, false); //Укус виверны Уровень 2
            if (level >= 50 && !character->HasSpell(13554) && character->HasSpell(1978)) character->learnSpell(13554, false); //Укус змеи Уровень 7
            if (level >= 50 && !character->HasSpell(56641)) character->learnSpell(56641, false); //Верный выстрел Уровень 1
            if (level >= 50 && !character->HasSpell(14294) && character->HasSpell(1510)) character->learnSpell(14294, false); //Град стрел Уровень 2
            if (level >= 52 && !character->HasSpell(13543) && character->HasSpell(136)) character->learnSpell(13543, false); //Лечение питомца Уровень 6
            if (level >= 52 && !character->HasSpell(20903) && character->HasSpell(19434)) character->learnSpell(20903, false); //Прицельный выстрел Уровень 5
            if (level >= 52 && !character->HasSpell(14286) && character->HasSpell(3044)) character->learnSpell(14286, false); //Чародейский выстрел Уровень 7
            if (level >= 54 && !character->HasSpell(14290) && character->HasSpell(2643)) character->learnSpell(14290, false); //Залп Уровень 4
            if (level >= 54 && !character->HasSpell(14317) && character->HasSpell(13813)) character->learnSpell(14317, false); //Взрывная ловушка Уровень 3
            if (level >= 54 && !character->HasSpell(20910) && character->HasSpell(19306)) character->learnSpell(20910, false); //Контратака Уровень 3
            if (level >= 56 && !character->HasSpell(20190) && character->HasSpell(20043)) character->learnSpell(20190, false); //Дух дикой природы Уровень 2
            if (level >= 56 && !character->HasSpell(14305) && character->HasSpell(13795)) character->learnSpell(14305, false); //Обжигающая ловушка Уровень 5
            if (level >= 56 && !character->HasSpell(14266) && character->HasSpell(2973)) character->learnSpell(14266, false); //Удар ящера Уровень 8
            if (level >= 57 && !character->HasSpell(63668) && character->HasSpell(3674)) character->learnSpell(63668, false); //Черная стрела Уровень 2
            if (level >= 58 && !character->HasSpell(13555) && character->HasSpell(1978)) character->learnSpell(13555, false); //Укус змеи Уровень 8
            if (level >= 58 && !character->HasSpell(14295) && character->HasSpell(1510)) character->learnSpell(14295, false); //Град стрел Уровень 3
            if (level >= 58 && !character->HasSpell(14271) && character->HasSpell(1495)) character->learnSpell(14271, false); //Укус мангуста Уровень 4
            if (level >= 58 && !character->HasSpell(14325) && character->HasSpell(1130)) character->learnSpell(14325, false); //Метка охотника Уровень 4
            if (level >= 58 && !character->HasSpell(14322) && character->HasSpell(13165)) character->learnSpell(14322, false); //Дух ястреба Уровень 6
            if (level >= 60 && !character->HasSpell(25294) && character->HasSpell(2643)) character->learnSpell(25294, false); //Залп Уровень 5
            if (level >= 60 && !character->HasSpell(25295) && character->HasSpell(1978)) character->learnSpell(25295, false); //Укус змеи Уровень 9
            if (level >= 60 && !character->HasSpell(25296) && character->HasSpell(13165)) character->learnSpell(25296, false); //Дух ястреба Уровень 7
            if (level >= 60 && !character->HasSpell(24133) && character->HasSpell(19386)) character->learnSpell(24133, false); //Укус виверны Уровень 3
            if (level >= 60 && !character->HasSpell(20904) && character->HasSpell(19434)) character->learnSpell(20904, false); //Прицельный выстрел Уровень 6
            if (level >= 60 && !character->HasSpell(14311) && character->HasSpell(1499)) character->learnSpell(14311, false); //Замораживающая ловушка Уровень 3
            if (level >= 60 && !character->HasSpell(13544) && character->HasSpell(136)) character->learnSpell(13544, false); //Лечение питомца Уровень 7
            if (level >= 60 && !character->HasSpell(14287) && character->HasSpell(3044)) character->learnSpell(14287, false); //Чародейский выстрел Уровень 8
            if (level >= 60 && !character->HasSpell(19263)) character->learnSpell(19263, false); //Сдерживание 
            if (level >= 60 && !character->HasSpell(19801)) character->learnSpell(19801, false); //Усмиряющий выстрел 
            if (level >= 61 && !character->HasSpell(27025) && character->HasSpell(13813)) character->learnSpell(27025, false); //Взрывная ловушка Уровень 4
            if (level >= 62 && !character->HasSpell(34120) && character->HasSpell(56641)) character->learnSpell(34120, false); //Верный выстрел Уровень 2
            if (level >= 63 && !character->HasSpell(27014) && character->HasSpell(2973)) character->learnSpell(27014, false); //Удар ящера Уровень 9
            if (level >= 63 && !character->HasSpell(63669) && character->HasSpell(3674)) character->learnSpell(63669, false); //Черная стрела Уровень 3
            if (level >= 65 && !character->HasSpell(27023) && character->HasSpell(13795)) character->learnSpell(27023, false); //Обжигающая ловушка Уровень 6
            if (level >= 66 && !character->HasSpell(27067) && character->HasSpell(19306)) character->learnSpell(27067, false); //Контратака Уровень 4
            if (level >= 66 && !character->HasSpell(34026)) character->learnSpell(34026, false); //Команда "Взять!" 
            if (level >= 67 && !character->HasSpell(27022) && character->HasSpell(1510)) character->learnSpell(27022, false); //Град стрел Уровень 4
            if (level >= 67 && !character->HasSpell(27021) && character->HasSpell(2643)) character->learnSpell(27021, false); //Залп Уровень 6
            if (level >= 67 && !character->HasSpell(27016) && character->HasSpell(1978)) character->learnSpell(27016, false); //Укус змеи Уровень 10
            if (level >= 68 && !character->HasSpell(34600)) character->learnSpell(34600, false); //Змеиная ловушка 
            if (level >= 68 && !character->HasSpell(27046) && character->HasSpell(136)) character->learnSpell(27046, false); //Лечение питомца Уровень 8
            if (level >= 68 && !character->HasSpell(27045) && character->HasSpell(20043)) character->learnSpell(27045, false); //Дух дикой природы Уровень 3
            if (level >= 68 && !character->HasSpell(27044) && character->HasSpell(13165)) character->learnSpell(27044, false); //Дух ястреба Уровень 8
            if (level >= 69 && !character->HasSpell(27019) && character->HasSpell(3044)) character->learnSpell(27019, false); //Чародейский выстрел Уровень 9
            if (level >= 69 && !character->HasSpell(63670) && character->HasSpell(3674)) character->learnSpell(63670, false); //Черная стрела Уровень 4
            if (level >= 70 && !character->HasSpell(60051) && character->HasSpell(53301)) character->learnSpell(60051, false); //Разрывной выстрел Уровень 2
            if (level >= 70 && !character->HasSpell(34477)) character->learnSpell(34477, false); //Перенаправление 
            if (level >= 70 && !character->HasSpell(27068) && character->HasSpell(19386)) character->learnSpell(27068, false); //Укус виверны Уровень 4
            if (level >= 70 && !character->HasSpell(27065) && character->HasSpell(19434)) character->learnSpell(27065, false); //Прицельный выстрел Уровень 7
            if (level >= 70 && !character->HasSpell(36916) && character->HasSpell(1495)) character->learnSpell(36916, false); //Укус мангуста Уровень 5
            if (level >= 71 && !character->HasSpell(53351)) character->learnSpell(53351, false); //Убийственный выстрел Уровень 1
            if (level >= 71 && !character->HasSpell(49066) && character->HasSpell(13813)) character->learnSpell(49066, false); // unk
            if (level >= 71 && !character->HasSpell(49051) && character->HasSpell(56641)) character->learnSpell(49051, false); // unk
            if (level >= 71 && !character->HasSpell(48995) && character->HasSpell(2973)) character->learnSpell(48995, false); // unk
            if (level >= 72 && !character->HasSpell(49055) && character->HasSpell(13795)) character->learnSpell(49055, false); // unk
            if (level >= 72 && !character->HasSpell(48998) && character->HasSpell(19306)) character->learnSpell(48998, false); // unk
            if (level >= 73 && !character->HasSpell(49000) && character->HasSpell(1978)) character->learnSpell(49000, false); // unk
            if (level >= 73 && !character->HasSpell(49044) && character->HasSpell(3044)) character->learnSpell(49044, false); // unk
            if (level >= 74 && !character->HasSpell(49047) && character->HasSpell(2643)) character->learnSpell(49047, false); // unk
            if (level >= 74 && !character->HasSpell(61846)) character->learnSpell(61846, false); //Дух дракондора Уровень 1
            if (level >= 74 && !character->HasSpell(48989) && character->HasSpell(136)) character->learnSpell(48989, false); // unk
            if (level >= 74 && !character->HasSpell(58431) && character->HasSpell(1510)) character->learnSpell(58431, false); //Град стрел Уровень 5
            if (level >= 75 && !character->HasSpell(61005) && character->HasSpell(53351)) character->learnSpell(61005, false); //Убийственный выстрел Уровень 2
            if (level >= 75 && !character->HasSpell(63671) && character->HasSpell(3674)) character->learnSpell(63671, false); //Черная стрела Уровень 5
            if (level >= 75 && !character->HasSpell(60052) && character->HasSpell(53301)) character->learnSpell(60052, false); //Разрывной выстрел Уровень 3
            if (level >= 75 && !character->HasSpell(53271)) character->learnSpell(53271, false); //Приказ хозяина 
            if (level >= 75 && !character->HasSpell(49049) && character->HasSpell(19434)) character->learnSpell(49049, false); // unk
            if (level >= 75 && !character->HasSpell(49011) && character->HasSpell(19386)) character->learnSpell(49011, false); // unk
            if (level >= 76 && !character->HasSpell(49071) && character->HasSpell(20043)) character->learnSpell(49071, false); // unk
            if (level >= 76 && !character->HasSpell(53338) && character->HasSpell(1130)) character->learnSpell(53338, false); //Метка охотника Уровень 5
            if (level >= 77 && !character->HasSpell(48996) && character->HasSpell(2973)) character->learnSpell(48996, false); // unk
            if (level >= 77 && !character->HasSpell(49052) && character->HasSpell(56641)) character->learnSpell(49052, false); // unk
            if (level >= 77 && !character->HasSpell(49067) && character->HasSpell(13813)) character->learnSpell(49067, false); // unk
            if (level >= 78 && !character->HasSpell(48999) && character->HasSpell(19306)) character->learnSpell(48999, false); // unk
            if (level >= 78 && !character->HasSpell(49056) && character->HasSpell(13795)) character->learnSpell(49056, false); // unk
            if (level >= 79 && !character->HasSpell(49001) && character->HasSpell(1978)) character->learnSpell(49001, false); // unk
            if (level >= 79 && !character->HasSpell(49045) && character->HasSpell(3044)) character->learnSpell(49045, false); // unk
            if (level >= 80 && !character->HasSpell(53339) && character->HasSpell(1495)) character->learnSpell(53339, false); //Укус мангуста Уровень 6
            if (level >= 80 && !character->HasSpell(49012) && character->HasSpell(19386)) character->learnSpell(49012, false); // unk
            if (level >= 80 && !character->HasSpell(62757)) character->learnSpell(62757, false); //Призыв питомца из стойл 
            if (level >= 80 && !character->HasSpell(61847) && character->HasSpell(61846)) character->learnSpell(61847, false); //Дух дракондора Уровень 2
            if (level >= 80 && !character->HasSpell(49048) && character->HasSpell(2643)) character->learnSpell(49048, false); // unk
            if (level >= 80 && !character->HasSpell(61006) && character->HasSpell(53351)) character->learnSpell(61006, false); //Убийственный выстрел Уровень 3
            if (level >= 80 && !character->HasSpell(49050) && character->HasSpell(19434)) character->learnSpell(49050, false); // unk
            if (level >= 80 && !character->HasSpell(60192)) character->learnSpell(60192, false); //Замораживающая стрела Уровень 1
            if (level >= 80 && !character->HasSpell(60053) && character->HasSpell(53301)) character->learnSpell(60053, false); //Разрывной выстрел Уровень 4
            if (level >= 80 && !character->HasSpell(48990) && character->HasSpell(136)) character->learnSpell(48990, false); // unk
            if (level >= 80 && !character->HasSpell(58434) && character->HasSpell(1510)) character->learnSpell(58434, false); //Град стрел Уровень 6
            if (level >= 80 && !character->HasSpell(63672) && character->HasSpell(3674)) character->learnSpell(63672, false); //Черная стрела Уровень 6
    }

    void learnSpellShaman()
    {

            if (level >= 1 && !character->HasSpell(8017)) character->learnSpell(8017, false); //Оружие камнедробителя Уровень 1
            if (level >= 4 && !character->HasSpell(8042)) character->learnSpell(8042, false); //Земной шок Уровень 1
            if (level >= 6 && !character->HasSpell(332) && character->HasSpell(331)) character->learnSpell(332, false); //Волна исцеления Уровень 2
            if (level >= 6 && !character->HasSpell(2484)) character->learnSpell(2484, false); //Тотем оков земли 
            if (level >= 8 && !character->HasSpell(5730)) character->learnSpell(5730, false); //Тотем каменного когтя Уровень 1
            if (level >= 8 && !character->HasSpell(8044) && character->HasSpell(8042)) character->learnSpell(8044, false); //Земной шок Уровень 2
            if (level >= 8 && !character->HasSpell(529) && character->HasSpell(403)) character->learnSpell(529, false); //Молния Уровень 2
            if (level >= 8 && !character->HasSpell(8018) && character->HasSpell(8017)) character->learnSpell(8018, false); //Оружие камнедробителя Уровень 2
            if (level >= 8 && !character->HasSpell(324)) character->learnSpell(324, false); //Щит молний Уровень 1
            if (level >= 10 && !character->HasSpell(8050)) character->learnSpell(8050, false); //Огненный шок Уровень 1
            if (level >= 10 && !character->HasSpell(8024)) character->learnSpell(8024, false); //Оружие языка пламени Уровень 1
            if (level >= 10 && !character->HasSpell(8075)) character->learnSpell(8075, false); //Тотем силы земли Уровень 1
            if (level >= 12 && !character->HasSpell(1535)) character->learnSpell(1535, false); //Кольцо огня Уровень 1
            if (level >= 12 && !character->HasSpell(370)) character->learnSpell(370, false); //Развеивание магии Уровень 1
            if (level >= 12 && !character->HasSpell(547) && character->HasSpell(331)) character->learnSpell(547, false); //Волна исцеления Уровень 3
            if (level >= 12 && !character->HasSpell(2008)) character->learnSpell(2008, false); //Дух предков Уровень 1
            if (level >= 14 && !character->HasSpell(8045) && character->HasSpell(8042)) character->learnSpell(8045, false); //Земной шок Уровень 3
            if (level >= 14 && !character->HasSpell(8154) && character->HasSpell(8071)) character->learnSpell(8154, false); //Тотем каменной кожи Уровень 2
            if (level >= 14 && !character->HasSpell(548) && character->HasSpell(403)) character->learnSpell(548, false); //Молния Уровень 3
            if (level >= 16 && !character->HasSpell(2645)) character->learnSpell(2645, false); //Призрачный волк 
            if (level >= 16 && !character->HasSpell(8019) && character->HasSpell(8017)) character->learnSpell(8019, false); //Оружие камнедробителя Уровень 3
            if (level >= 16 && !character->HasSpell(57994)) character->learnSpell(57994, false); //Пронизывающий ветер 
            if (level >= 16 && !character->HasSpell(526)) character->learnSpell(526, false); //Оздоровление 
            if (level >= 16 && !character->HasSpell(325) && character->HasSpell(324)) character->learnSpell(325, false); //Щит молний Уровень 2
            if (level >= 18 && !character->HasSpell(6390) && character->HasSpell(5730)) character->learnSpell(6390, false); //Тотем каменного когтя Уровень 2
            if (level >= 18 && !character->HasSpell(913) && character->HasSpell(331)) character->learnSpell(913, false); //Волна исцеления Уровень 4
            if (level >= 18 && !character->HasSpell(8027) && character->HasSpell(8024)) character->learnSpell(8027, false); //Оружие языка пламени Уровень 2
            if (level >= 18 && !character->HasSpell(8143)) character->learnSpell(8143, false); //Тотем трепета 
            if (level >= 18 && !character->HasSpell(8052) && character->HasSpell(8050)) character->learnSpell(8052, false); //Огненный шок Уровень 2
            if (level >= 20 && !character->HasSpell(8004)) character->learnSpell(8004, false); //Малая волна исцеления Уровень 1
            if (level >= 20 && !character->HasSpell(915) && character->HasSpell(403)) character->learnSpell(915, false); //Молния Уровень 4
            if (level >= 20 && !character->HasSpell(8033)) character->learnSpell(8033, false); //Оружие ледяного клейма Уровень 1
            if (level >= 20 && !character->HasSpell(6363) && character->HasSpell(3599)) character->learnSpell(6363, false); //Опаляющий тотем Уровень 2
            if (level >= 20 && !character->HasSpell(52127)) character->learnSpell(52127, false); // unk
            if (level >= 20 && !character->HasSpell(8056)) character->learnSpell(8056, false); //Ледяной шок Уровень 1
            if (level >= 22 && !character->HasSpell(131)) character->learnSpell(131, false); //Подводное дыхание 
            if (level >= 22 && !character->HasSpell(8498) && character->HasSpell(1535)) character->learnSpell(8498, false); //Кольцо огня Уровень 2
            if (level >= 24 && !character->HasSpell(8046) && character->HasSpell(8042)) character->learnSpell(8046, false); //Земной шок Уровень 4
            if (level >= 24 && !character->HasSpell(905) && character->HasSpell(324)) character->learnSpell(905, false); //Щит молний Уровень 3
            if (level >= 24 && !character->HasSpell(8160) && character->HasSpell(8075)) character->learnSpell(8160, false); //Тотем силы земли Уровень 2
            if (level >= 24 && !character->HasSpell(939) && character->HasSpell(331)) character->learnSpell(939, false); //Волна исцеления Уровень 5
            if (level >= 24 && !character->HasSpell(8181)) character->learnSpell(8181, false); //Тотем защиты от магии льда Уровень 1
            if (level >= 24 && !character->HasSpell(8155) && character->HasSpell(8071)) character->learnSpell(8155, false); //Тотем каменной кожи Уровень 3
            if (level >= 24 && !character->HasSpell(10399) && character->HasSpell(8017)) character->learnSpell(10399, false); //Оружие камнедробителя Уровень 4
            if (level >= 24 && !character->HasSpell(20609) && character->HasSpell(2008)) character->learnSpell(20609, false); //Дух предков Уровень 2
            if (level >= 26 && !character->HasSpell(8030) && character->HasSpell(8024)) character->learnSpell(8030, false); //Оружие языка пламени Уровень 3
            if (level >= 26 && !character->HasSpell(8190)) character->learnSpell(8190, false); //Тотем магмы Уровень 1
            if (level >= 26 && !character->HasSpell(5675)) character->learnSpell(5675, false); //Тотем источника маны Уровень 1
            if (level >= 26 && !character->HasSpell(6196)) character->learnSpell(6196, false); //Дальнее зрение 
            if (level >= 26 && !character->HasSpell(943) && character->HasSpell(403)) character->learnSpell(943, false); //Молния Уровень 5
            if (level >= 28 && !character->HasSpell(8038) && character->HasSpell(8033)) character->learnSpell(8038, false); //Оружие ледяного клейма Уровень 2
            if (level >= 28 && !character->HasSpell(8053) && character->HasSpell(8050)) character->learnSpell(8053, false); //Огненный шок Уровень 3
            if (level >= 28 && !character->HasSpell(546)) character->learnSpell(546, false); //Хождение по воде 
            if (level >= 28 && !character->HasSpell(52129) && character->HasSpell(52127)) character->learnSpell(52129, false); // unk
            if (level >= 28 && !character->HasSpell(8227)) character->learnSpell(8227, false); //Тотем языка пламени Уровень 1
            if (level >= 28 && !character->HasSpell(6391) && character->HasSpell(5730)) character->learnSpell(6391, false); //Тотем каменного когтя Уровень 3
            if (level >= 28 && !character->HasSpell(8008) && character->HasSpell(8004)) character->learnSpell(8008, false); //Малая волна исцеления Уровень 2
            if (level >= 28 && !character->HasSpell(8184)) character->learnSpell(8184, false); //Тотем защиты от огня Уровень 1
            if (level >= 30 && !character->HasSpell(8232)) character->learnSpell(8232, false); //Оружие неистовства ветра Уровень 1
            if (level >= 30 && !character->HasSpell(66842)) character->learnSpell(66842, false); //Зов Стихий 
            if (level >= 30 && !character->HasSpell(8177)) character->learnSpell(8177, false); //Тотем заземления 
            if (level >= 30 && !character->HasSpell(556)) character->learnSpell(556, false); //Астральное возвращение 
            if (level >= 30 && !character->HasSpell(51730)) character->learnSpell(51730, false); // unk
            if (level >= 30 && !character->HasSpell(36936)) character->learnSpell(36936, false); //Возвращение тотемов 
            if (level >= 30 && !character->HasSpell(6375) && character->HasSpell(5394)) character->learnSpell(6375, false); //Тотем исцеляющего потока Уровень 2
            if (level >= 30 && !character->HasSpell(10595)) character->learnSpell(10595, false); //Тотем защиты от сил природы Уровень 1
            if (level >= 30 && !character->HasSpell(20608)) character->learnSpell(20608, false); //Перерождение Пассивная
            if (level >= 30 && !character->HasSpell(6364) && character->HasSpell(3599)) character->learnSpell(6364, false); //Опаляющий тотем Уровень 3
            if (level >= 32 && !character->HasSpell(8512)) character->learnSpell(8512, false); //Тотем неистовства ветра 
            if (level >= 32 && !character->HasSpell(6041) && character->HasSpell(403)) character->learnSpell(6041, false); //Молния Уровень 6
            if (level >= 32 && !character->HasSpell(421)) character->learnSpell(421, false); //Цепная молния Уровень 1
            if (level >= 32 && !character->HasSpell(8012) && character->HasSpell(370)) character->learnSpell(8012, false); //Развеивание магии Уровень 2
            if (level >= 32 && !character->HasSpell(959) && character->HasSpell(331)) character->learnSpell(959, false); //Волна исцеления Уровень 6
            if (level >= 32 && !character->HasSpell(945) && character->HasSpell(324)) character->learnSpell(945, false); //Щит молний Уровень 4
            if (level >= 32 && !character->HasSpell(8499) && character->HasSpell(1535)) character->learnSpell(8499, false); //Кольцо огня Уровень 3
            if (level >= 34 && !character->HasSpell(8058) && character->HasSpell(8056)) character->learnSpell(8058, false); //Ледяной шок Уровень 2
            if (level >= 34 && !character->HasSpell(10406) && character->HasSpell(8071)) character->learnSpell(10406, false); //Тотем каменной кожи Уровень 4
            if (level >= 34 && !character->HasSpell(6495)) character->learnSpell(6495, false); //Сторожевой тотем 
            if (level >= 34 && !character->HasSpell(52131) && character->HasSpell(52127)) character->learnSpell(52131, false); // unk
            if (level >= 36 && !character->HasSpell(10495) && character->HasSpell(5675)) character->learnSpell(10495, false); //Тотем источника маны Уровень 2
            if (level >= 36 && !character->HasSpell(10412) && character->HasSpell(8042)) character->learnSpell(10412, false); //Земной шок Уровень 5
            if (level >= 36 && !character->HasSpell(10585) && character->HasSpell(8190)) character->learnSpell(10585, false); //Тотем магмы Уровень 2
            if (level >= 36 && !character->HasSpell(20610) && character->HasSpell(2008)) character->learnSpell(20610, false); //Дух предков Уровень 3
            if (level >= 36 && !character->HasSpell(16339) && character->HasSpell(8024)) character->learnSpell(16339, false); //Оружие языка пламени Уровень 4
            if (level >= 36 && !character->HasSpell(8010) && character->HasSpell(8004)) character->learnSpell(8010, false); //Малая волна исцеления Уровень 3
            if (level >= 38 && !character->HasSpell(8249) && character->HasSpell(8227)) character->learnSpell(8249, false); //Тотем языка пламени Уровень 2
            if (level >= 38 && !character->HasSpell(6392) && character->HasSpell(5730)) character->learnSpell(6392, false); //Тотем каменного когтя Уровень 4
            if (level >= 38 && !character->HasSpell(10456) && character->HasSpell(8033)) character->learnSpell(10456, false); //Оружие ледяного клейма Уровень 3
            if (level >= 38 && !character->HasSpell(8170)) character->learnSpell(8170, false); //Тотем очищения 
            if (level >= 38 && !character->HasSpell(8161) && character->HasSpell(8075)) character->learnSpell(8161, false); //Тотем силы земли Уровень 3
            if (level >= 38 && !character->HasSpell(10478) && character->HasSpell(8181)) character->learnSpell(10478, false); //Тотем защиты от магии льда Уровень 2
            if (level >= 38 && !character->HasSpell(10391) && character->HasSpell(403)) character->learnSpell(10391, false); //Молния Уровень 7
            if (level >= 40 && !character->HasSpell(51988) && character->HasSpell(51730)) character->learnSpell(51988, false); // unk
            if (level >= 40 && !character->HasSpell(8737)) character->learnSpell(8737, false); //Кольчужные доспехи 
            if (level >= 40 && !character->HasSpell(10447) && character->HasSpell(8050)) character->learnSpell(10447, false); //Огненный шок Уровень 4
            if (level >= 40 && !character->HasSpell(930) && character->HasSpell(421)) character->learnSpell(930, false); //Цепная молния Уровень 2
            if (level >= 40 && !character->HasSpell(8005) && character->HasSpell(331)) character->learnSpell(8005, false); //Волна исцеления Уровень 7
            if (level >= 40 && !character->HasSpell(66843)) character->learnSpell(66843, false); //Зов Предков 
            if (level >= 40 && !character->HasSpell(1064)) character->learnSpell(1064, false); //Цепное исцеление Уровень 1
            if (level >= 40 && !character->HasSpell(6365) && character->HasSpell(3599)) character->learnSpell(6365, false); //Опаляющий тотем Уровень 4
            if (level >= 40 && !character->HasSpell(8134) && character->HasSpell(324)) character->learnSpell(8134, false); //Щит молний Уровень 5
            if (level >= 40 && !character->HasSpell(6377) && character->HasSpell(5394)) character->learnSpell(6377, false); //Тотем исцеляющего потока Уровень 3
            if (level >= 40 && !character->HasSpell(8235) && character->HasSpell(8232)) character->learnSpell(8235, false); //Оружие неистовства ветра Уровень 2
            if (level >= 41 && !character->HasSpell(52134) && character->HasSpell(52127)) character->learnSpell(52134, false); // unk
            if (level >= 42 && !character->HasSpell(10537) && character->HasSpell(8184)) character->learnSpell(10537, false); //Тотем защиты от огня Уровень 2
            if (level >= 42 && !character->HasSpell(11314) && character->HasSpell(1535)) character->learnSpell(11314, false); //Кольцо огня Уровень 4
            if (level >= 44 && !character->HasSpell(10600) && character->HasSpell(10595)) character->learnSpell(10600, false); //Тотем защиты от сил природы Уровень 2
            if (level >= 44 && !character->HasSpell(10392) && character->HasSpell(403)) character->learnSpell(10392, false); //Молния Уровень 8
            if (level >= 44 && !character->HasSpell(10466) && character->HasSpell(8004)) character->learnSpell(10466, false); //Малая волна исцеления Уровень 4
            if (level >= 44 && !character->HasSpell(10407) && character->HasSpell(8071)) character->learnSpell(10407, false); //Тотем каменной кожи Уровень 5
            if (level >= 46 && !character->HasSpell(10622) && character->HasSpell(1064)) character->learnSpell(10622, false); //Цепное исцеление Уровень 2
            if (level >= 46 && !character->HasSpell(10496) && character->HasSpell(5675)) character->learnSpell(10496, false); //Тотем источника маны Уровень 3
            if (level >= 46 && !character->HasSpell(10586) && character->HasSpell(8190)) character->learnSpell(10586, false); //Тотем магмы Уровень 3
            if (level >= 46 && !character->HasSpell(10472) && character->HasSpell(8056)) character->learnSpell(10472, false); //Ледяной шок Уровень 3
            if (level >= 46 && !character->HasSpell(16341) && character->HasSpell(8024)) character->learnSpell(16341, false); //Оружие языка пламени Уровень 5
            if (level >= 48 && !character->HasSpell(10395) && character->HasSpell(331)) character->learnSpell(10395, false); //Волна исцеления Уровень 8
            if (level >= 48 && !character->HasSpell(52136) && character->HasSpell(52127)) character->learnSpell(52136, false); // unk
            if (level >= 48 && !character->HasSpell(10413) && character->HasSpell(8042)) character->learnSpell(10413, false); //Земной шок Уровень 6
            if (level >= 48 && !character->HasSpell(2860) && character->HasSpell(421)) character->learnSpell(2860, false); //Цепная молния Уровень 3
            if (level >= 48 && !character->HasSpell(10427) && character->HasSpell(5730)) character->learnSpell(10427, false); //Тотем каменного когтя Уровень 5
            if (level >= 48 && !character->HasSpell(10431) && character->HasSpell(324)) character->learnSpell(10431, false); //Щит молний Уровень 6
            if (level >= 48 && !character->HasSpell(10526) && character->HasSpell(8227)) character->learnSpell(10526, false); //Тотем языка пламени Уровень 3
            if (level >= 48 && !character->HasSpell(16355) && character->HasSpell(8033)) character->learnSpell(16355, false); //Оружие ледяного клейма Уровень 4
            if (level >= 48 && !character->HasSpell(20776) && character->HasSpell(2008)) character->learnSpell(20776, false); //Дух предков Уровень 4
            if (level >= 50 && !character->HasSpell(51991) && character->HasSpell(51730)) character->learnSpell(51991, false); // unk
            if (level >= 50 && !character->HasSpell(15207) && character->HasSpell(403)) character->learnSpell(15207, false); //Молния Уровень 9
            if (level >= 50 && !character->HasSpell(10486) && character->HasSpell(8232)) character->learnSpell(10486, false); //Оружие неистовства ветра Уровень 3
            if (level >= 50 && !character->HasSpell(66844)) character->learnSpell(66844, false); //Зов Духов 
            if (level >= 50 && !character->HasSpell(10462) && character->HasSpell(5394)) character->learnSpell(10462, false); //Тотем исцеляющего потока Уровень 4
            if (level >= 50 && !character->HasSpell(10437) && character->HasSpell(3599)) character->learnSpell(10437, false); //Опаляющий тотем Уровень 5
            if (level >= 52 && !character->HasSpell(11315) && character->HasSpell(1535)) character->learnSpell(11315, false); //Кольцо огня Уровень 5
            if (level >= 52 && !character->HasSpell(10448) && character->HasSpell(8050)) character->learnSpell(10448, false); //Огненный шок Уровень 5
            if (level >= 52 && !character->HasSpell(10467) && character->HasSpell(8004)) character->learnSpell(10467, false); //Малая волна исцеления Уровень 5
            if (level >= 52 && !character->HasSpell(10442) && character->HasSpell(8075)) character->learnSpell(10442, false); //Тотем силы земли Уровень 4
            if (level >= 54 && !character->HasSpell(10623) && character->HasSpell(1064)) character->learnSpell(10623, false); //Цепное исцеление Уровень 3
            if (level >= 54 && !character->HasSpell(10408) && character->HasSpell(8071)) character->learnSpell(10408, false); //Тотем каменной кожи Уровень 6
            if (level >= 54 && !character->HasSpell(10479) && character->HasSpell(8181)) character->learnSpell(10479, false); //Тотем защиты от магии льда Уровень 3
            if (level >= 55 && !character->HasSpell(52138) && character->HasSpell(52127)) character->learnSpell(52138, false); // unk
            if (level >= 56 && !character->HasSpell(10605) && character->HasSpell(421)) character->learnSpell(10605, false); //Цепная молния Уровень 4
            if (level >= 56 && !character->HasSpell(10396) && character->HasSpell(331)) character->learnSpell(10396, false); //Волна исцеления Уровень 9
            if (level >= 56 && !character->HasSpell(10497) && character->HasSpell(5675)) character->learnSpell(10497, false); //Тотем источника маны Уровень 4
            if (level >= 56 && !character->HasSpell(16342) && character->HasSpell(8024)) character->learnSpell(16342, false); //Оружие языка пламени Уровень 6
            if (level >= 56 && !character->HasSpell(10587) && character->HasSpell(8190)) character->learnSpell(10587, false); //Тотем магмы Уровень 4
            if (level >= 56 && !character->HasSpell(15208) && character->HasSpell(403)) character->learnSpell(15208, false); //Молния Уровень 10
            if (level >= 56 && !character->HasSpell(10432) && character->HasSpell(324)) character->learnSpell(10432, false); //Щит молний Уровень 7
            if (level >= 58 && !character->HasSpell(10538) && character->HasSpell(8184)) character->learnSpell(10538, false); //Тотем защиты от огня Уровень 3
            if (level >= 58 && !character->HasSpell(10428) && character->HasSpell(5730)) character->learnSpell(10428, false); //Тотем каменного когтя Уровень 6
            if (level >= 58 && !character->HasSpell(10473) && character->HasSpell(8056)) character->learnSpell(10473, false); //Ледяной шок Уровень 4
            if (level >= 58 && !character->HasSpell(16387) && character->HasSpell(8227)) character->learnSpell(16387, false); //Тотем языка пламени Уровень 4
            if (level >= 58 && !character->HasSpell(16356) && character->HasSpell(8033)) character->learnSpell(16356, false); //Оружие ледяного клейма Уровень 5
            if (level >= 60 && !character->HasSpell(51992) && character->HasSpell(51730)) character->learnSpell(51992, false); // unk
            if (level >= 60 && !character->HasSpell(10463) && character->HasSpell(5394)) character->learnSpell(10463, false); //Тотем исцеляющего потока Уровень 5
            if (level >= 60 && !character->HasSpell(32593) && character->HasSpell(974)) character->learnSpell(32593, false); //Щит земли Уровень 2
            if (level >= 60 && !character->HasSpell(10414) && character->HasSpell(8042)) character->learnSpell(10414, false); //Земной шок Уровень 7
            if (level >= 60 && !character->HasSpell(10438) && character->HasSpell(3599)) character->learnSpell(10438, false); //Опаляющий тотем Уровень 6
            if (level >= 60 && !character->HasSpell(29228) && character->HasSpell(8050)) character->learnSpell(29228, false); //Огненный шок Уровень 6
            if (level >= 60 && !character->HasSpell(57720) && character->HasSpell(30706)) character->learnSpell(57720, false); //Тотем гнева Уровень 2
            if (level >= 60 && !character->HasSpell(10601) && character->HasSpell(10595)) character->learnSpell(10601, false); //Тотем защиты от сил природы Уровень 3
            if (level >= 60 && !character->HasSpell(16362) && character->HasSpell(8232)) character->learnSpell(16362, false); //Оружие неистовства ветра Уровень 4
            if (level >= 60 && !character->HasSpell(10468) && character->HasSpell(8004)) character->learnSpell(10468, false); //Малая волна исцеления Уровень 6
            if (level >= 60 && !character->HasSpell(20777) && character->HasSpell(2008)) character->learnSpell(20777, false); //Дух предков Уровень 5
            if (level >= 60 && !character->HasSpell(25357) && character->HasSpell(331)) character->learnSpell(25357, false); //Волна исцеления Уровень 10
            if (level >= 60 && !character->HasSpell(25361) && character->HasSpell(8075)) character->learnSpell(25361, false); //Тотем силы земли Уровень 5
            if (level >= 61 && !character->HasSpell(25546) && character->HasSpell(1535)) character->learnSpell(25546, false); //Кольцо огня Уровень 6
            if (level >= 61 && !character->HasSpell(25422) && character->HasSpell(1064)) character->learnSpell(25422, false); //Цепное исцеление Уровень 4
            if (level >= 62 && !character->HasSpell(25448) && character->HasSpell(403)) character->learnSpell(25448, false); //Молния Уровень 11
            if (level >= 62 && !character->HasSpell(24398) && character->HasSpell(52127)) character->learnSpell(24398, false); //Водный щит Уровень 7
            if (level >= 63 && !character->HasSpell(25439) && character->HasSpell(421)) character->learnSpell(25439, false); //Цепная молния Уровень 5
            if (level >= 63 && !character->HasSpell(25508) && character->HasSpell(8071)) character->learnSpell(25508, false); //Тотем каменной кожи Уровень 7
            if (level >= 63 && !character->HasSpell(25469) && character->HasSpell(324)) character->learnSpell(25469, false); //Щит молний Уровень 8
            if (level >= 63 && !character->HasSpell(25391) && character->HasSpell(331)) character->learnSpell(25391, false); //Волна исцеления Уровень 11
            if (level >= 64 && !character->HasSpell(25489) && character->HasSpell(8024)) character->learnSpell(25489, false); //Оружие языка пламени Уровень 7
            if (level >= 64 && !character->HasSpell(3738)) character->learnSpell(3738, false); //Тотем гнева воздуха 
            if (level >= 65 && !character->HasSpell(25528) && character->HasSpell(8075)) character->learnSpell(25528, false); //Тотем силы земли Уровень 6
            if (level >= 65 && !character->HasSpell(25552) && character->HasSpell(8190)) character->learnSpell(25552, false); //Тотем магмы Уровень 5
            if (level >= 65 && !character->HasSpell(25570) && character->HasSpell(5675)) character->learnSpell(25570, false); //Тотем источника маны Уровень 5
            if (level >= 66 && !character->HasSpell(2062)) character->learnSpell(2062, false); //Тотем элементаля земли 
            if (level >= 66 && !character->HasSpell(25500) && character->HasSpell(8033)) character->learnSpell(25500, false); //Оружие ледяного клейма Уровень 6
            if (level >= 66 && !character->HasSpell(25420) && character->HasSpell(8004)) character->learnSpell(25420, false); //Малая волна исцеления Уровень 7
            if (level >= 67 && !character->HasSpell(25449) && character->HasSpell(403)) character->learnSpell(25449, false); //Молния Уровень 12
            if (level >= 67 && !character->HasSpell(25560) && character->HasSpell(8181)) character->learnSpell(25560, false); //Тотем защиты от магии льда Уровень 4
            if (level >= 67 && !character->HasSpell(25525) && character->HasSpell(5730)) character->learnSpell(25525, false); //Тотем каменного когтя Уровень 7
            if (level >= 67 && !character->HasSpell(25557) && character->HasSpell(8227)) character->learnSpell(25557, false); //Тотем языка пламени Уровень 5
            if (level >= 68 && !character->HasSpell(25464) && character->HasSpell(8056)) character->learnSpell(25464, false); //Ледяной шок Уровень 5
            if (level >= 68 && !character->HasSpell(25505) && character->HasSpell(8232)) character->learnSpell(25505, false); //Оружие неистовства ветра Уровень 5
            if (level >= 68 && !character->HasSpell(25423) && character->HasSpell(1064)) character->learnSpell(25423, false); //Цепное исцеление Уровень 5
            if (level >= 68 && !character->HasSpell(25563) && character->HasSpell(8184)) character->learnSpell(25563, false); //Тотем защиты от огня Уровень 4
            if (level >= 68 && !character->HasSpell(2894)) character->learnSpell(2894, false); //Тотем элементаля огня 
            if (level >= 69 && !character->HasSpell(25574) && character->HasSpell(10595)) character->learnSpell(25574, false); //Тотем защиты от сил природы Уровень 4
            if (level >= 69 && !character->HasSpell(25454) && character->HasSpell(8042)) character->learnSpell(25454, false); //Земной шок Уровень 8
            if (level >= 69 && !character->HasSpell(25533) && character->HasSpell(3599)) character->learnSpell(25533, false); //Опаляющий тотем Уровень 7
            if (level >= 69 && !character->HasSpell(25590) && character->HasSpell(2008)) character->learnSpell(25590, false); //Дух предков Уровень 6
            if (level >= 69 && !character->HasSpell(25567) && character->HasSpell(5394)) character->learnSpell(25567, false); //Тотем исцеляющего потока Уровень 6
            if (level >= 69 && !character->HasSpell(33736) && character->HasSpell(52127)) character->learnSpell(33736, false); //Водный щит Уровень 8
            if (level >= 70 && !character->HasSpell(32594) && character->HasSpell(974)) character->learnSpell(32594, false); //Щит земли Уровень 3
            if (level >= 70 && !character->HasSpell(25547) && character->HasSpell(1535)) character->learnSpell(25547, false); //Кольцо огня Уровень 7
            if (character->GetTeam() == ALLIANCE && level >= 70 && !character->HasSpell(32182)) character->learnSpell(32182, false); //Героизм 
            if (level >= 70 && !character->HasSpell(51993) && character->HasSpell(51730)) character->learnSpell(51993, false); // unk
            if (level >= 70 && !character->HasSpell(25509) && character->HasSpell(8071)) character->learnSpell(25509, false); //Тотем каменной кожи Уровень 8
            if (level >= 70 && !character->HasSpell(25472) && character->HasSpell(324)) character->learnSpell(25472, false); //Щит молний Уровень 9
            if (level >= 70 && !character->HasSpell(59156) && character->HasSpell(51490)) character->learnSpell(59156, false); //Гром и молния Уровень 2
            if (character->GetTeam() == HORDE && level >= 70 && !character->HasSpell(2825)) character->learnSpell(2825, false); //Жажда крови 
            if (level >= 70 && !character->HasSpell(25396) && character->HasSpell(331)) character->learnSpell(25396, false); //Волна исцеления Уровень 12
            if (level >= 70 && !character->HasSpell(57721) && character->HasSpell(30706)) character->learnSpell(57721, false); //Тотем гнева Уровень 3
            if (level >= 70 && !character->HasSpell(61299) && character->HasSpell(61295)) character->learnSpell(61299, false); //Быстрина Уровень 2
            if (level >= 70 && !character->HasSpell(25442) && character->HasSpell(421)) character->learnSpell(25442, false); //Цепная молния Уровень 6
            if (level >= 70 && !character->HasSpell(25457) && character->HasSpell(8050)) character->learnSpell(25457, false); //Огненный шок Уровень 7
            if (level >= 71 && !character->HasSpell(58699) && character->HasSpell(3599)) character->learnSpell(58699, false); //Опаляющий тотем Уровень 8
            if (level >= 71 && !character->HasSpell(58580) && character->HasSpell(5730)) character->learnSpell(58580, false); //Тотем каменного когтя Уровень 8
            if (level >= 71 && !character->HasSpell(58649) && character->HasSpell(8227)) character->learnSpell(58649, false); //Тотем языка пламени Уровень 6
            if (level >= 71 && !character->HasSpell(58755) && character->HasSpell(5394)) character->learnSpell(58755, false); //Тотем исцеляющего потока Уровень 7
            if (level >= 71 && !character->HasSpell(58801) && character->HasSpell(8232)) character->learnSpell(58801, false); //Оружие неистовства ветра Уровень 6
            if (level >= 71 && !character->HasSpell(58771) && character->HasSpell(5675)) character->learnSpell(58771, false); //Тотем источника маны Уровень 6
            if (level >= 71 && !character->HasSpell(58794) && character->HasSpell(8033)) character->learnSpell(58794, false); //Оружие ледяного клейма Уровень 7
            if (level >= 71 && !character->HasSpell(58785) && character->HasSpell(8024)) character->learnSpell(58785, false); //Оружие языка пламени Уровень 8
            if (level >= 72 && !character->HasSpell(49275) && character->HasSpell(8004)) character->learnSpell(49275, false); // unk
            if (level >= 73 && !character->HasSpell(49235) && character->HasSpell(8056)) character->learnSpell(49235, false); // unk
            if (level >= 73 && !character->HasSpell(58751) && character->HasSpell(8071)) character->learnSpell(58751, false); //Тотем каменной кожи Уровень 9
            if (level >= 73 && !character->HasSpell(49237) && character->HasSpell(403)) character->learnSpell(49237, false); // unk
            if (level >= 73 && !character->HasSpell(58731) && character->HasSpell(8190)) character->learnSpell(58731, false); //Тотем магмы Уровень 6
            if (level >= 74 && !character->HasSpell(55458) && character->HasSpell(1064)) character->learnSpell(55458, false); //Цепное исцеление Уровень 6
            if (level >= 74 && !character->HasSpell(49230) && character->HasSpell(8042)) character->learnSpell(49230, false); // unk
            if (level >= 74 && !character->HasSpell(49270) && character->HasSpell(421)) character->learnSpell(49270, false); // unk
            if (level >= 75 && !character->HasSpell(61649) && character->HasSpell(1535)) character->learnSpell(61649, false); //Кольцо огня Уровень 8
            if (level >= 75 && !character->HasSpell(58652) && character->HasSpell(8227)) character->learnSpell(58652, false); //Тотем языка пламени Уровень 7
            if (level >= 75 && !character->HasSpell(58703) && character->HasSpell(3599)) character->learnSpell(58703, false); //Опаляющий тотем Уровень 9
            if (level >= 75 && !character->HasSpell(59158) && character->HasSpell(51490)) character->learnSpell(59158, false); //Гром и молния Уровень 3
            if (level >= 75 && !character->HasSpell(61300) && character->HasSpell(61295)) character->learnSpell(61300, false); //Быстрина Уровень 3
            if (level >= 75 && !character->HasSpell(58737) && character->HasSpell(8184)) character->learnSpell(58737, false); //Тотем защиты от огня Уровень 5
            if (level >= 75 && !character->HasSpell(58741) && character->HasSpell(8181)) character->learnSpell(58741, false); //Тотем защиты от магии льда Уровень 5
            if (level >= 75 && !character->HasSpell(58746) && character->HasSpell(10595)) character->learnSpell(58746, false); //Тотем защиты от сил природы Уровень 5
            if (level >= 75 && !character->HasSpell(49283) && character->HasSpell(974)) character->learnSpell(49283, false); // unk
            if (level >= 75 && !character->HasSpell(51505)) character->learnSpell(51505, false); // unk
            if (level >= 75 && !character->HasSpell(49280) && character->HasSpell(324)) character->learnSpell(49280, false); // unk
            if (level >= 75 && !character->HasSpell(49272) && character->HasSpell(331)) character->learnSpell(49272, false); // unk
            if (level >= 75 && !character->HasSpell(49232) && character->HasSpell(8050)) character->learnSpell(49232, false); // unk
            if (level >= 75 && !character->HasSpell(57622) && character->HasSpell(8075)) character->learnSpell(57622, false); //Тотем силы земли Уровень 7
            if (level >= 75 && !character->HasSpell(58581) && character->HasSpell(5730)) character->learnSpell(58581, false); //Тотем каменного когтя Уровень 9
            if (level >= 76 && !character->HasSpell(58789) && character->HasSpell(8024)) character->learnSpell(58789, false); //Оружие языка пламени Уровень 9
            if (level >= 76 && !character->HasSpell(58803) && character->HasSpell(8232)) character->learnSpell(58803, false); //Оружие неистовства ветра Уровень 7
            if (level >= 76 && !character->HasSpell(57960) && character->HasSpell(52127)) character->learnSpell(57960, false); //Водный щит Уровень 9
            if (level >= 76 && !character->HasSpell(58756) && character->HasSpell(5394)) character->learnSpell(58756, false); //Тотем исцеляющего потока Уровень 8
            if (level >= 76 && !character->HasSpell(58773) && character->HasSpell(5675)) character->learnSpell(58773, false); //Тотем источника маны Уровень 7
            if (level >= 76 && !character->HasSpell(58795) && character->HasSpell(8033)) character->learnSpell(58795, false); //Оружие ледяного клейма Уровень 8
            if (level >= 77 && !character->HasSpell(49276) && character->HasSpell(8004)) character->learnSpell(49276, false); // unk
            if (level >= 78 && !character->HasSpell(58753) && character->HasSpell(8071)) character->learnSpell(58753, false); //Тотем каменной кожи Уровень 10
            if (level >= 78 && !character->HasSpell(58734) && character->HasSpell(8190)) character->learnSpell(58734, false); //Тотем магмы Уровень 7
            if (level >= 78 && !character->HasSpell(58582) && character->HasSpell(5730)) character->learnSpell(58582, false); //Тотем каменного когтя Уровень 10
            if (level >= 78 && !character->HasSpell(49236) && character->HasSpell(8056)) character->learnSpell(49236, false); // unk
            if (level >= 79 && !character->HasSpell(49238) && character->HasSpell(403)) character->learnSpell(49238, false); // unk
            if (level >= 79 && !character->HasSpell(49231) && character->HasSpell(8042)) character->learnSpell(49231, false); // unk
            if (level >= 80 && !character->HasSpell(58790) && character->HasSpell(8024)) character->learnSpell(58790, false); //Оружие языка пламени Уровень 10
            if (level >= 80 && !character->HasSpell(49273) && character->HasSpell(331)) character->learnSpell(49273, false); // unk
            if (level >= 80 && !character->HasSpell(61657) && character->HasSpell(1535)) character->learnSpell(61657, false); //Кольцо огня Уровень 9
            if (level >= 80 && !character->HasSpell(58804) && character->HasSpell(8232)) character->learnSpell(58804, false); //Оружие неистовства ветра Уровень 8
            if (level >= 80 && !character->HasSpell(59159) && character->HasSpell(51490)) character->learnSpell(59159, false); //Гром и молния Уровень 4
            if (level >= 80 && !character->HasSpell(49233) && character->HasSpell(8050)) character->learnSpell(49233, false); // unk
            if (level >= 80 && !character->HasSpell(49271) && character->HasSpell(421)) character->learnSpell(49271, false); // unk
            if (level >= 80 && !character->HasSpell(61301) && character->HasSpell(61295)) character->learnSpell(61301, false); //Быстрина Уровень 4
            if (level >= 80 && !character->HasSpell(60043) && character->HasSpell(51505)) character->learnSpell(60043, false); //Выброс лавы Уровень 2
            if (level >= 80 && !character->HasSpell(58796) && character->HasSpell(8033)) character->learnSpell(58796, false); //Оружие ледяного клейма Уровень 9
            if (level >= 80 && !character->HasSpell(49281) && character->HasSpell(324)) character->learnSpell(49281, false); // unk
            if (level >= 80 && !character->HasSpell(58656) && character->HasSpell(8227)) character->learnSpell(58656, false); //Тотем языка пламени Уровень 8
            if (level >= 80 && !character->HasSpell(58704) && character->HasSpell(3599)) character->learnSpell(58704, false); //Опаляющий тотем Уровень 10
            if (level >= 80 && !character->HasSpell(55459) && character->HasSpell(1064)) character->learnSpell(55459, false); //Цепное исцеление Уровень 7
            if (level >= 80 && !character->HasSpell(51994) && character->HasSpell(51730)) character->learnSpell(51994, false); // unk
            if (level >= 80 && !character->HasSpell(58739) && character->HasSpell(8184)) character->learnSpell(58739, false); //Тотем защиты от огня Уровень 6
            if (level >= 80 && !character->HasSpell(51514)) character->learnSpell(51514, false); // unk
            if (level >= 80 && !character->HasSpell(58745) && character->HasSpell(8181)) character->learnSpell(58745, false); //Тотем защиты от магии льда Уровень 6
            if (level >= 80 && !character->HasSpell(49284) && character->HasSpell(974)) character->learnSpell(49284, false); // unk
            if (level >= 80 && !character->HasSpell(58749) && character->HasSpell(10595)) character->learnSpell(58749, false); //Тотем защиты от сил природы Уровень 6
            if (level >= 80 && !character->HasSpell(58757) && character->HasSpell(5394)) character->learnSpell(58757, false); //Тотем исцеляющего потока Уровень 9
            if (level >= 80 && !character->HasSpell(57722) && character->HasSpell(30706)) character->learnSpell(57722, false); //Тотем гнева Уровень 4
            if (level >= 80 && !character->HasSpell(58774) && character->HasSpell(5675)) character->learnSpell(58774, false); //Тотем источника маны Уровень 8
            if (level >= 80 && !character->HasSpell(49277) && character->HasSpell(2008)) character->learnSpell(49277, false); // unk
            if (level >= 80 && !character->HasSpell(58643) && character->HasSpell(8075)) character->learnSpell(58643, false); //Тотем силы земли Уровень 8
    }

    void learnSpellWarrior()
    {

            if (level >= 1 && !character->HasSpell(6673)) character->learnSpell(6673, false); //Боевой крик Уровень 1
            if (level >= 4 && !character->HasSpell(100)) character->learnSpell(100, false); //Рывок Уровень 1
            if (level >= 4 && !character->HasSpell(772)) character->learnSpell(772, false); //Кровопускание Уровень 1
            if (level >= 6 && !character->HasSpell(3127)) character->learnSpell(3127, false); //Парирование Пассивная
            if (level >= 6 && !character->HasSpell(6343)) character->learnSpell(6343, false); //Удар грома Уровень 1
            if (level >= 6 && !character->HasSpell(34428)) character->learnSpell(34428, false); //Победный раж 
            if (level >= 8 && !character->HasSpell(1715)) character->learnSpell(1715, false); //Подрезать сухожилия 
            if (level >= 8 && !character->HasSpell(284) && character->HasSpell(78)) character->learnSpell(284, false); //Удар героя Уровень 2
            if (level >= 10 && !character->HasSpell(71)) character->learnSpell(71, false);
            if (level >= 10 && !character->HasSpell(7386)) character->learnSpell(7386, false);
            if (level >= 10 && !character->HasSpell(355)) character->learnSpell(355, false);
            if (level >= 10 && !character->HasSpell(6546) && character->HasSpell(772)) character->learnSpell(6546, false); //Кровопускание Уровень 2
            if (level >= 10 && !character->HasSpell(2687)) character->learnSpell(2687, false); //Кровавая ярость 
            if (level >= 12 && !character->HasSpell(5242) && character->HasSpell(6673)) character->learnSpell(5242, false); //Боевой крик Уровень 2
            if (level >= 12 && !character->HasSpell(7384)) character->learnSpell(7384, false); //Превосходство 
            if (level >= 12 && !character->HasSpell(72)) character->learnSpell(72, false); //Удар щитом 
            if (level >= 14 && !character->HasSpell(6572)) character->learnSpell(6572, false); //Реванш Уровень 1
            if (level >= 14 && !character->HasSpell(1160)) character->learnSpell(1160, false); //Деморализующий крик Уровень 1
            if (level >= 16 && !character->HasSpell(2565)) character->learnSpell(2565, false); //Блок щитом 
            if (level >= 16 && !character->HasSpell(285) && character->HasSpell(78)) character->learnSpell(285, false); //Удар героя Уровень 3
            if (level >= 16 && !character->HasSpell(694)) character->learnSpell(694, false); //Дразнящий удар 
            if (level >= 18 && !character->HasSpell(676)) character->learnSpell(676, false); //Разоружение 
            if (level >= 18 && !character->HasSpell(8198) && character->HasSpell(6343)) character->learnSpell(8198, false); //Удар грома Уровень 2
            if (level >= 20 && !character->HasSpell(12678)) character->learnSpell(12678, false); //Знание боевых стоек Пассивная
            if (level >= 20 && !character->HasSpell(674)) character->learnSpell(674, false); //Бой двумя оружиями Пассивная
            if (level >= 20 && !character->HasSpell(20230)) character->learnSpell(20230, false); //Возмездие 
            if (level >= 20 && !character->HasSpell(6547) && character->HasSpell(772)) character->learnSpell(6547, false); //Кровопускание Уровень 3
            if (level >= 20 && !character->HasSpell(845)) character->learnSpell(845, false); //Рассекающий удар Уровень 1
            if (level >= 22 && !character->HasSpell(6192) && character->HasSpell(6673)) character->learnSpell(6192, false); //Боевой крик Уровень 3
            if (level >= 22 && !character->HasSpell(5246)) character->learnSpell(5246, false); //Устрашающий крик 
            if (level >= 24 && !character->HasSpell(1608) && character->HasSpell(78)) character->learnSpell(1608, false); //Удар героя Уровень 4
            if (level >= 24 && !character->HasSpell(6190) && character->HasSpell(1160)) character->learnSpell(6190, false); //Деморализующий крик Уровень 2
            if (level >= 24 && !character->HasSpell(6574) && character->HasSpell(6572)) character->learnSpell(6574, false); //Реванш Уровень 2
            if (level >= 24 && !character->HasSpell(5308)) character->learnSpell(5308, false); //Казнь Уровень 1
            if (level >= 26 && !character->HasSpell(6178) && character->HasSpell(100)) character->learnSpell(6178, false); //Рывок Уровень 2
            if (level >= 26 && !character->HasSpell(1161)) character->learnSpell(1161, false); //Вызывающий крик 
            if (level >= 28 && !character->HasSpell(8204) && character->HasSpell(6343)) character->learnSpell(8204, false); //Удар грома Уровень 3
            if (level >= 28 && !character->HasSpell(871)) character->learnSpell(871, false); //Глухая оборона
            if (level >= 30 && !character->HasSpell(20252)) character->learnSpell(20252, false); //Перехват
            if (level >= 30 && !character->HasSpell(2458)) character->learnSpell(2458, false); //Стойка берсерка
            if (level >= 30 && !character->HasSpell(7369) && character->HasSpell(845)) character->learnSpell(7369, false); //Рассекающий удар Уровень 2
            if (level >= 30 && !character->HasSpell(20252)) character->learnSpell(20252, false); //Перехват 
            if (level >= 30 && !character->HasSpell(6548) && character->HasSpell(772)) character->learnSpell(6548, false); //Кровопускание Уровень 4
            if (level >= 30 && !character->HasSpell(1464)) character->learnSpell(1464, false); //Мощный удар Уровень 1
            if (level >= 32 && !character->HasSpell(18499)) character->learnSpell(18499, false); //Ярость берсерка 
            if (level >= 32 && !character->HasSpell(20658) && character->HasSpell(5308)) character->learnSpell(20658, false); //Казнь Уровень 2
            if (level >= 32 && !character->HasSpell(11549) && character->HasSpell(6673)) character->learnSpell(11549, false); //Боевой крик Уровень 4
            if (level >= 32 && !character->HasSpell(11564) && character->HasSpell(78)) character->learnSpell(11564, false); //Удар героя Уровень 5
            if (level >= 34 && !character->HasSpell(11554) && character->HasSpell(1160)) character->learnSpell(11554, false); //Деморализующий крик Уровень 3
            if (level >= 34 && !character->HasSpell(7379) && character->HasSpell(6572)) character->learnSpell(7379, false); //Реванш Уровень 3
            if (level >= 36 && !character->HasSpell(1680)) character->learnSpell(1680, false); //Вихрь 
            if (level >= 38 && !character->HasSpell(8820) && character->HasSpell(1464)) character->learnSpell(8820, false); //Мощный удар Уровень 2
            if (level >= 38 && !character->HasSpell(8205) && character->HasSpell(6343)) character->learnSpell(8205, false); //Удар грома Уровень 4
            if (level >= 38 && !character->HasSpell(6552)) character->learnSpell(6552, false); //Зуботычина 
            if (level >= 40 && !character->HasSpell(11572) && character->HasSpell(772)) character->learnSpell(11572, false); //Кровопускание Уровень 5
            if (level >= 40 && !character->HasSpell(23922)) character->learnSpell(23922, false); //Мощный удар щитом Уровень 1
            if (level >= 40 && !character->HasSpell(11565) && character->HasSpell(78)) character->learnSpell(11565, false); //Удар героя Уровень 6
            if (level >= 40 && !character->HasSpell(750)) character->learnSpell(750, false); //Латы 
            if (level >= 40 && !character->HasSpell(11608) && character->HasSpell(845)) character->learnSpell(11608, false); //Рассекающий удар Уровень 3
            if (level >= 40 && !character->HasSpell(20660) && character->HasSpell(5308)) character->learnSpell(20660, false); //Казнь Уровень 3
            if (level >= 42 && !character->HasSpell(11550) && character->HasSpell(6673)) character->learnSpell(11550, false); //Боевой крик Уровень 5
            if (level >= 44 && !character->HasSpell(11555) && character->HasSpell(1160)) character->learnSpell(11555, false); //Деморализующий крик Уровень 4
            if (level >= 44 && !character->HasSpell(11600) && character->HasSpell(6572)) character->learnSpell(11600, false); //Реванш Уровень 4
            if (level >= 46 && !character->HasSpell(11604) && character->HasSpell(1464)) character->learnSpell(11604, false); //Мощный удар Уровень 3
            if (level >= 46 && !character->HasSpell(11578) && character->HasSpell(100)) character->learnSpell(11578, false); //Рывок Уровень 3
            if (level >= 48 && !character->HasSpell(23923) && character->HasSpell(23922)) character->learnSpell(23923, false); //Мощный удар щитом Уровень 2
            if (level >= 48 && !character->HasSpell(20661) && character->HasSpell(5308)) character->learnSpell(20661, false); //Казнь Уровень 4
            if (level >= 48 && !character->HasSpell(21551) && character->HasSpell(12294)) character->learnSpell(21551, false); //Смертельный удар Уровень 2
            if (level >= 48 && !character->HasSpell(11566) && character->HasSpell(78)) character->learnSpell(11566, false); //Удар героя Уровень 7
            if (level >= 48 && !character->HasSpell(11580) && character->HasSpell(6343)) character->learnSpell(11580, false); //Удар грома Уровень 5
            if (level >= 50 && !character->HasSpell(11609) && character->HasSpell(845)) character->learnSpell(11609, false); //Рассекающий удар Уровень 4
            if (level >= 50 && !character->HasSpell(11573) && character->HasSpell(772)) character->learnSpell(11573, false); //Кровопускание Уровень 6
            if (level >= 50 && !character->HasSpell(1719)) character->learnSpell(1719, false); //Безрассудство 
            if (level >= 52 && !character->HasSpell(11551) && character->HasSpell(6673)) character->learnSpell(11551, false); //Боевой крик Уровень 6
            if (level >= 54 && !character->HasSpell(21552) && character->HasSpell(12294)) character->learnSpell(21552, false); //Смертельный удар Уровень 3
            if (level >= 54 && !character->HasSpell(23924) && character->HasSpell(23922)) character->learnSpell(23924, false); //Мощный удар щитом Уровень 3
            if (level >= 54 && !character->HasSpell(11556) && character->HasSpell(1160)) character->learnSpell(11556, false); //Деморализующий крик Уровень 5
            if (level >= 54 && !character->HasSpell(11601) && character->HasSpell(6572)) character->learnSpell(11601, false); //Реванш Уровень 5
            if (level >= 54 && !character->HasSpell(11605) && character->HasSpell(1464)) character->learnSpell(11605, false); //Мощный удар Уровень 4
            if (level >= 56 && !character->HasSpell(20662) && character->HasSpell(5308)) character->learnSpell(20662, false); //Казнь Уровень 5
            if (level >= 56 && !character->HasSpell(11567) && character->HasSpell(78)) character->learnSpell(11567, false); //Удар героя Уровень 8
            if (level >= 58 && !character->HasSpell(11581) && character->HasSpell(6343)) character->learnSpell(11581, false); //Удар грома Уровень 6
            if (level >= 60 && !character->HasSpell(23925) && character->HasSpell(23922)) character->learnSpell(23925, false); //Мощный удар щитом Уровень 4
            if (level >= 60 && !character->HasSpell(25286) && character->HasSpell(78)) character->learnSpell(25286, false); //Удар героя Уровень 9
            if (level >= 60 && !character->HasSpell(25288) && character->HasSpell(6572)) character->learnSpell(25288, false); //Реванш Уровень 6
            if (level >= 60 && !character->HasSpell(21553) && character->HasSpell(12294)) character->learnSpell(21553, false); //Смертельный удар Уровень 4
            if (level >= 60 && !character->HasSpell(11574) && character->HasSpell(772)) character->learnSpell(11574, false); //Кровопускание Уровень 7
            if (level >= 60 && !character->HasSpell(25289) && character->HasSpell(6673)) character->learnSpell(25289, false); //Боевой крик Уровень 7
            if (level >= 60 && !character->HasSpell(20569) && character->HasSpell(845)) character->learnSpell(20569, false); //Рассекающий удар Уровень 5
            if (level >= 60 && !character->HasSpell(30016) && character->HasSpell(20243)) character->learnSpell(30016, false); //Сокрушение Уровень 2
            if (level >= 61 && !character->HasSpell(25241) && character->HasSpell(1464)) character->learnSpell(25241, false); //Мощный удар Уровень 5
            if (level >= 62 && !character->HasSpell(25202) && character->HasSpell(1160)) character->learnSpell(25202, false); //Деморализующий крик Уровень 6
            if (level >= 63 && !character->HasSpell(25269) && character->HasSpell(6572)) character->learnSpell(25269, false); //Реванш Уровень 7
            if (level >= 64 && !character->HasSpell(23920)) character->learnSpell(23920, false); //Отражение заклинания 
            if (level >= 65 && !character->HasSpell(25234) && character->HasSpell(5308)) character->learnSpell(25234, false); //Казнь Уровень 6
            if (level >= 66 && !character->HasSpell(25258) && character->HasSpell(23922)) character->learnSpell(25258, false); //Мощный удар щитом Уровень 5
            if (level >= 66 && !character->HasSpell(29707) && character->HasSpell(78)) character->learnSpell(29707, false); //Удар героя Уровень 10
            if (level >= 66 && !character->HasSpell(25248) && character->HasSpell(12294)) character->learnSpell(25248, false); //Смертельный удар Уровень 5
            if (level >= 67 && !character->HasSpell(25264) && character->HasSpell(6343)) character->learnSpell(25264, false); //Удар грома Уровень 7
            if (level >= 68 && !character->HasSpell(469)) character->learnSpell(469, false); //Командирский крик Уровень 1
            if (level >= 68 && !character->HasSpell(25231) && character->HasSpell(845)) character->learnSpell(25231, false); //Рассекающий удар Уровень 6
            if (level >= 68 && !character->HasSpell(25208) && character->HasSpell(772)) character->learnSpell(25208, false); //Кровопускание Уровень 8
            if (level >= 69 && !character->HasSpell(25242) && character->HasSpell(1464)) character->learnSpell(25242, false); //Мощный удар Уровень 6
            if (level >= 69 && !character->HasSpell(2048) && character->HasSpell(6673)) character->learnSpell(2048, false); //Боевой крик Уровень 8
            if (level >= 70 && !character->HasSpell(30330) && character->HasSpell(12294)) character->learnSpell(30330, false); //Смертельный удар Уровень 6
            if (level >= 70 && !character->HasSpell(30356) && character->HasSpell(23922)) character->learnSpell(30356, false); //Мощный удар щитом Уровень 6
            if (level >= 70 && !character->HasSpell(30357) && character->HasSpell(6572)) character->learnSpell(30357, false); //Реванш Уровень 8
            if (level >= 70 && !character->HasSpell(30324) && character->HasSpell(78)) character->learnSpell(30324, false); //Удар героя Уровень 11
            if (level >= 70 && !character->HasSpell(30022) && character->HasSpell(20243)) character->learnSpell(30022, false); //Сокрушение Уровень 3
            if (level >= 70 && !character->HasSpell(25203) && character->HasSpell(1160)) character->learnSpell(25203, false); //Деморализующий крик Уровень 7
            if (level >= 70 && !character->HasSpell(25236) && character->HasSpell(5308)) character->learnSpell(25236, false); //Казнь Уровень 7
            if (level >= 70 && !character->HasSpell(3411)) character->learnSpell(3411, false); //Вмешательство 
            if (level >= 71 && !character->HasSpell(46845) && character->HasSpell(772)) character->learnSpell(46845, false); //Кровопускание Уровень 9
            if (level >= 71 && !character->HasSpell(64382)) character->learnSpell(64382, false); //Сокрушительный бросок 
            if (level >= 72 && !character->HasSpell(47449) && character->HasSpell(78)) character->learnSpell(47449, false); // unk
            if (level >= 72 && !character->HasSpell(47519) && character->HasSpell(845)) character->learnSpell(47519, false); // unk
            if (level >= 73 && !character->HasSpell(47470) && character->HasSpell(5308)) character->learnSpell(47470, false); // unk
            if (level >= 73 && !character->HasSpell(47501) && character->HasSpell(6343)) character->learnSpell(47501, false); // unk
            if (level >= 74 && !character->HasSpell(47439) && character->HasSpell(469)) character->learnSpell(47439, false); // unk
            if (level >= 74 && !character->HasSpell(47474) && character->HasSpell(1464)) character->learnSpell(47474, false); // unk
            if (level >= 75 && !character->HasSpell(55694)) character->learnSpell(55694, false); //Безудержное восстановление 
            if (level >= 75 && !character->HasSpell(47485) && character->HasSpell(12294)) character->learnSpell(47485, false); // unk
            if (level >= 75 && !character->HasSpell(47497) && character->HasSpell(20243)) character->learnSpell(47497, false); // unk
            if (level >= 75 && !character->HasSpell(47487) && character->HasSpell(23922)) character->learnSpell(47487, false); // unk
            if (level >= 76 && !character->HasSpell(47450) && character->HasSpell(78)) character->learnSpell(47450, false); // unk
            if (level >= 76 && !character->HasSpell(47465) && character->HasSpell(772)) character->learnSpell(47465, false); // unk
            if (level >= 77 && !character->HasSpell(47520) && character->HasSpell(845)) character->learnSpell(47520, false); // unk
            if (level >= 78 && !character->HasSpell(47502) && character->HasSpell(6343)) character->learnSpell(47502, false); // unk
            if (level >= 78 && !character->HasSpell(47436) && character->HasSpell(6673)) character->learnSpell(47436, false); // unk
            if (level >= 79 && !character->HasSpell(47475) && character->HasSpell(1464)) character->learnSpell(47475, false); // unk
            if (level >= 79 && !character->HasSpell(47437) && character->HasSpell(1160)) character->learnSpell(47437, false); // unk
            if (level >= 80 && !character->HasSpell(47498) && character->HasSpell(20243)) character->learnSpell(47498, false); // unk
            if (level >= 80 && !character->HasSpell(47488) && character->HasSpell(23922)) character->learnSpell(47488, false); // unk
            if (level >= 80 && !character->HasSpell(47486) && character->HasSpell(12294)) character->learnSpell(47486, false); // unk
            if (level >= 80 && !character->HasSpell(47440) && character->HasSpell(469)) character->learnSpell(47440, false); // unk
            if (level >= 80 && !character->HasSpell(57755)) character->learnSpell(57755, false); //Героический бросок 
            if (level >= 80 && !character->HasSpell(57823) && character->HasSpell(6572)) character->learnSpell(57823, false); //Реванш Уровень 9
            if (level >= 80 && !character->HasSpell(47471) && character->HasSpell(5308)) character->learnSpell(47471, false); // unk
    }

    void learnSpellPaladin()
    {

            if (level >= 1 && !character->HasSpell(465)) character->learnSpell(465, false); //Аура благочестия Уровень 1
            if (level >= 4 && !character->HasSpell(19740)) character->learnSpell(19740, false); //Благословение могущества Уровень 1
            if (level >= 4 && !character->HasSpell(20271)) character->learnSpell(20271, false); //Правосудие света 
            if (level >= 6 && !character->HasSpell(498)) character->learnSpell(498, false); //Божественная защита 
            if (level >= 6 && !character->HasSpell(639) && character->HasSpell(635)) character->learnSpell(639, false); //Свет небес Уровень 2
            if (level >= 8 && !character->HasSpell(853)) character->learnSpell(853, false); //Молот правосудия Уровень 1
            if (level >= 8 && !character->HasSpell(3127)) character->learnSpell(3127, false); //Парирование Пассивная
            if (level >= 8 && !character->HasSpell(1152)) character->learnSpell(1152, false); //Омовение 
            if (level >= 10 && !character->HasSpell(633)) character->learnSpell(633, false); //Возложение рук Уровень 1
            if (level >= 10 && !character->HasSpell(10290) && character->HasSpell(465)) character->learnSpell(10290, false); //Аура благочестия Уровень 2
            if (level >= 10 && !character->HasSpell(1022)) character->learnSpell(1022, false); //Длань защиты Уровень 1
            if (level >= 12 && !character->HasSpell(53408)) character->learnSpell(53408, false); //Правосудие мудрости 
            if (level >= 12 && !character->HasSpell(19834) && character->HasSpell(19740)) character->learnSpell(19834, false); //Благословение могущества Уровень 2
            if (level >= 14 && !character->HasSpell(19742)) character->learnSpell(19742, false); //Благословение мудрости Уровень 1
            if (level >= 14 && !character->HasSpell(31789)) character->learnSpell(31789, false); //Праведная защита 
            if (level >= 14 && !character->HasSpell(647) && character->HasSpell(635)) character->learnSpell(647, false); //Свет небес Уровень 3
            if (level >= 16 && !character->HasSpell(25780)) character->learnSpell(25780, false); //Праведное неистовство 
            if (level >= 16 && !character->HasSpell(7294)) character->learnSpell(7294, false); //Аура воздаяния Уровень 1
            if (level >= 16 && !character->HasSpell(62124)) character->learnSpell(62124, false); //Длань возмездия 
            if (level >= 18 && !character->HasSpell(1044)) character->learnSpell(1044, false); //Длань свободы 
            if (level >= 20 && !character->HasSpell(19750)) character->learnSpell(19750, false); //Вспышка Света Уровень 1
            if (level >= 20 && !character->HasSpell(5502)) character->learnSpell(5502, false); //Чутье на нежить 
            if (level >= 20 && !character->HasSpell(20217)) character->learnSpell(20217, false); //Благословение королей 
            if (level >= 20 && !character->HasSpell(643) && character->HasSpell(465)) character->learnSpell(643, false); //Аура благочестия Уровень 3
            if (level >= 20 && !character->HasSpell(26573)) character->learnSpell(26573, false); //Освящение Уровень 1
            if (level >= 20 && !character->HasSpell(879)) character->learnSpell(879, false); //Экзорцизм Уровень 1
            if (level >= 22 && !character->HasSpell(19835) && character->HasSpell(19740)) character->learnSpell(19835, false); //Благословение могущества Уровень 3
            if (level >= 22 && !character->HasSpell(20164)) character->learnSpell(20164, false); //Печать справедливости 
            if (level >= 22 && !character->HasSpell(1026) && character->HasSpell(635)) character->learnSpell(1026, false); //Свет небес Уровень 4
            if (level >= 22 && !character->HasSpell(19746)) character->learnSpell(19746, false); //Аура сосредоточенности 
            if (level >= 24 && !character->HasSpell(10322) && character->HasSpell(7328)) character->learnSpell(10322, false); //Искупление Уровень 2
            if (level >= 24 && !character->HasSpell(5599) && character->HasSpell(1022)) character->learnSpell(5599, false); //Длань защиты Уровень 2
            if (level >= 24 && !character->HasSpell(10326)) character->learnSpell(10326, false); //Изгнание зла 
            if (level >= 24 && !character->HasSpell(5588) && character->HasSpell(853)) character->learnSpell(5588, false); //Молот правосудия Уровень 2
            if (level >= 24 && !character->HasSpell(19850) && character->HasSpell(19742)) character->learnSpell(19850, false); //Благословение мудрости Уровень 2
            if (level >= 26 && !character->HasSpell(10298) && character->HasSpell(7294)) character->learnSpell(10298, false); //Аура воздаяния Уровень 2
            if (level >= 26 && !character->HasSpell(1038)) character->learnSpell(1038, false); //Длань спасения 
            if (level >= 26 && !character->HasSpell(19939) && character->HasSpell(19750)) character->learnSpell(19939, false); //Вспышка Света Уровень 2
            if (level >= 28 && !character->HasSpell(53407)) character->learnSpell(53407, false); //Правосудие справедливости 
            if (level >= 28 && !character->HasSpell(19876)) character->learnSpell(19876, false); //Аура защиты от темной магии Уровень 1
            if (level >= 28 && !character->HasSpell(5614) && character->HasSpell(879)) character->learnSpell(5614, false); //Экзорцизм Уровень 2
            if (level >= 30 && !character->HasSpell(19752)) character->learnSpell(19752, false); //Божественное вмешательство 
            if (level >= 30 && !character->HasSpell(20165)) character->learnSpell(20165, false); //Печать Света 
            if (level >= 30 && !character->HasSpell(2800) && character->HasSpell(633)) character->learnSpell(2800, false); //Возложение рук Уровень 2
            if (level >= 30 && !character->HasSpell(10291) && character->HasSpell(465)) character->learnSpell(10291, false); //Аура благочестия Уровень 4
            if (level >= 30 && !character->HasSpell(1042) && character->HasSpell(635)) character->learnSpell(1042, false); //Свет небес Уровень 5
            if (level >= 30 && !character->HasSpell(20116) && character->HasSpell(26573)) character->learnSpell(20116, false); //Освящение Уровень 2
            if (level >= 32 && !character->HasSpell(19836) && character->HasSpell(19740)) character->learnSpell(19836, false); //Благословение могущества Уровень 4
            if (level >= 32 && !character->HasSpell(19888)) character->learnSpell(19888, false); //Аура защиты от магии льда Уровень 1
            if (level >= 34 && !character->HasSpell(19852) && character->HasSpell(19742)) character->learnSpell(19852, false); //Благословение мудрости Уровень 3
            if (level >= 34 && !character->HasSpell(19940) && character->HasSpell(19750)) character->learnSpell(19940, false); //Вспышка Света Уровень 3
            if (level >= 34 && !character->HasSpell(642)) character->learnSpell(642, false); //Божественный щит 
            if (level >= 36 && !character->HasSpell(19891)) character->learnSpell(19891, false); //Аура защиты от огня Уровень 1
            if (level >= 36 && !character->HasSpell(5615) && character->HasSpell(879)) character->learnSpell(5615, false); //Экзорцизм Уровень 3
            if (level >= 36 && !character->HasSpell(10324) && character->HasSpell(7328)) character->learnSpell(10324, false); //Искупление Уровень 3
            if (level >= 36 && !character->HasSpell(10299) && character->HasSpell(7294)) character->learnSpell(10299, false); //Аура воздаяния Уровень 3
            if (level >= 38 && !character->HasSpell(10278) && character->HasSpell(1022)) character->learnSpell(10278, false); //Длань защиты Уровень 3
            if (level >= 38 && !character->HasSpell(20166)) character->learnSpell(20166, false); //Печать мудрости 
            if (level >= 38 && !character->HasSpell(3472) && character->HasSpell(635)) character->learnSpell(3472, false); //Свет небес Уровень 6
            if (level >= 40 && !character->HasSpell(19895) && character->HasSpell(19876)) character->learnSpell(19895, false); //Аура защиты от темной магии Уровень 2
            if (level >= 40 && !character->HasSpell(750)) character->learnSpell(750, false); //Латы 
            if (level >= 40 && !character->HasSpell(1032) && character->HasSpell(465)) character->learnSpell(1032, false); //Аура благочестия Уровень 5
            if (level >= 40 && !character->HasSpell(20922) && character->HasSpell(26573)) character->learnSpell(20922, false); //Освящение Уровень 3
            if (level >= 40 && !character->HasSpell(5589) && character->HasSpell(853)) character->learnSpell(5589, false); //Молот правосудия Уровень 3
            if (level >= 42 && !character->HasSpell(19941) && character->HasSpell(19750)) character->learnSpell(19941, false); //Вспышка Света Уровень 4
            if (level >= 42 && !character->HasSpell(19837) && character->HasSpell(19740)) character->learnSpell(19837, false); //Благословение могущества Уровень 5
            if (level >= 42 && !character->HasSpell(4987)) character->learnSpell(4987, false); //Очищение 
            if (level >= 44 && !character->HasSpell(24275)) character->learnSpell(24275, false); //Молот гнева Уровень 1
            if (level >= 44 && !character->HasSpell(19897) && character->HasSpell(19888)) character->learnSpell(19897, false); //Аура защиты от магии льда Уровень 2
            if (level >= 44 && !character->HasSpell(10312) && character->HasSpell(879)) character->learnSpell(10312, false); //Экзорцизм Уровень 4
            if (level >= 44 && !character->HasSpell(19853) && character->HasSpell(19742)) character->learnSpell(19853, false); //Благословение мудрости Уровень 4
            if (level >= 46 && !character->HasSpell(10300) && character->HasSpell(7294)) character->learnSpell(10300, false); //Аура воздаяния Уровень 4
            if (level >= 46 && !character->HasSpell(6940)) character->learnSpell(6940, false); //Длань жертвенности 
            if (level >= 46 && !character->HasSpell(10328) && character->HasSpell(635)) character->learnSpell(10328, false); //Свет небес Уровень 7
            if (level >= 48 && !character->HasSpell(19899) && character->HasSpell(19891)) character->learnSpell(19899, false); //Аура защиты от огня Уровень 2
            if (level >= 48 && !character->HasSpell(20929) && character->HasSpell(20473)) character->learnSpell(20929, false); //Шок небес Уровень 2
            if (level >= 48 && !character->HasSpell(20772) && character->HasSpell(7328)) character->learnSpell(20772, false); //Искупление Уровень 4
            if (level >= 50 && !character->HasSpell(20923) && character->HasSpell(26573)) character->learnSpell(20923, false); //Освящение Уровень 4
            if (level >= 50 && !character->HasSpell(20927) && character->HasSpell(20925)) character->learnSpell(20927, false); //Щит небес Уровень 2
            if (level >= 50 && !character->HasSpell(2812)) character->learnSpell(2812, false); //Гнев небес Уровень 1
            if (level >= 50 && !character->HasSpell(10292) && character->HasSpell(465)) character->learnSpell(10292, false); //Аура благочестия Уровень 6
            if (level >= 50 && !character->HasSpell(10310) && character->HasSpell(633)) character->learnSpell(10310, false); //Возложение рук Уровень 3
            if (level >= 50 && !character->HasSpell(19942) && character->HasSpell(19750)) character->learnSpell(19942, false); //Вспышка Света Уровень 5
            if (level >= 52 && !character->HasSpell(25782)) character->learnSpell(25782, false); //Великое благословение могущества Ур. 1
            if (level >= 52 && !character->HasSpell(19896) && character->HasSpell(19876)) character->learnSpell(19896, false); //Аура защиты от темной магии Уровень 3
            if (level >= 52 && !character->HasSpell(24274) && character->HasSpell(24275)) character->learnSpell(24274, false); //Молот гнева Уровень 2
            if (level >= 52 && !character->HasSpell(19838) && character->HasSpell(19740)) character->learnSpell(19838, false); //Благословение могущества Уровень 6
            if (level >= 52 && !character->HasSpell(10313) && character->HasSpell(879)) character->learnSpell(10313, false); //Экзорцизм Уровень 5
            if (level >= 54 && !character->HasSpell(10308) && character->HasSpell(853)) character->learnSpell(10308, false); //Молот правосудия Уровень 4
            if (level >= 54 && !character->HasSpell(25894)) character->learnSpell(25894, false); //Великое благословение мудрости Ур. 1
            if (level >= 54 && !character->HasSpell(10329) && character->HasSpell(635)) character->learnSpell(10329, false); //Свет небес Уровень 8
            if (level >= 54 && !character->HasSpell(19854) && character->HasSpell(19742)) character->learnSpell(19854, false); //Благословение мудрости Уровень 5
            if (level >= 56 && !character->HasSpell(20930) && character->HasSpell(20473)) character->learnSpell(20930, false); //Шок небес Уровень 3
            if (level >= 56 && !character->HasSpell(10301) && character->HasSpell(7294)) character->learnSpell(10301, false); //Аура воздаяния Уровень 5
            if (level >= 56 && !character->HasSpell(19898) && character->HasSpell(19888)) character->learnSpell(19898, false); //Аура защиты от магии льда Уровень 3
            if (level >= 58 && !character->HasSpell(19943) && character->HasSpell(19750)) character->learnSpell(19943, false); //Вспышка Света Уровень 6
            if (level >= 60 && !character->HasSpell(25898)) character->learnSpell(25898, false); //Великое благословение королей 
            if (level >= 60 && !character->HasSpell(25899)) character->learnSpell(25899, false); //Великое благословение неприкосновенности 
            if (level >= 60 && !character->HasSpell(25916) && character->HasSpell(25782)) character->learnSpell(25916, false); //Великое благословение могущества Ур. 2
            if (level >= 60 && !character->HasSpell(32699) && character->HasSpell(31935)) character->learnSpell(32699, false); //Щит мстителя Уровень 2
            if (level >= 60 && !character->HasSpell(25918) && character->HasSpell(25894)) character->learnSpell(25918, false); //Великое благословение мудрости Ур. 2
            if (level >= 60 && !character->HasSpell(10293) && character->HasSpell(465)) character->learnSpell(10293, false); //Аура благочестия Уровень 7
            if (level >= 60 && !character->HasSpell(25292) && character->HasSpell(635)) character->learnSpell(25292, false); //Свет небес Уровень 9
            if (level >= 60 && !character->HasSpell(10318) && character->HasSpell(2812)) character->learnSpell(10318, false); //Гнев небес Уровень 2
            if (level >= 60 && !character->HasSpell(20924) && character->HasSpell(26573)) character->learnSpell(20924, false); //Освящение Уровень 5
            if (level >= 60 && !character->HasSpell(20773) && character->HasSpell(7328)) character->learnSpell(20773, false); //Искупление Уровень 5
            if (level >= 60 && !character->HasSpell(19900) && character->HasSpell(19891)) character->learnSpell(19900, false); //Аура защиты от огня Уровень 3
            if (level >= 60 && !character->HasSpell(10314) && character->HasSpell(879)) character->learnSpell(10314, false); //Экзорцизм Уровень 6
            if (level >= 60 && !character->HasSpell(24239) && character->HasSpell(24275)) character->learnSpell(24239, false); //Молот гнева Уровень 3
            if (level >= 60 && !character->HasSpell(25290) && character->HasSpell(19742)) character->learnSpell(25290, false); //Благословение мудрости Уровень 6
            if (level >= 60 && !character->HasSpell(25291) && character->HasSpell(19740)) character->learnSpell(25291, false); //Благословение могущества Уровень 7
            if (level >= 60 && !character->HasSpell(20928) && character->HasSpell(20925)) character->learnSpell(20928, false); //Щит небес Уровень 3
            if (level >= 62 && !character->HasSpell(32223)) character->learnSpell(32223, false); //Аура воина Света 
            if (level >= 62 && !character->HasSpell(27135) && character->HasSpell(635)) character->learnSpell(27135, false); //Свет небес Уровень 10
            if (level >= 63 && !character->HasSpell(27151) && character->HasSpell(19876)) character->learnSpell(27151, false); //Аура защиты от темной магии Уровень 4
            if (level >= 64 && !character->HasSpell(27174) && character->HasSpell(20473)) character->learnSpell(27174, false); //Шок небес Уровень 4
            if (level >= 65 && !character->HasSpell(27142) && character->HasSpell(19742)) character->learnSpell(27142, false); //Благословение мудрости Уровень 7
            if (level >= 65 && !character->HasSpell(27143) && character->HasSpell(25894)) character->learnSpell(27143, false); //Великое благословение мудрости Ур. 3
            //if (level >= 66 && !character->HasSpell(53720)) character->learnSpell(53720, false); //Печать мученика 
            if (level >= 66 && !character->HasSpell(27137) && character->HasSpell(19750)) character->learnSpell(27137, false); //Вспышка Света Уровень 7
            if (level >= 66 && !character->HasSpell(27150) && character->HasSpell(7294)) character->learnSpell(27150, false); //Аура воздаяния Уровень 6
            if (level >= 68 && !character->HasSpell(27180) && character->HasSpell(24275)) character->learnSpell(27180, false); //Молот гнева Уровень 4
            if (level >= 68 && !character->HasSpell(27152) && character->HasSpell(19888)) character->learnSpell(27152, false); //Аура защиты от магии льда Уровень 4
            if (level >= 68 && !character->HasSpell(27138) && character->HasSpell(879)) character->learnSpell(27138, false); //Экзорцизм Уровень 7
            if (level >= 69 && !character->HasSpell(27139) && character->HasSpell(2812)) character->learnSpell(27139, false); //Гнев небес Уровень 3
            if (level >= 69 && !character->HasSpell(27154) && character->HasSpell(633)) character->learnSpell(27154, false); //Возложение рук Уровень 4
            if (level >= 70 && !character->HasSpell(32700) && character->HasSpell(31935)) character->learnSpell(32700, false); //Щит мстителя Уровень 3
            if (level >= 70 && !character->HasSpell(33072) && character->HasSpell(20473)) character->learnSpell(33072, false); //Шок небес Уровень 5
            if (level >= 70 && !character->HasSpell(31884)) character->learnSpell(31884, false); //Гнев карателя 
            if (level >= 70 && !character->HasSpell(27136) && character->HasSpell(635)) character->learnSpell(27136, false); //Свет небес Уровень 11
            if (level >= 70 && !character->HasSpell(27141) && character->HasSpell(25782)) character->learnSpell(27141, false); //Великое благословение могущества Ур. 3
            if (level >= 70 && !character->HasSpell(27140) && character->HasSpell(19740)) character->learnSpell(27140, false); //Благословение могущества Уровень 8
            if (level >= 70 && !character->HasSpell(27149) && character->HasSpell(465)) character->learnSpell(27149, false); //Аура благочестия Уровень 8
            if (level >= 70 && !character->HasSpell(27153) && character->HasSpell(19891)) character->learnSpell(27153, false); //Аура защиты от огня Уровень 4
            if (level >= 70 && !character->HasSpell(27173) && character->HasSpell(26573)) character->learnSpell(27173, false); //Освящение Уровень 6
            if (level >= 70 && !character->HasSpell(27179) && character->HasSpell(20925)) character->learnSpell(27179, false); //Щит небес Уровень 4
            if (level >= 71 && !character->HasSpell(48935) && character->HasSpell(19742)) character->learnSpell(48935, false); // unk
            if (level >= 71 && !character->HasSpell(48937) && character->HasSpell(25894)) character->learnSpell(48937, false); // unk
            if (level >= 71 && !character->HasSpell(54428)) character->learnSpell(54428, false); //Святая клятва 
            if (level >= 72 && !character->HasSpell(48949) && character->HasSpell(7328)) character->learnSpell(48949, false); // unk
            if (level >= 72 && !character->HasSpell(48816) && character->HasSpell(2812)) character->learnSpell(48816, false); // unk
            if (level >= 73 && !character->HasSpell(48800) && character->HasSpell(879)) character->learnSpell(48800, false); // unk
            if (level >= 73 && !character->HasSpell(48933) && character->HasSpell(25782)) character->learnSpell(48933, false); // unk
            if (level >= 73 && !character->HasSpell(48931) && character->HasSpell(19740)) character->learnSpell(48931, false); // unk
            if (level >= 74 && !character->HasSpell(48941) && character->HasSpell(465)) character->learnSpell(48941, false); // unk
            if (level >= 74 && !character->HasSpell(48805) && character->HasSpell(24275)) character->learnSpell(48805, false); // unk
            if (level >= 74 && !character->HasSpell(48784) && character->HasSpell(19750)) character->learnSpell(48784, false); // unk
            if (level >= 75 && !character->HasSpell(48951) && character->HasSpell(20925)) character->learnSpell(48951, false); // unk
            if (level >= 75 && !character->HasSpell(53600)) character->learnSpell(53600, false); //Щит праведности Уровень 1
            if (level >= 75 && !character->HasSpell(48826) && character->HasSpell(31935)) character->learnSpell(48826, false); // unk
            if (level >= 75 && !character->HasSpell(48781) && character->HasSpell(635)) character->learnSpell(48781, false); // unk
            if (level >= 75 && !character->HasSpell(48818) && character->HasSpell(26573)) character->learnSpell(48818, false); // unk
            if (level >= 75 && !character->HasSpell(48824) && character->HasSpell(20473)) character->learnSpell(48824, false); // unk
            if (level >= 76 && !character->HasSpell(54043) && character->HasSpell(7294)) character->learnSpell(54043, false); //Аура воздаяния Уровень 7
            if (level >= 76 && !character->HasSpell(48943) && character->HasSpell(19876)) character->learnSpell(48943, false); // unk
            if (level >= 77 && !character->HasSpell(48938) && character->HasSpell(25894)) character->learnSpell(48938, false); // unk
            if (level >= 77 && !character->HasSpell(48936) && character->HasSpell(19742)) character->learnSpell(48936, false); // unk
            if (level >= 77 && !character->HasSpell(48945) && character->HasSpell(19888)) character->learnSpell(48945, false); // unk
            if (level >= 78 && !character->HasSpell(48947) && character->HasSpell(19891)) character->learnSpell(48947, false); // unk
            if (level >= 78 && !character->HasSpell(48788) && character->HasSpell(633)) character->learnSpell(48788, false); // unk
            if (level >= 78 && !character->HasSpell(48817) && character->HasSpell(2812)) character->learnSpell(48817, false); // unk
            if (level >= 79 && !character->HasSpell(48950) && character->HasSpell(7328)) character->learnSpell(48950, false); // unk
            if (level >= 79 && !character->HasSpell(48932) && character->HasSpell(19740)) character->learnSpell(48932, false); // unk
            if (level >= 79 && !character->HasSpell(48942) && character->HasSpell(465)) character->learnSpell(48942, false); // unk
            if (level >= 79 && !character->HasSpell(48785) && character->HasSpell(19750)) character->learnSpell(48785, false); // unk
            if (level >= 79 && !character->HasSpell(48934) && character->HasSpell(25782)) character->learnSpell(48934, false); // unk
            if (level >= 79 && !character->HasSpell(48801) && character->HasSpell(879)) character->learnSpell(48801, false); // unk
            if (level >= 80 && !character->HasSpell(61411) && character->HasSpell(53600)) character->learnSpell(61411, false); //Щит праведности Уровень 2
            if (level >= 80 && !character->HasSpell(48782) && character->HasSpell(635)) character->learnSpell(48782, false); // unk
            if (level >= 80 && !character->HasSpell(48806) && character->HasSpell(24275)) character->learnSpell(48806, false); // unk
            if (level >= 80 && !character->HasSpell(48819) && character->HasSpell(26573)) character->learnSpell(48819, false); // unk
            if (level >= 80 && !character->HasSpell(48827) && character->HasSpell(31935)) character->learnSpell(48827, false); // unk
            if (level >= 80 && !character->HasSpell(48952) && character->HasSpell(20925)) character->learnSpell(48952, false); // unk
            if (level >= 80 && !character->HasSpell(48825) && character->HasSpell(20473)) character->learnSpell(48825, false); // unk
            if (level >= 80 && !character->HasSpell(53601)) character->learnSpell(53601, false); //Священный щит Уровень 1
    }

    void learnSpellPriest()
    {

            if (level >= 1 && !character->HasSpell(1243)) character->learnSpell(1243, false); //Слово силы: Стойкость Уровень 1
            if (level >= 4 && !character->HasSpell(589)) character->learnSpell(589, false); //Слово Тьмы: Боль Уровень 1
            if (level >= 4 && !character->HasSpell(2052) && character->HasSpell(2050)) character->learnSpell(2052, false); //Малое исцеление Уровень 2
            if (level >= 6 && !character->HasSpell(17)) character->learnSpell(17, false); //Слово силы: Щит Уровень 1
            if (level >= 6 && !character->HasSpell(591) && character->HasSpell(585)) character->learnSpell(591, false); //Кара Уровень 2
            if (level >= 8 && !character->HasSpell(139)) character->learnSpell(139, false); //Обновление Уровень 1
            if (level >= 8 && !character->HasSpell(586)) character->learnSpell(586, false); //Уход в тень 
            if (level >= 10 && !character->HasSpell(594) && character->HasSpell(589)) character->learnSpell(594, false); //Слово Тьмы: Боль Уровень 2
            if (level >= 10 && !character->HasSpell(2006)) character->learnSpell(2006, false); //Воскрешение Уровень 1
            if (level >= 10 && !character->HasSpell(8092)) character->learnSpell(8092, false); //Взрыв разума Уровень 1
            if (level >= 10 && !character->HasSpell(2053) && character->HasSpell(2050)) character->learnSpell(2053, false); //Малое исцеление Уровень 3
            if (level >= 12 && !character->HasSpell(588)) character->learnSpell(588, false); //Внутренний огонь Уровень 1
            if (level >= 12 && !character->HasSpell(1244) && character->HasSpell(1243)) character->learnSpell(1244, false); //Слово силы: Стойкость Уровень 2
            if (level >= 12 && !character->HasSpell(592) && character->HasSpell(17)) character->learnSpell(592, false); //Слово силы: Щит Уровень 2
            if (level >= 14 && !character->HasSpell(8122)) character->learnSpell(8122, false); //Ментальный крик Уровень 1
            if (level >= 14 && !character->HasSpell(598) && character->HasSpell(585)) character->learnSpell(598, false); //Кара Уровень 3
            if (level >= 14 && !character->HasSpell(6074) && character->HasSpell(139)) character->learnSpell(6074, false); //Обновление Уровень 2
            if (level >= 14 && !character->HasSpell(528)) character->learnSpell(528, false); //Излечение болезни 
            if (level >= 16 && !character->HasSpell(8102) && character->HasSpell(8092)) character->learnSpell(8102, false); //Взрыв разума Уровень 2
            if (level >= 16 && !character->HasSpell(2054)) character->learnSpell(2054, false); //Исцеление Уровень 1
            if (level >= 18 && !character->HasSpell(970) && character->HasSpell(589)) character->learnSpell(970, false); //Слово Тьмы: Боль Уровень 3
            if (level >= 18 && !character->HasSpell(527)) character->learnSpell(527, false); //Рассеивание заклинаний Уровень 1
            if (level >= 18 && !character->HasSpell(600) && character->HasSpell(17)) character->learnSpell(600, false); //Слово силы: Щит Уровень 3
            if (level >= 20 && !character->HasSpell(2061)) character->learnSpell(2061, false); //Быстрое исцеление Уровень 1
            if (level >= 20 && !character->HasSpell(6075) && character->HasSpell(139)) character->learnSpell(6075, false); //Обновление Уровень 3
            if (level >= 20 && !character->HasSpell(453)) character->learnSpell(453, false); //Усмирение 
            if (level >= 20 && !character->HasSpell(9484)) character->learnSpell(9484, false); //Сковывание нежити Уровень 1
            if (level >= 20 && !character->HasSpell(2944)) character->learnSpell(2944, false); //Всепожирающая чума Уровень 1
            if (level >= 20 && !character->HasSpell(14914)) character->learnSpell(14914, false); //Священный огонь Уровень 1
            if (level >= 20 && !character->HasSpell(7128) && character->HasSpell(588)) character->learnSpell(7128, false); //Внутренний огонь Уровень 2
            if (level >= 20 && !character->HasSpell(6346)) character->learnSpell(6346, false); //Защита от страха 
            if (level >= 20 && !character->HasSpell(15237)) character->learnSpell(15237, false); //Кольцо света Уровень 1
            if (level >= 22 && !character->HasSpell(8103) && character->HasSpell(8092)) character->learnSpell(8103, false); //Взрыв разума Уровень 3
            if (level >= 22 && !character->HasSpell(2096)) character->learnSpell(2096, false); //Внутреннее зрение Уровень 1
            if (level >= 22 && !character->HasSpell(2055) && character->HasSpell(2054)) character->learnSpell(2055, false); //Исцеление Уровень 2
            if (level >= 22 && !character->HasSpell(984) && character->HasSpell(585)) character->learnSpell(984, false); //Кара Уровень 4
            if (level >= 22 && !character->HasSpell(2010) && character->HasSpell(2006)) character->learnSpell(2010, false); //Воскрешение Уровень 2
            if (level >= 24 && !character->HasSpell(8129)) character->learnSpell(8129, false); //Сожжение маны 
            if (level >= 24 && !character->HasSpell(1245) && character->HasSpell(1243)) character->learnSpell(1245, false); //Слово силы: Стойкость Уровень 3
            if (level >= 24 && !character->HasSpell(15262) && character->HasSpell(14914)) character->learnSpell(15262, false); //Священный огонь Уровень 2
            if (level >= 24 && !character->HasSpell(3747) && character->HasSpell(17)) character->learnSpell(3747, false); //Слово силы: Щит Уровень 4
            if (level >= 26 && !character->HasSpell(9472) && character->HasSpell(2061)) character->learnSpell(9472, false); //Быстрое исцеление Уровень 2
            if (level >= 26 && !character->HasSpell(992) && character->HasSpell(589)) character->learnSpell(992, false); //Слово Тьмы: Боль Уровень 4
            if (level >= 26 && !character->HasSpell(19238) && character->HasSpell(19236)) character->learnSpell(19238, false); //Молитва отчаяния Уровень 2
            if (level >= 26 && !character->HasSpell(6076) && character->HasSpell(139)) character->learnSpell(6076, false); //Обновление Уровень 4
            if (level >= 28 && !character->HasSpell(17311) && character->HasSpell(15407)) character->learnSpell(17311, false); //Пытка разума Уровень 2
            if (level >= 28 && !character->HasSpell(19276) && character->HasSpell(2944)) character->learnSpell(19276, false); //Всепожирающая чума Уровень 2
            if (level >= 28 && !character->HasSpell(8104) && character->HasSpell(8092)) character->learnSpell(8104, false); //Взрыв разума Уровень 4
            if (level >= 28 && !character->HasSpell(15430) && character->HasSpell(15237)) character->learnSpell(15430, false); //Кольцо света Уровень 2
            if (level >= 28 && !character->HasSpell(8124) && character->HasSpell(8122)) character->learnSpell(8124, false); //Ментальный крик Уровень 2
            if (level >= 28 && !character->HasSpell(6063) && character->HasSpell(2054)) character->learnSpell(6063, false); //Исцеление Уровень 3
            if (level >= 30 && !character->HasSpell(602) && character->HasSpell(588)) character->learnSpell(602, false); //Внутренний огонь Уровень 3
            if (level >= 30 && !character->HasSpell(14752)) character->learnSpell(14752, false); //Божественный дух Уровень 1
            if (level >= 30 && !character->HasSpell(15263) && character->HasSpell(14914)) character->learnSpell(15263, false); //Священный огонь Уровень 3
            if (level >= 30 && !character->HasSpell(976)) character->learnSpell(976, false); //Защита от темной магии Уровень 1
            if (level >= 30 && !character->HasSpell(6065) && character->HasSpell(17)) character->learnSpell(6065, false); //Слово силы: Щит Уровень 5
            if (level >= 30 && !character->HasSpell(1004) && character->HasSpell(585)) character->learnSpell(1004, false); //Кара Уровень 5
            if (level >= 30 && !character->HasSpell(605)) character->learnSpell(605, false); //Контроль над разумом 
            if (level >= 30 && !character->HasSpell(596)) character->learnSpell(596, false); //Молитва исцеления Уровень 1
            if (level >= 32 && !character->HasSpell(552)) character->learnSpell(552, false); //Устранение болезни 
            if (level >= 32 && !character->HasSpell(6077) && character->HasSpell(139)) character->learnSpell(6077, false); //Обновление Уровень 5
            if (level >= 32 && !character->HasSpell(9473) && character->HasSpell(2061)) character->learnSpell(9473, false); //Быстрое исцеление Уровень 3
            if (level >= 34 && !character->HasSpell(1706)) character->learnSpell(1706, false); //Левитация 
            if (level >= 34 && !character->HasSpell(10880) && character->HasSpell(2006)) character->learnSpell(10880, false); //Воскрешение Уровень 3
            if (level >= 34 && !character->HasSpell(19240) && character->HasSpell(19236)) character->learnSpell(19240, false); //Молитва отчаяния Уровень 3
            if (level >= 34 && !character->HasSpell(6064) && character->HasSpell(2054)) character->learnSpell(6064, false); //Исцеление Уровень 4
            if (level >= 34 && !character->HasSpell(2767) && character->HasSpell(589)) character->learnSpell(2767, false); //Слово Тьмы: Боль Уровень 5
            if (level >= 34 && !character->HasSpell(8105) && character->HasSpell(8092)) character->learnSpell(8105, false); //Взрыв разума Уровень 5
            if (level >= 36 && !character->HasSpell(6066) && character->HasSpell(17)) character->learnSpell(6066, false); //Слово силы: Щит Уровень 6
            if (level >= 36 && !character->HasSpell(19277) && character->HasSpell(2944)) character->learnSpell(19277, false); //Всепожирающая чума Уровень 3
            if (level >= 36 && !character->HasSpell(17312) && character->HasSpell(15407)) character->learnSpell(17312, false); //Пытка разума Уровень 3
            if (level >= 36 && !character->HasSpell(988) && character->HasSpell(527)) character->learnSpell(988, false); //Рассеивание заклинаний Уровень 2
            if (level >= 36 && !character->HasSpell(15431) && character->HasSpell(15237)) character->learnSpell(15431, false); //Кольцо света Уровень 3
            if (level >= 36 && !character->HasSpell(2791) && character->HasSpell(1243)) character->learnSpell(2791, false); //Слово силы: Стойкость Уровень 4
            if (level >= 36 && !character->HasSpell(15264) && character->HasSpell(14914)) character->learnSpell(15264, false); //Священный огонь Уровень 4
            if (level >= 38 && !character->HasSpell(6060) && character->HasSpell(585)) character->learnSpell(6060, false); //Кара Уровень 6
            if (level >= 38 && !character->HasSpell(9474) && character->HasSpell(2061)) character->learnSpell(9474, false); //Быстрое исцеление Уровень 4
            if (level >= 38 && !character->HasSpell(6078) && character->HasSpell(139)) character->learnSpell(6078, false); //Обновление Уровень 6
            if (level >= 40 && !character->HasSpell(2060)) character->learnSpell(2060, false); //Великое исцеление Уровень 1
            if (level >= 40 && !character->HasSpell(1006) && character->HasSpell(588)) character->learnSpell(1006, false); //Внутренний огонь Уровень 4
            if (level >= 40 && !character->HasSpell(9485) && character->HasSpell(9484)) character->learnSpell(9485, false); //Сковывание нежити Уровень 2
            if (level >= 40 && !character->HasSpell(996) && character->HasSpell(596)) character->learnSpell(996, false); //Молитва исцеления Уровень 2
            if (level >= 40 && !character->HasSpell(14818) && character->HasSpell(14752)) character->learnSpell(14818, false); //Божественный дух Уровень 2
            if (level >= 40 && !character->HasSpell(8106) && character->HasSpell(8092)) character->learnSpell(8106, false); //Взрыв разума Уровень 6
            if (level >= 42 && !character->HasSpell(19241) && character->HasSpell(19236)) character->learnSpell(19241, false); //Молитва отчаяния Уровень 4
            if (level >= 42 && !character->HasSpell(10957) && character->HasSpell(976)) character->learnSpell(10957, false); //Защита от темной магии Уровень 2
            if (level >= 42 && !character->HasSpell(15265) && character->HasSpell(14914)) character->learnSpell(15265, false); //Священный огонь Уровень 5
            if (level >= 42 && !character->HasSpell(10888) && character->HasSpell(8122)) character->learnSpell(10888, false); //Ментальный крик Уровень 3
            if (level >= 42 && !character->HasSpell(10898) && character->HasSpell(17)) character->learnSpell(10898, false); //Слово силы: Щит Уровень 7
            if (level >= 42 && !character->HasSpell(10892) && character->HasSpell(589)) character->learnSpell(10892, false); //Слово Тьмы: Боль Уровень 6
            if (level >= 44 && !character->HasSpell(19278) && character->HasSpell(2944)) character->learnSpell(19278, false); //Всепожирающая чума Уровень 4
            if (level >= 44 && !character->HasSpell(10927) && character->HasSpell(139)) character->learnSpell(10927, false); //Обновление Уровень 7
            if (level >= 44 && !character->HasSpell(10915) && character->HasSpell(2061)) character->learnSpell(10915, false); //Быстрое исцеление Уровень 5
            if (level >= 44 && !character->HasSpell(10909) && character->HasSpell(2096)) character->learnSpell(10909, false); //Внутреннее зрение Уровень 2
            if (level >= 44 && !character->HasSpell(27799) && character->HasSpell(15237)) character->learnSpell(27799, false); //Кольцо света Уровень 4
            if (level >= 44 && !character->HasSpell(17313) && character->HasSpell(15407)) character->learnSpell(17313, false); //Пытка разума Уровень 4
            if (level >= 46 && !character->HasSpell(10963) && character->HasSpell(2060)) character->learnSpell(10963, false); //Великое исцеление Уровень 2
            if (level >= 46 && !character->HasSpell(10945) && character->HasSpell(8092)) character->learnSpell(10945, false); //Взрыв разума Уровень 7
            if (level >= 46 && !character->HasSpell(10933) && character->HasSpell(585)) character->learnSpell(10933, false); //Кара Уровень 7
            if (level >= 46 && !character->HasSpell(10881) && character->HasSpell(2006)) character->learnSpell(10881, false); //Воскрешение Уровень 4
            if (level >= 48 && !character->HasSpell(15266) && character->HasSpell(14914)) character->learnSpell(15266, false); //Священный огонь Уровень 6
            if (level >= 48 && !character->HasSpell(10937) && character->HasSpell(1243)) character->learnSpell(10937, false); //Слово силы: Стойкость Уровень 5
            if (level >= 48 && !character->HasSpell(10899) && character->HasSpell(17)) character->learnSpell(10899, false); //Слово силы: Щит Уровень 8
            if (level >= 48 && !character->HasSpell(21562)) character->learnSpell(21562, false); //Молитва стойкости Уровень 1
            if (level >= 50 && !character->HasSpell(10960) && character->HasSpell(596)) character->learnSpell(10960, false); //Молитва исцеления Уровень 3
            if (level >= 50 && !character->HasSpell(10893) && character->HasSpell(589)) character->learnSpell(10893, false); //Слово Тьмы: Боль Уровень 7
            if (level >= 50 && !character->HasSpell(14819) && character->HasSpell(14752)) character->learnSpell(14819, false); //Божественный дух Уровень 3
            if (level >= 50 && !character->HasSpell(10916) && character->HasSpell(2061)) character->learnSpell(10916, false); //Быстрое исцеление Уровень 6
            if (level >= 50 && !character->HasSpell(10928) && character->HasSpell(139)) character->learnSpell(10928, false); //Обновление Уровень 8
            if (level >= 50 && !character->HasSpell(19242) && character->HasSpell(19236)) character->learnSpell(19242, false); //Молитва отчаяния Уровень 5
            if (level >= 50 && !character->HasSpell(10951) && character->HasSpell(588)) character->learnSpell(10951, false); //Внутренний огонь Уровень 5
            if (level >= 50 && !character->HasSpell(27870) && character->HasSpell(724)) character->learnSpell(27870, false); //Колодец Света Уровень 2
            if (level >= 52 && !character->HasSpell(19279) && character->HasSpell(2944)) character->learnSpell(19279, false); //Всепожирающая чума Уровень 5
            if (level >= 52 && !character->HasSpell(27800) && character->HasSpell(15237)) character->learnSpell(27800, false); //Кольцо света Уровень 5
            if (level >= 52 && !character->HasSpell(17314) && character->HasSpell(15407)) character->learnSpell(17314, false); //Пытка разума Уровень 5
            if (level >= 52 && !character->HasSpell(10946) && character->HasSpell(8092)) character->learnSpell(10946, false); //Взрыв разума Уровень 8
            if (level >= 52 && !character->HasSpell(10964) && character->HasSpell(2060)) character->learnSpell(10964, false); //Великое исцеление Уровень 3
            if (level >= 54 && !character->HasSpell(15267) && character->HasSpell(14914)) character->learnSpell(15267, false); //Священный огонь Уровень 7
            if (level >= 54 && !character->HasSpell(10900) && character->HasSpell(17)) character->learnSpell(10900, false); //Слово силы: Щит Уровень 9
            if (level >= 54 && !character->HasSpell(10934) && character->HasSpell(585)) character->learnSpell(10934, false); //Кара Уровень 8
            if (level >= 56 && !character->HasSpell(10958) && character->HasSpell(976)) character->learnSpell(10958, false); //Защита от темной магии Уровень 3
            if (level >= 56 && !character->HasSpell(10929) && character->HasSpell(139)) character->learnSpell(10929, false); //Обновление Уровень 9
            if (level >= 56 && !character->HasSpell(27683)) character->learnSpell(27683, false); //Молитва защиты от темной магии Ур. 1
            if (level >= 56 && !character->HasSpell(10917) && character->HasSpell(2061)) character->learnSpell(10917, false); //Быстрое исцеление Уровень 7
            if (level >= 56 && !character->HasSpell(34863) && character->HasSpell(34861)) character->learnSpell(34863, false); //Круг исцеления Уровень 2
            if (level >= 56 && !character->HasSpell(10890) && character->HasSpell(8122)) character->learnSpell(10890, false); //Ментальный крик Уровень 4
            if (level >= 58 && !character->HasSpell(20770) && character->HasSpell(2006)) character->learnSpell(20770, false); //Воскрешение Уровень 5
            if (level >= 58 && !character->HasSpell(10894) && character->HasSpell(589)) character->learnSpell(10894, false); //Слово Тьмы: Боль Уровень 8
            if (level >= 58 && !character->HasSpell(19243) && character->HasSpell(19236)) character->learnSpell(19243, false); //Молитва отчаяния Уровень 6
            if (level >= 58 && !character->HasSpell(10947) && character->HasSpell(8092)) character->learnSpell(10947, false); //Взрыв разума Уровень 9
            if (level >= 58 && !character->HasSpell(10965) && character->HasSpell(2060)) character->learnSpell(10965, false); //Великое исцеление Уровень 4
            if (level >= 60 && !character->HasSpell(25314) && character->HasSpell(2060)) character->learnSpell(25314, false); //Великое исцеление Уровень 5
            if (level >= 60 && !character->HasSpell(25315) && character->HasSpell(139)) character->learnSpell(25315, false); //Обновление Уровень 10
            if (level >= 60 && !character->HasSpell(27841) && character->HasSpell(14752)) character->learnSpell(27841, false); //Божественный дух Уровень 4
            if (level >= 60 && !character->HasSpell(27801) && character->HasSpell(15237)) character->learnSpell(27801, false); //Кольцо света Уровень 6
            if (level >= 60 && !character->HasSpell(27681)) character->learnSpell(27681, false); //Молитва духа Уровень 1
            if (level >= 60 && !character->HasSpell(25316) && character->HasSpell(596)) character->learnSpell(25316, false); //Молитва исцеления Уровень 5
            if (level >= 60 && !character->HasSpell(27871) && character->HasSpell(724)) character->learnSpell(27871, false); //Колодец Света Уровень 3
            if (level >= 60 && !character->HasSpell(34864) && character->HasSpell(34861)) character->learnSpell(34864, false); //Круг исцеления Уровень 3
            if (level >= 60 && !character->HasSpell(15261) && character->HasSpell(14914)) character->learnSpell(15261, false); //Священный огонь Уровень 8
            if (level >= 60 && !character->HasSpell(10961) && character->HasSpell(596)) character->learnSpell(10961, false); //Молитва исцеления Уровень 4
            if (level >= 60 && !character->HasSpell(18807) && character->HasSpell(15407)) character->learnSpell(18807, false); //Пытка разума Уровень 6
            if (level >= 60 && !character->HasSpell(10952) && character->HasSpell(588)) character->learnSpell(10952, false); //Внутренний огонь Уровень 6
            if (level >= 60 && !character->HasSpell(10938) && character->HasSpell(1243)) character->learnSpell(10938, false); //Слово силы: Стойкость Уровень 6
            if (level >= 60 && !character->HasSpell(10955) && character->HasSpell(9484)) character->learnSpell(10955, false); //Сковывание нежити Уровень 3
            if (level >= 60 && !character->HasSpell(19280) && character->HasSpell(2944)) character->learnSpell(19280, false); //Всепожирающая чума Уровень 6
            if (level >= 60 && !character->HasSpell(10901) && character->HasSpell(17)) character->learnSpell(10901, false); //Слово силы: Щит Уровень 10
            if (level >= 60 && !character->HasSpell(21564) && character->HasSpell(21562)) character->learnSpell(21564, false); //Молитва стойкости Уровень 2
            if (level >= 60 && !character->HasSpell(34916) && character->HasSpell(34914)) character->learnSpell(34916, false); //Прикосновение вампира Уровень 2
            if (level >= 61 && !character->HasSpell(25233) && character->HasSpell(2061)) character->learnSpell(25233, false); //Быстрое исцеление Уровень 8
            if (level >= 61 && !character->HasSpell(25363) && character->HasSpell(585)) character->learnSpell(25363, false); //Кара Уровень 9
            if (level >= 62 && !character->HasSpell(32379)) character->learnSpell(32379, false); //Слово Тьмы: Смерть Уровень 1
            if (level >= 63 && !character->HasSpell(25372) && character->HasSpell(8092)) character->learnSpell(25372, false); //Взрыв разума Уровень 10
            if (level >= 63 && !character->HasSpell(25210) && character->HasSpell(2060)) character->learnSpell(25210, false); //Великое исцеление Уровень 6
            if (level >= 64 && !character->HasSpell(32546)) character->learnSpell(32546, false); //Связующее исцеление Уровень 1
            if (level >= 65 && !character->HasSpell(25217) && character->HasSpell(17)) character->learnSpell(25217, false); //Слово силы: Щит Уровень 11
            if (level >= 65 && !character->HasSpell(25367) && character->HasSpell(589)) character->learnSpell(25367, false); //Слово Тьмы: Боль Уровень 9
            if (level >= 65 && !character->HasSpell(34865) && character->HasSpell(34861)) character->learnSpell(34865, false); //Круг исцеления Уровень 4
            if (level >= 65 && !character->HasSpell(25221) && character->HasSpell(139)) character->learnSpell(25221, false); //Обновление Уровень 11
            if (level >= 66 && !character->HasSpell(34433)) character->learnSpell(34433, false); //Исчадие Тьмы 
            if (level >= 66 && !character->HasSpell(25437) && character->HasSpell(19236)) character->learnSpell(25437, false); //Молитва отчаяния Уровень 7
            if (level >= 66 && !character->HasSpell(25384) && character->HasSpell(14914)) character->learnSpell(25384, false); //Священный огонь Уровень 9
            if (level >= 67 && !character->HasSpell(25235) && character->HasSpell(2061)) character->learnSpell(25235, false); //Быстрое исцеление Уровень 9
            if (level >= 68 && !character->HasSpell(25213) && character->HasSpell(2060)) character->learnSpell(25213, false); //Великое исцеление Уровень 7
            if (level >= 68 && !character->HasSpell(25467) && character->HasSpell(2944)) character->learnSpell(25467, false); //Всепожирающая чума Уровень 7
            if (level >= 68 && !character->HasSpell(33076)) character->learnSpell(33076, false); //Молитва восстановления Уровень 1
            if (level >= 68 && !character->HasSpell(25308) && character->HasSpell(596)) character->learnSpell(25308, false); //Молитва исцеления Уровень 6
            if (level >= 68 && !character->HasSpell(25387) && character->HasSpell(15407)) character->learnSpell(25387, false); //Пытка разума Уровень 7
            if (level >= 68 && !character->HasSpell(25331) && character->HasSpell(15237)) character->learnSpell(25331, false); //Кольцо света Уровень 7
            if (level >= 68 && !character->HasSpell(25433) && character->HasSpell(976)) character->learnSpell(25433, false); //Защита от темной магии Уровень 4
            if (level >= 68 && !character->HasSpell(25435) && character->HasSpell(2006)) character->learnSpell(25435, false); //Воскрешение Уровень 6
            if (level >= 69 && !character->HasSpell(25364) && character->HasSpell(585)) character->learnSpell(25364, false); //Кара Уровень 10
            if (level >= 69 && !character->HasSpell(25431) && character->HasSpell(588)) character->learnSpell(25431, false); //Внутренний огонь Уровень 7
            if (level >= 69 && !character->HasSpell(25375) && character->HasSpell(8092)) character->learnSpell(25375, false); //Взрыв разума Уровень 11
            if (level >= 70 && !character->HasSpell(34917) && character->HasSpell(34914)) character->learnSpell(34917, false); //Прикосновение вампира Уровень 3
            if (level >= 70 && !character->HasSpell(53005) && character->HasSpell(47540)) character->learnSpell(53005, false); // unk
            if (level >= 70 && !character->HasSpell(39374) && character->HasSpell(27683)) character->learnSpell(39374, false); //Молитва защиты от темной магии Ур. 2
            if (level >= 70 && !character->HasSpell(34866) && character->HasSpell(34861)) character->learnSpell(34866, false); //Круг исцеления Уровень 5
            if (level >= 70 && !character->HasSpell(32999) && character->HasSpell(27681)) character->learnSpell(32999, false); //Молитва духа Уровень 2
            if (level >= 70 && !character->HasSpell(25218) && character->HasSpell(17)) character->learnSpell(25218, false); //Слово силы: Щит Уровень 12
            if (level >= 70 && !character->HasSpell(25222) && character->HasSpell(139)) character->learnSpell(25222, false); //Обновление Уровень 12
            if (level >= 70 && !character->HasSpell(25312) && character->HasSpell(14752)) character->learnSpell(25312, false); //Божественный дух Уровень 5
            if (level >= 70 && !character->HasSpell(25368) && character->HasSpell(589)) character->learnSpell(25368, false); //Слово Тьмы: Боль Уровень 10
            if (level >= 70 && !character->HasSpell(25389) && character->HasSpell(1243)) character->learnSpell(25389, false); //Слово силы: Стойкость Уровень 7
            if (level >= 70 && !character->HasSpell(25392) && character->HasSpell(21562)) character->learnSpell(25392, false); //Молитва стойкости Уровень 3
            if (level >= 70 && !character->HasSpell(28275) && character->HasSpell(724)) character->learnSpell(28275, false); //Колодец Света Уровень 4
            if (level >= 70 && !character->HasSpell(32375)) character->learnSpell(32375, false); //Массовое рассеивание 
            if (level >= 70 && !character->HasSpell(32996) && character->HasSpell(32379)) character->learnSpell(32996, false); //Слово Тьмы: Смерть Уровень 2
            if (level >= 71 && !character->HasSpell(48040) && character->HasSpell(588)) character->learnSpell(48040, false); // unk
            if (level >= 72 && !character->HasSpell(48134) && character->HasSpell(14914)) character->learnSpell(48134, false); // unk
            if (level >= 72 && !character->HasSpell(48119) && character->HasSpell(32546)) character->learnSpell(48119, false); // unk
            if (level >= 73 && !character->HasSpell(48070) && character->HasSpell(2061)) character->learnSpell(48070, false); // unk
            if (level >= 73 && !character->HasSpell(48299) && character->HasSpell(2944)) character->learnSpell(48299, false); // unk
            if (level >= 73 && !character->HasSpell(48172) && character->HasSpell(19236)) character->learnSpell(48172, false); // unk
            if (level >= 73 && !character->HasSpell(48062) && character->HasSpell(2060)) character->learnSpell(48062, false); // unk
            if (level >= 74 && !character->HasSpell(48112) && character->HasSpell(33076)) character->learnSpell(48112, false); // unk
            if (level >= 74 && !character->HasSpell(48155) && character->HasSpell(15407)) character->learnSpell(48155, false); // unk
            if (level >= 74 && !character->HasSpell(48126) && character->HasSpell(8092)) character->learnSpell(48126, false); // unk
            if (level >= 74 && !character->HasSpell(48122) && character->HasSpell(585)) character->learnSpell(48122, false); // unk
            if (level >= 75 && !character->HasSpell(53006) && character->HasSpell(47540)) character->learnSpell(53006, false); // unk
            if (level >= 75 && !character->HasSpell(48086) && character->HasSpell(724)) character->learnSpell(48086, false); // unk
            if (level >= 75 && !character->HasSpell(48124) && character->HasSpell(589)) character->learnSpell(48124, false); // unk
            if (level >= 75 && !character->HasSpell(48157) && character->HasSpell(32379)) character->learnSpell(48157, false); // unk
            if (level >= 75 && !character->HasSpell(48159) && character->HasSpell(34914)) character->learnSpell(48159, false); // unk
            if (level >= 75 && !character->HasSpell(48088) && character->HasSpell(34861)) character->learnSpell(48088, false); // unk
            if (level >= 75 && !character->HasSpell(48067) && character->HasSpell(139)) character->learnSpell(48067, false); // unk
            if (level >= 75 && !character->HasSpell(48065) && character->HasSpell(17)) character->learnSpell(48065, false); // unk
            if (level >= 75 && !character->HasSpell(48077) && character->HasSpell(15237)) character->learnSpell(48077, false); // unk
            if (level >= 75 && !character->HasSpell(48045)) character->learnSpell(48045, false); // unk
            if (level >= 76 && !character->HasSpell(48169) && character->HasSpell(976)) character->learnSpell(48169, false); // unk
            if (level >= 76 && !character->HasSpell(48072) && character->HasSpell(596)) character->learnSpell(48072, false); // unk
            if (level >= 77 && !character->HasSpell(48170) && character->HasSpell(27683)) character->learnSpell(48170, false); // unk
            if (level >= 77 && !character->HasSpell(48168) && character->HasSpell(588)) character->learnSpell(48168, false); // unk
            if (level >= 78 && !character->HasSpell(48063) && character->HasSpell(2060)) character->learnSpell(48063, false); // unk
            if (level >= 78 && !character->HasSpell(48171) && character->HasSpell(2006)) character->learnSpell(48171, false); // unk
            if (level >= 78 && !character->HasSpell(48120) && character->HasSpell(32546)) character->learnSpell(48120, false); // unk
            if (level >= 78 && !character->HasSpell(48135) && character->HasSpell(14914)) character->learnSpell(48135, false); // unk
            if (level >= 79 && !character->HasSpell(48071) && character->HasSpell(2061)) character->learnSpell(48071, false); // unk
            if (level >= 79 && !character->HasSpell(48300) && character->HasSpell(2944)) character->learnSpell(48300, false); // unk
            if (level >= 79 && !character->HasSpell(48127) && character->HasSpell(8092)) character->learnSpell(48127, false); // unk
            if (level >= 79 && !character->HasSpell(48113) && character->HasSpell(33076)) character->learnSpell(48113, false); // unk
            if (level >= 79 && !character->HasSpell(48123) && character->HasSpell(585)) character->learnSpell(48123, false); // unk
            if (level >= 80 && !character->HasSpell(53007) && character->HasSpell(47540)) character->learnSpell(53007, false); // unk
            if (level >= 80 && !character->HasSpell(64843)) character->learnSpell(64843, false); //Божественный гимн Уровень 1
            if (level >= 80 && !character->HasSpell(53023) && character->HasSpell(48045)) character->learnSpell(53023, false); // unk
            if (level >= 80 && !character->HasSpell(48173) && character->HasSpell(19236)) character->learnSpell(48173, false); // unk
            if (level >= 80 && !character->HasSpell(48162) && character->HasSpell(21562)) character->learnSpell(48162, false); // unk
            if (level >= 80 && !character->HasSpell(48066) && character->HasSpell(17)) character->learnSpell(48066, false); // unk
            if (level >= 80 && !character->HasSpell(48087) && character->HasSpell(724)) character->learnSpell(48087, false); // unk
            if (level >= 80 && !character->HasSpell(48078) && character->HasSpell(15237)) character->learnSpell(48078, false); // unk
            if (level >= 80 && !character->HasSpell(48074) && character->HasSpell(27681)) character->learnSpell(48074, false); // unk
            if (level >= 80 && !character->HasSpell(48073) && character->HasSpell(14752)) character->learnSpell(48073, false); // unk
            if (level >= 80 && !character->HasSpell(48125) && character->HasSpell(589)) character->learnSpell(48125, false); // unk
            if (level >= 80 && !character->HasSpell(48156) && character->HasSpell(15407)) character->learnSpell(48156, false); // unk
            if (level >= 80 && !character->HasSpell(48068) && character->HasSpell(139)) character->learnSpell(48068, false); // unk
            if (level >= 80 && !character->HasSpell(48158) && character->HasSpell(32379)) character->learnSpell(48158, false); // unk
            if (level >= 80 && !character->HasSpell(48089) && character->HasSpell(34861)) character->learnSpell(48089, false); // unk
            if (level >= 80 && !character->HasSpell(48160) && character->HasSpell(34914)) character->learnSpell(48160, false); // unk
            if (level >= 80 && !character->HasSpell(48161) && character->HasSpell(1243)) character->learnSpell(48161, false); // unk
            if (level >= 80 && !character->HasSpell(64901)) character->learnSpell(64901, false); //Гимн надежды 
    }

    void learnSpellDeathKnight()
    {
            if (!character->HasSpell(53343)) character->learnSpell(53343, false); //Руна ледяного жара 
            if (!character->HasSpell(53341)) character->learnSpell(53341, false); //Руна оплавленного ледника 
            if (!character->HasSpell(46584)) character->learnSpell(46584, false); //Воскрешение мертвых 

            if (level < 56) return;

            if (!character->HasSpell(50842)) character->learnSpell(50842, false); //Мор
            if (!character->HasSpell(49998)) character->learnSpell(49998, false); //Удар смерти

            if (level < 57) return;

            if (!character->HasSpell(48263)) character->learnSpell(48263, false); //Власть крови
            if (!character->HasSpell(53342)) character->learnSpell(53342, false); //Руна сокрушения чар 
            if (!character->HasSpell(47528)) character->learnSpell(47528, false); //Заморозка разума
            if (!character->HasSpell(54447)) character->learnSpell(54447, false); //Руна разрушения чар 

            if (level < 58) return;
            
            if (!character->HasSpell(48721)) character->learnSpell(48721, false); //Вскипание крови
            if (!character->HasSpell(45524)) character->learnSpell(45524, false); //Ледяные оковы 

            if (level< 59) return;
            
            if (!character->HasSpell(55258) && character->HasSpell(55050)) character->learnSpell(55258, false); //Удар в сердце Уровень 2
            if (!character->HasSpell(47476)) character->learnSpell(47476, false); //Удушение
            if (!character->HasSpell(49926) && character->HasSpell(45902)) character->learnSpell(49926, false); //Кровавый удар Уровень 2

            if (level < 60) return;
            
            if (!character->HasSpell(51416) && character->HasSpell(49143)) character->learnSpell(51416, false); //Ледяной Удар Уровень 2
            if (!character->HasSpell(53331)) character->learnSpell(53331, false); //Руна проклятия Плети 
            if (!character->HasSpell(49917) && character->HasSpell(45462)) character->learnSpell(49917, false); //Удар чумы Уровень 2
            if (!character->HasSpell(43265)) character->learnSpell(43265, false); //Смерть и разложение Уровень 1
            if (!character->HasSpell(51325) && character->HasSpell(49158)) character->learnSpell(51325, false); //Взрыв трупа Уровень 2
            
            if (level < 61) return;

            if (!character->HasSpell(49020)) character->learnSpell(49020, false); //Уничтожение
            if (!character->HasSpell(49896) && character->HasSpell(45477)) character->learnSpell(49896, false); //Ледяное прикосновение Уровень 2
            if (!character->HasSpell(3714)) character->learnSpell(3714, false); //Льдистый путь 
            if (!character->HasSpell(49892) && character->HasSpell(47541)) character->learnSpell(49892, false); //Лик смерти Уровень 2

            if (level < 62) return;

            // if (!character->HasSpell(51426)) character->learnSpell(51426, false); //Мор Уровень 2
            if (!character->HasSpell(48792)) character->learnSpell(48792, false); //Незыблемость льда

            if (level < 63) return;

            if (!character->HasSpell(49999) && character->HasSpell(49998)) character->learnSpell(49999, false); //Удар смерти Уровень 2
            if (!character->HasSpell(53323)) character->learnSpell(53323, false); //Руна расколотых мечей 
            if (!character->HasSpell(54446)) character->learnSpell(54446, false); //Руна сломанных мечей 

            if (level < 64) return;

            if (!character->HasSpell(45529)) character->learnSpell(45529, false); //Кровоотвод 
            if (!character->HasSpell(49927) && character->HasSpell(45902)) character->learnSpell(49927, false); //Кровавый удар Уровень 3
            if (!character->HasSpell(55259) && character->HasSpell(55050)) character->learnSpell(55259, false); //Удар в сердце Уровень 3

            if (level < 65) return;
            
            if (!character->HasSpell(49918) && character->HasSpell(45462)) character->learnSpell(49918, false); //Удар чумы Уровень 3
            if (!character->HasSpell(51417) && character->HasSpell(49143)) character->learnSpell(51417, false); //Ледяной Удар Уровень 3
            if (!character->HasSpell(57330)) character->learnSpell(57330, false); //Зимний горн Уровень 1
            if (!character->HasSpell(56222)) character->learnSpell(56222, false); //Темная власть 
            //if (!character->HasSpell(49913)) character->learnSpell(49913, false); //Удушение Уровень 2

            if (level < 66) return;
            
            if (!character->HasSpell(49939) && character->HasSpell(48721)) character->learnSpell(49939, false); //Вскипание крови Уровень 1
            if (!character->HasSpell(48743)) character->learnSpell(48743, false); //Смертельный союз

            if (level < 67) return;
            
            if (!character->HasSpell(56815)) character->learnSpell(56815, false); //Рунический удар 
            if (!character->HasSpell(51423) && character->HasSpell(49020)) character->learnSpell(51423, false); //Уничтожение Уровень 1
            if (!character->HasSpell(49936) && character->HasSpell(43265)) character->learnSpell(49936, false); //Смерть и разложение Уровень 2
            if (!character->HasSpell(49903) && character->HasSpell(45477)) character->learnSpell(49903, false); //Ледяное прикосновение Уровень 3
            if (!character->HasSpell(55265) && character->HasSpell(55090)) character->learnSpell(55265, false); //Удар Плети Уровень 2

            if (level < 68) return;
            
            if (!character->HasSpell(49893) && character->HasSpell(47541)) character->learnSpell(49893, false); //Ли к смерти Уровень 1
            if (!character->HasSpell(48707)) character->learnSpell(48707, false); //Антимагический панцирь
            //if (!character->HasSpell(51427)) character->learnSpell(51427, false); //Мор Уровень 3

            if (level < 69) return;
            
            if (!character->HasSpell(49928) && character->HasSpell(45902)) character->learnSpell(49928, false); //Кровавый удвар Уровень 4
            if (!character->HasSpell(55260) && character->HasSpell(55050)) character->learnSpell(55260, false); //Удар в сердце Уровень 4
            //if (!character->HasSpell(49914)) character->learnSpell(49914, false); //Удушение Уровень 3

            if (level < 70) return;
            
            if (level >= 70 && !character->HasSpell(51418) && character->HasSpell(49143)) character->learnSpell(51418, false); //Ледяной Удар Уровень 4
            if (level >= 70 && !character->HasSpell(53344)) character->learnSpell(53344, false); //Руна павшего рыцаря 
            if (level >= 70 && !character->HasSpell(51409) && character->HasSpell(49184)) character->learnSpell(51409, false); //Воющий ветер Уровень 2
            if (level >= 70 && !character->HasSpell(49919) && character->HasSpell(45462)) character->learnSpell(49919, false); //Удар чумы Уровень 4
            if (level >= 70 && !character->HasSpell(51326) && character->HasSpell(49158)) character->learnSpell(51326, false); //Взрыв трупа Уровень 3
            if (level >= 70 && !character->HasSpell(48265)) character->learnSpell(48265, false); //Власть нечестивости
            if (level >= 70 && !character->HasSpell(45463) && character->HasSpell(49998)) character->learnSpell(45463, false); //Удар смерти Уровень 3
            if (level >= 72 && !character->HasSpell(61999)) character->learnSpell(61999, false); //Воскрешение союзника 
            if (level >= 72 && !character->HasSpell(62158)) character->learnSpell(62158, false); //Руна каменной горгульи 
            if (level >= 72 && !character->HasSpell(70164)) character->learnSpell(70164, false); //Руна нерубского панциря 
            if (level >= 72 && !character->HasSpell(49940) && character->HasSpell(48721)) character->learnSpell(49940, false); //Вскипание крови Уровень 3
            if (level >= 73 && !character->HasSpell(49937) && character->HasSpell(43265)) character->learnSpell(49937, false); //Смерть и разложение Уровень 3
            if (level >= 73 && !character->HasSpell(49904) && character->HasSpell(45477)) character->learnSpell(49904, false); //Ледяное прикосновение Уровень 4
            if (level >= 73 && !character->HasSpell(51424) && character->HasSpell(49020)) character->learnSpell(51424, false); //Уничтожение Уровень 3
            if (level >= 73 && !character->HasSpell(55270) && character->HasSpell(55090)) character->learnSpell(55270, false); //Удар Плети Уровень 3

            // if (level >= 74 && !character->HasSpell(49915)) character->learnSpell(49915, false); //Удушение Уровень 4

            if (level >= 74 && !character->HasSpell(55261) && character->HasSpell(55050)) character->learnSpell(55261, false); //Удар в сердце Уровень 5
            //if (level >= 74 && !character->HasSpell(51428)) character->learnSpell(51428, false); //Мор Уровень 4
            if (level >= 74 && !character->HasSpell(49929) && character->HasSpell(45902)) character->learnSpell(49929, false); // unk
            if (level >= 75 && !character->HasSpell(49920) && character->HasSpell(45462)) character->learnSpell(49920, false); // unk
            if (level >= 75 && !character->HasSpell(49923) && character->HasSpell(49998)) character->learnSpell(49923, false); // unk
            if (level >= 75 && !character->HasSpell(57623) && character->HasSpell(57330)) character->learnSpell(57623, false); //Зимний горн Уровень 2
            if (level >= 75 && !character->HasSpell(51419) && character->HasSpell(49143)) character->learnSpell(51419, false); // unk
            if (level >= 75 && !character->HasSpell(51327) && character->HasSpell(49158)) character->learnSpell(51327, false); // unk
            if (level >= 75 && !character->HasSpell(47568)) character->learnSpell(47568, false); //Усиление рунического оружия
            if (level >= 75 && !character->HasSpell(51410) && character->HasSpell(49184)) character->learnSpell(51410, false); // unk
            if (level >= 76 && !character->HasSpell(49894) && character->HasSpell(47541)) character->learnSpell(49894, false); // unk
            if (level >= 78 && !character->HasSpell(49941) && character->HasSpell(48721)) character->learnSpell(49941, false); // unk
            if (level >= 78 && !character->HasSpell(49909) && character->HasSpell(45477)) character->learnSpell(49909, false); // unk
            //if (level >= 79 && !character->HasSpell(49916)) character->learnSpell(49916, false); //Удушение Уровень 5
            if (level >= 79 && !character->HasSpell(55271) && character->HasSpell(55090)) character->learnSpell(55271, false); //Удар Плети Уровень 4
            //if (level >= 79 && !character->HasSpell(51429)) character->learnSpell(51429, false); //Мор Уровень 5
            if (level >= 79 && !character->HasSpell(51425) && character->HasSpell(49020)) character->learnSpell(51425, false); // unk
            if (level >= 80 && !character->HasSpell(49895) && character->HasSpell(47541)) character->learnSpell(49895, false); // unk
            if (level >= 80 && !character->HasSpell(42650)) character->learnSpell(42650, false); //Войско мертвых 
            if (level >= 80 && !character->HasSpell(49924) && character->HasSpell(49998)) character->learnSpell(49924, false); // unk
            if (level >= 80 && !character->HasSpell(55268) && character->HasSpell(49143)) character->learnSpell(55268, false); //Ледяной удар Уровень 6
            if (level >= 80 && !character->HasSpell(49921) && character->HasSpell(45462)) character->learnSpell(49921, false); // unk
            if (level >= 80 && !character->HasSpell(49930) && character->HasSpell(45902)) character->learnSpell(49930, false); // unk
            if (level >= 80 && !character->HasSpell(49938) && character->HasSpell(43265)) character->learnSpell(49938, false); // unk
            if (level >= 80 && !character->HasSpell(51328) && character->HasSpell(49158)) character->learnSpell(51328, false); // unk
            if (level >= 80 && !character->HasSpell(51411) && character->HasSpell(49184)) character->learnSpell(51411, false); // unk
            if (level >= 80 && !character->HasSpell(55262) && character->HasSpell(55050)) character->learnSpell(55262, false); //Удар в сердце Уровень 6
    }

    void learnSpellMage()
    {
            if (level >= 1 && !character->HasSpell(1459)) character->learnSpell(1459, false); //Чародейский интеллект Уровень 1
            if (level >= 4 && !character->HasSpell(116)) character->learnSpell(116, false); //Ледяная стрела Уровень 1
            if (level >= 4 && !character->HasSpell(5504)) character->learnSpell(5504, false); //Сотворение воды Уровень 1
            if (level >= 6 && !character->HasSpell(143) && character->HasSpell(133)) character->learnSpell(143, false); //Огненный шар Уровень 2
            if (level >= 6 && !character->HasSpell(587)) character->learnSpell(587, false); //Сотворение пищи Уровень 1
            if (level >= 6 && !character->HasSpell(2136)) character->learnSpell(2136, false); //Огненный взрыв Уровень 1
            if (level >= 8 && !character->HasSpell(5143)) character->learnSpell(5143, false); //Чародейские стрелы Уровень 1
            if (level >= 8 && !character->HasSpell(205) && character->HasSpell(116)) character->learnSpell(205, false); //Ледяная стрела Уровень 2
            if (level >= 8 && !character->HasSpell(118)) character->learnSpell(118, false); //Превращение Уровень 1
            if (level >= 10 && !character->HasSpell(7300) && character->HasSpell(168)) character->learnSpell(7300, false); //Морозный доспех Уровень 2
            if (level >= 10 && !character->HasSpell(122)) character->learnSpell(122, false); //Кольцо льда Уровень 1
            if (level >= 10 && !character->HasSpell(5505) && character->HasSpell(5504)) character->learnSpell(5505, false); //Сотворение воды Уровень 2
            if (level >= 12 && !character->HasSpell(130)) character->learnSpell(130, false); //Замедленное падение 
            if (level >= 12 && !character->HasSpell(145) && character->HasSpell(133)) character->learnSpell(145, false); //Огненный шар Уровень 3
            if (level >= 12 && !character->HasSpell(604)) character->learnSpell(604, false); //Ослабление магии Уровень 1
            if (level >= 12 && !character->HasSpell(597) && character->HasSpell(587)) character->learnSpell(597, false); //Сотворение пищи Уровень 2
            if (level >= 14 && !character->HasSpell(1449)) character->learnSpell(1449, false); //Чародейский взрыв Уровень 1
            if (level >= 14 && !character->HasSpell(1460) && character->HasSpell(1459)) character->learnSpell(1460, false); //Чародейский интеллект Уровень 2
            if (level >= 14 && !character->HasSpell(2137) && character->HasSpell(2136)) character->learnSpell(2137, false); //Огненный взрыв Уровень 2
            if (level >= 14 && !character->HasSpell(837) && character->HasSpell(116)) character->learnSpell(837, false); //Ледяная стрела Уровень 3
            if (level >= 16 && !character->HasSpell(5144) && character->HasSpell(5143)) character->learnSpell(5144, false); //Чародейские стрелы Уровень 2
            if (level >= 16 && !character->HasSpell(2120)) character->learnSpell(2120, false); //Огненный столб Уровень 1
            if (level >= 18 && !character->HasSpell(3140) && character->HasSpell(133)) character->learnSpell(3140, false); //Огненный шар Уровень 4
            if (level >= 18 && !character->HasSpell(1008)) character->learnSpell(1008, false); //Усиление магии Уровень 1
            if (level >= 18 && !character->HasSpell(475)) character->learnSpell(475, false); //Снятие проклятия 
            if (level >= 20 && !character->HasSpell(7301) && character->HasSpell(168)) character->learnSpell(7301, false); //Морозный доспех Уровень 3
            if (level >= 20 && !character->HasSpell(5506) && character->HasSpell(5504)) character->learnSpell(5506, false); //Сотворение воды Уровень 3
            if (level >= 20 && !character->HasSpell(1953)) character->learnSpell(1953, false); //Скачок 
            if (level >= 20 && !character->HasSpell(1463)) character->learnSpell(1463, false); //Щит маны Уровень 1
            if (level >= 20 && !character->HasSpell(12824) && character->HasSpell(118)) character->learnSpell(12824, false); //Превращение Уровень 2
            if (level >= 20 && !character->HasSpell(10)) character->learnSpell(10, false); //Снежная буря Уровень 1
            if (level >= 20 && !character->HasSpell(543)) character->learnSpell(543, false); //Защита от огня Уровень 1
            if (level >= 20 && !character->HasSpell(12051)) character->learnSpell(12051, false); //Прилив сил 
            if (level >= 20 && !character->HasSpell(7322) && character->HasSpell(116)) character->learnSpell(7322, false); //Ледяная стрела Уровень 4
            if (level >= 22 && !character->HasSpell(2948)) character->learnSpell(2948, false); //Ожог Уровень 1
            if (level >= 22 && !character->HasSpell(6143)) character->learnSpell(6143, false); //Защита от магии льда Уровень 1
            if (level >= 22 && !character->HasSpell(2138) && character->HasSpell(2136)) character->learnSpell(2138, false); //Огненный взрыв Уровень 3
            if (level >= 22 && !character->HasSpell(8437) && character->HasSpell(1449)) character->learnSpell(8437, false); //Чародейский взрыв Уровень 2
            if (level >= 22 && !character->HasSpell(990) && character->HasSpell(587)) character->learnSpell(990, false); //Сотворение пищи Уровень 3
            if (level >= 24 && !character->HasSpell(5145) && character->HasSpell(5143)) character->learnSpell(5145, false); //Чародейские стрелы Уровень 3
            if (level >= 24 && !character->HasSpell(2121) && character->HasSpell(2120)) character->learnSpell(2121, false); //Огненный столб Уровень 2
            if (level >= 24 && !character->HasSpell(8400) && character->HasSpell(133)) character->learnSpell(8400, false); //Огненный шар Уровень 5
            if (level >= 24 && !character->HasSpell(8450) && character->HasSpell(604)) character->learnSpell(8450, false); //Ослабление магии Уровень 2
            if (level >= 24 && !character->HasSpell(2139)) character->learnSpell(2139, false); //Антимагия 
            if (level >= 24 && !character->HasSpell(12505) && character->HasSpell(11366)) character->learnSpell(12505, false); //Огненная глыба Уровень 2
            if (level >= 26 && !character->HasSpell(865) && character->HasSpell(122)) character->learnSpell(865, false); //Кольцо льда Уровень 2
            if (level >= 26 && !character->HasSpell(120)) character->learnSpell(120, false); //Конус холода Уровень 1
            if (level >= 26 && !character->HasSpell(8406) && character->HasSpell(116)) character->learnSpell(8406, false); //Ледяная стрела Уровень 5
            if (level >= 28 && !character->HasSpell(8494) && character->HasSpell(1463)) character->learnSpell(8494, false); //Щит маны Уровень 2
            if (level >= 28 && !character->HasSpell(8444) && character->HasSpell(2948)) character->learnSpell(8444, false); //Ожог Уровень 2
            if (level >= 28 && !character->HasSpell(6141) && character->HasSpell(10)) character->learnSpell(6141, false); //Снежная буря Уровень 2
            if (level >= 28 && !character->HasSpell(1461) && character->HasSpell(1459)) character->learnSpell(1461, false); //Чародейский интеллект Уровень 3
            if (level >= 28 && !character->HasSpell(759)) character->learnSpell(759, false); //Сотворение самоцвета маны Уровень 1
            if (level >= 30 && !character->HasSpell(45438)) character->learnSpell(45438, false); //Ледяная глыба 
            if (level >= 30 && !character->HasSpell(12522) && character->HasSpell(11366)) character->learnSpell(12522, false); //Огненная глыба Уровень 3
            if (level >= 30 && !character->HasSpell(8457) && character->HasSpell(543)) character->learnSpell(8457, false); //Защита от огня Уровень 2
            if (level >= 30 && !character->HasSpell(8455) && character->HasSpell(1008)) character->learnSpell(8455, false); //Усиление магии Уровень 2
            if (level >= 30 && !character->HasSpell(6127) && character->HasSpell(5504)) character->learnSpell(6127, false); //Сотворение воды Уровень 4
            if (level >= 30 && !character->HasSpell(8438) && character->HasSpell(1449)) character->learnSpell(8438, false); //Чародейский взрыв Уровень 3
            if (level >= 30 && !character->HasSpell(7302)) character->learnSpell(7302, false); //Ледяной доспех Уровень 1
            if (level >= 30 && !character->HasSpell(8412) && character->HasSpell(2136)) character->learnSpell(8412, false); //Огненный взрыв Уровень 4
            if (level >= 30 && !character->HasSpell(8401) && character->HasSpell(133)) character->learnSpell(8401, false); //Огненный шар Уровень 6
            if (level >= 32 && !character->HasSpell(8416) && character->HasSpell(5143)) character->learnSpell(8416, false); //Чародейские стрелы Уровень 4
            if (level >= 32 && !character->HasSpell(8461) && character->HasSpell(6143)) character->learnSpell(8461, false); //Защита от магии льда Уровень 2
            if (level >= 32 && !character->HasSpell(8407) && character->HasSpell(116)) character->learnSpell(8407, false); //Ледяная стрела Уровень 6
            if (level >= 32 && !character->HasSpell(6129) && character->HasSpell(587)) character->learnSpell(6129, false); //Сотворение пищи Уровень 4
            if (level >= 32 && !character->HasSpell(8422) && character->HasSpell(2120)) character->learnSpell(8422, false); //Огненный столб Уровень 3
            if (level >= 34 && !character->HasSpell(8492) && character->HasSpell(120)) character->learnSpell(8492, false); //Конус холода Уровень 2
            if (level >= 34 && !character->HasSpell(6117)) character->learnSpell(6117, false); //Магический доспех Уровень 1
            if (level >= 34 && !character->HasSpell(8445) && character->HasSpell(2948)) character->learnSpell(8445, false); //Ожог Уровень 3
            if (level >= 36 && !character->HasSpell(13018) && character->HasSpell(11113)) character->learnSpell(13018, false); //Взрывная волна Уровень 2
            if (level >= 36 && !character->HasSpell(8427) && character->HasSpell(10)) character->learnSpell(8427, false); //Снежная буря Уровень 3
            if (level >= 36 && !character->HasSpell(8451) && character->HasSpell(604)) character->learnSpell(8451, false); //Ослабление магии Уровень 3
            if (level >= 36 && !character->HasSpell(8402) && character->HasSpell(133)) character->learnSpell(8402, false); //Огненный шар Уровень 7
            if (level >= 36 && !character->HasSpell(8495) && character->HasSpell(1463)) character->learnSpell(8495, false); //Щит маны Уровень 3
            if (level >= 36 && !character->HasSpell(12523) && character->HasSpell(11366)) character->learnSpell(12523, false); //Огненная глыба Уровень 4
            if (level >= 38 && !character->HasSpell(3552) && character->HasSpell(759)) character->learnSpell(3552, false); //Сотворение самоцвета маны Уровень 2
            if (level >= 38 && !character->HasSpell(8408) && character->HasSpell(116)) character->learnSpell(8408, false); //Ледяная стрела Уровень 7
            if (level >= 38 && !character->HasSpell(8439) && character->HasSpell(1449)) character->learnSpell(8439, false); //Чародейский взрыв Уровень 4
            if (level >= 38 && !character->HasSpell(8413) && character->HasSpell(2136)) character->learnSpell(8413, false); //Огненный взрыв Уровень 5
            if (level >= 40 && !character->HasSpell(10138) && character->HasSpell(5504)) character->learnSpell(10138, false); //Сотворение воды Уровень 5
            if (level >= 40 && !character->HasSpell(12825) && character->HasSpell(118)) character->learnSpell(12825, false); //Превращение Уровень 3
            if (level >= 40 && !character->HasSpell(8423) && character->HasSpell(2120)) character->learnSpell(8423, false); //Огненный столб Уровень 4
            if (level >= 40 && !character->HasSpell(8446) && character->HasSpell(2948)) character->learnSpell(8446, false); //Ожог Уровень 4
            if (level >= 40 && !character->HasSpell(8458) && character->HasSpell(543)) character->learnSpell(8458, false); //Защита от огня Уровень 3
            if (level >= 40 && !character->HasSpell(6131) && character->HasSpell(122)) character->learnSpell(6131, false); //Кольцо льда Уровень 3
            if (level >= 40 && !character->HasSpell(8417) && character->HasSpell(5143)) character->learnSpell(8417, false); //Чародейские стрелы Уровень 5
            if (level >= 40 && !character->HasSpell(7320) && character->HasSpell(7302)) character->learnSpell(7320, false); //Ледяной доспех Уровень 2
            if (level >= 42 && !character->HasSpell(10159) && character->HasSpell(120)) character->learnSpell(10159, false); //Конус холода Уровень 3
            if (level >= 42 && !character->HasSpell(12524) && character->HasSpell(11366)) character->learnSpell(12524, false); //Огненная глыба Уровень 5
            if (level >= 42 && !character->HasSpell(10169) && character->HasSpell(1008)) character->learnSpell(10169, false); //Усиление магии Уровень 3
            if (level >= 42 && !character->HasSpell(10156) && character->HasSpell(1459)) character->learnSpell(10156, false); //Чародейский интеллект Уровень 4
            if (level >= 42 && !character->HasSpell(8462) && character->HasSpell(6143)) character->learnSpell(8462, false); //Защита от магии льда Уровень 3
            if (level >= 42 && !character->HasSpell(10148) && character->HasSpell(133)) character->learnSpell(10148, false); //Огненный шар Уровень 8
            if (level >= 42 && !character->HasSpell(10144) && character->HasSpell(587)) character->learnSpell(10144, false); //Сотворение пищи Уровень 5
            if (level >= 44 && !character->HasSpell(10179) && character->HasSpell(116)) character->learnSpell(10179, false); //Ледяная стрела Уровень 8
            if (level >= 44 && !character->HasSpell(10185) && character->HasSpell(10)) character->learnSpell(10185, false); //Снежная буря Уровень 4
            if (level >= 44 && !character->HasSpell(10191) && character->HasSpell(1463)) character->learnSpell(10191, false); //Щит маны Уровень 4
            if (level >= 44 && !character->HasSpell(13019) && character->HasSpell(11113)) character->learnSpell(13019, false); //Взрывная волна Уровень 3
            if (level >= 46 && !character->HasSpell(13031) && character->HasSpell(11426)) character->learnSpell(13031, false); //Ледяная преграда Уровень 2
            if (level >= 46 && !character->HasSpell(10201) && character->HasSpell(1449)) character->learnSpell(10201, false); //Чародейский взрыв Уровень 5
            if (level >= 46 && !character->HasSpell(10197) && character->HasSpell(2136)) character->learnSpell(10197, false); //Огненный взрыв Уровень 6
            if (level >= 46 && !character->HasSpell(22782) && character->HasSpell(6117)) character->learnSpell(22782, false); //Магический доспех Уровень 2
            if (level >= 46 && !character->HasSpell(10205) && character->HasSpell(2948)) character->learnSpell(10205, false); //Ожог Уровень 5
            if (level >= 48 && !character->HasSpell(10215) && character->HasSpell(2120)) character->learnSpell(10215, false); //Огненный столб Уровень 5
            if (level >= 48 && !character->HasSpell(10211) && character->HasSpell(5143)) character->learnSpell(10211, false); //Чародейские стрелы Уровень 6
            if (level >= 48 && !character->HasSpell(10149) && character->HasSpell(133)) character->learnSpell(10149, false); //Огненный шар Уровень 9
            if (level >= 48 && !character->HasSpell(10173) && character->HasSpell(604)) character->learnSpell(10173, false); //Ослабление магии Уровень 4
            if (level >= 48 && !character->HasSpell(12525) && character->HasSpell(11366)) character->learnSpell(12525, false); //Огненная глыба Уровень 6
            if (level >= 48 && !character->HasSpell(10053) && character->HasSpell(759)) character->learnSpell(10053, false); //Сотворение самоцвета маны Уровень 3
            if (level >= 50 && !character->HasSpell(10223) && character->HasSpell(543)) character->learnSpell(10223, false); //Защита от огня Уровень 4
            if (level >= 50 && !character->HasSpell(10139) && character->HasSpell(5504)) character->learnSpell(10139, false); //Сотворение воды Уровень 6
            if (level >= 50 && !character->HasSpell(10180) && character->HasSpell(116)) character->learnSpell(10180, false); //Ледяная стрела Уровень 9
            if (level >= 50 && !character->HasSpell(10160) && character->HasSpell(120)) character->learnSpell(10160, false); //Конус холода Уровень 4
            if (level >= 50 && !character->HasSpell(10219) && character->HasSpell(7302)) character->learnSpell(10219, false); //Ледяной доспех Уровень 3
            if (level >= 52 && !character->HasSpell(10177) && character->HasSpell(6143)) character->learnSpell(10177, false); //Защита от магии льда Уровень 4
            if (level >= 52 && !character->HasSpell(13020) && character->HasSpell(11113)) character->learnSpell(13020, false); //Взрывная волна Уровень 4
            if (level >= 52 && !character->HasSpell(10145) && character->HasSpell(587)) character->learnSpell(10145, false); //Сотворение пищи Уровень 6
            if (level >= 52 && !character->HasSpell(10206) && character->HasSpell(2948)) character->learnSpell(10206, false); //Ожог Уровень 6
            if (level >= 52 && !character->HasSpell(10186) && character->HasSpell(10)) character->learnSpell(10186, false); //Снежная буря Уровень 5
            if (level >= 52 && !character->HasSpell(13032) && character->HasSpell(11426)) character->learnSpell(13032, false); //Ледяная преграда Уровень 3
            if (level >= 52 && !character->HasSpell(10192) && character->HasSpell(1463)) character->learnSpell(10192, false); //Щит маны Уровень 5
            if (level >= 54 && !character->HasSpell(10150) && character->HasSpell(133)) character->learnSpell(10150, false); //Огненный шар Уровень 10
            if (level >= 54 && !character->HasSpell(12526) && character->HasSpell(11366)) character->learnSpell(12526, false); //Огненная глыба Уровень 7
            if (level >= 54 && !character->HasSpell(10170) && character->HasSpell(1008)) character->learnSpell(10170, false); //Усиление магии Уровень 4
            if (level >= 54 && !character->HasSpell(10230) && character->HasSpell(122)) character->learnSpell(10230, false); //Кольцо льда Уровень 4
            if (level >= 54 && !character->HasSpell(10199) && character->HasSpell(2136)) character->learnSpell(10199, false); //Огненный взрыв Уровень 7
            if (level >= 54 && !character->HasSpell(10202) && character->HasSpell(1449)) character->learnSpell(10202, false); //Чародейский взрыв Уровень 6
            if (level >= 56 && !character->HasSpell(10157) && character->HasSpell(1459)) character->learnSpell(10157, false); //Чародейский интеллект Уровень 5
            if (level >= 56 && !character->HasSpell(10181) && character->HasSpell(116)) character->learnSpell(10181, false); //Ледяная стрела Уровень 10
            if (level >= 56 && !character->HasSpell(33041) && character->HasSpell(31661)) character->learnSpell(33041, false); //Дыхание дракона Уровень 2
            if (level >= 56 && !character->HasSpell(10216) && character->HasSpell(2120)) character->learnSpell(10216, false); //Огненный столб Уровень 6
            if (level >= 56 && !character->HasSpell(23028)) character->learnSpell(23028, false); //Чародейская гениальность Уровень 1
            if (level >= 56 && !character->HasSpell(10212) && character->HasSpell(5143)) character->learnSpell(10212, false); //Чародейские стрелы Уровень 7
            if (level >= 58 && !character->HasSpell(22783) && character->HasSpell(6117)) character->learnSpell(22783, false); //Магический доспех Уровень 3
            if (level >= 58 && !character->HasSpell(13033) && character->HasSpell(11426)) character->learnSpell(13033, false); //Ледяная преграда Уровень 4
            if (level >= 58 && !character->HasSpell(10207) && character->HasSpell(2948)) character->learnSpell(10207, false); //Ожог Уровень 7
            if (level >= 58 && !character->HasSpell(10161) && character->HasSpell(120)) character->learnSpell(10161, false); //Конус холода Уровень 5
            if (level >= 58 && !character->HasSpell(10054) && character->HasSpell(759)) character->learnSpell(10054, false); //Сотворение самоцвета маны Уровень 4
            if (level >= 60 && !character->HasSpell(28609) && character->HasSpell(6143)) character->learnSpell(28609, false); //Защита от магии льда Уровень 5
            if (level >= 60 && !character->HasSpell(28612) && character->HasSpell(587)) character->learnSpell(28612, false); //Сотворение пищи Уровень 7
            if (level >= 60 && !character->HasSpell(25345) && character->HasSpell(5143)) character->learnSpell(25345, false); //Чародейские стрелы Уровень 8
            if (level >= 60 && !character->HasSpell(25304) && character->HasSpell(116)) character->learnSpell(25304, false); //Ледяная стрела Уровень 11
            if (level >= 60 && !character->HasSpell(10140) && character->HasSpell(5504)) character->learnSpell(10140, false); //Сотворение воды Уровень 7
            if (level >= 60 && !character->HasSpell(10151) && character->HasSpell(133)) character->learnSpell(10151, false); //Огненный шар Уровень 11
            if (level >= 60 && !character->HasSpell(10174) && character->HasSpell(604)) character->learnSpell(10174, false); //Ослабление магии Уровень 5
            if (level >= 60 && !character->HasSpell(18809) && character->HasSpell(11366)) character->learnSpell(18809, false); //Огненная глыба Уровень 8
            if (level >= 60 && !character->HasSpell(10220) && character->HasSpell(7302)) character->learnSpell(10220, false); //Ледяной доспех Уровень 4
            if (level >= 60 && !character->HasSpell(10225) && character->HasSpell(543)) character->learnSpell(10225, false); //Защита от огня Уровень 5
            if (level >= 60 && !character->HasSpell(12826) && character->HasSpell(118)) character->learnSpell(12826, false); //Превращение Уровень 4
            if (level >= 60 && !character->HasSpell(10193) && character->HasSpell(1463)) character->learnSpell(10193, false); //Щит маны Уровень 6
            if (level >= 60 && !character->HasSpell(10187) && character->HasSpell(10)) character->learnSpell(10187, false); //Снежная буря Уровень 6
            if (level >= 60 && !character->HasSpell(13021) && character->HasSpell(11113)) character->learnSpell(13021, false); //Взрывная волна Уровень 5
            if (level >= 61 && !character->HasSpell(27078) && character->HasSpell(2136)) character->learnSpell(27078, false); //Огненный взрыв Уровень 8
            if (level >= 62 && !character->HasSpell(25306) && character->HasSpell(133)) character->learnSpell(25306, false); //Огненный шар Уровень 12
            if (level >= 62 && !character->HasSpell(30482)) character->learnSpell(30482, false); //Раскаленный доспех Уровень 1
            if (level >= 62 && !character->HasSpell(27080) && character->HasSpell(1449)) character->learnSpell(27080, false); //Чародейский взрыв Уровень 7
            if (level >= 63 && !character->HasSpell(27075) && character->HasSpell(5143)) character->learnSpell(27075, false); //Чародейские стрелы Уровень 9
            if (level >= 63 && !character->HasSpell(27130) && character->HasSpell(1008)) character->learnSpell(27130, false); //Усиление магии Уровень 5
            if (level >= 63 && !character->HasSpell(27071) && character->HasSpell(116)) character->learnSpell(27071, false); //Ледяная стрела Уровень 12
            if (level >= 64 && !character->HasSpell(30451)) character->learnSpell(30451, false); //Чародейская вспышка Уровень 1
            if (level >= 64 && !character->HasSpell(27134) && character->HasSpell(11426)) character->learnSpell(27134, false); //Ледяная преграда Уровень 5
            if (level >= 64 && !character->HasSpell(33042) && character->HasSpell(31661)) character->learnSpell(33042, false); //Дыхание дракона Уровень 3
            if (level >= 64 && !character->HasSpell(27086) && character->HasSpell(2120)) character->learnSpell(27086, false); //Огненный столб Уровень 7
            if (level >= 65 && !character->HasSpell(27087) && character->HasSpell(120)) character->learnSpell(27087, false); //Конус холода Уровень 6
            if (level >= 65 && !character->HasSpell(27133) && character->HasSpell(11113)) character->learnSpell(27133, false); //Взрывная волна Уровень 6
            if (level >= 65 && !character->HasSpell(27073) && character->HasSpell(2948)) character->learnSpell(27073, false); //Ожог Уровень 8
            if (level >= 65 && !character->HasSpell(37420) && character->HasSpell(5504)) character->learnSpell(37420, false); //Сотворение воды Уровень 8
            if (level >= 66 && !character->HasSpell(27070) && character->HasSpell(133)) character->learnSpell(27070, false); //Огненный шар Уровень 13
            if (level >= 66 && !character->HasSpell(30455)) character->learnSpell(30455, false); //Ледяное копье Уровень 1
            if (level >= 66 && !character->HasSpell(27132) && character->HasSpell(11366)) character->learnSpell(27132, false); //Огненная глыба Уровень 9
            if (level >= 67 && !character->HasSpell(33944) && character->HasSpell(604)) character->learnSpell(33944, false); //Ослабление магии Уровень 6
            if (level >= 67 && !character->HasSpell(27088) && character->HasSpell(122)) character->learnSpell(27088, false); //Кольцо льда Уровень 5
            if (level >= 68 && !character->HasSpell(66)) character->learnSpell(66, false); //Невидимость 
            if (level >= 68 && !character->HasSpell(27085) && character->HasSpell(10)) character->learnSpell(27085, false); //Снежная буря Уровень 7
            if (level >= 68 && !character->HasSpell(27101) && character->HasSpell(759)) character->learnSpell(27101, false); //Сотворение самоцвета маны Уровень 5
            if (level >= 68 && !character->HasSpell(27131) && character->HasSpell(1463)) character->learnSpell(27131, false); //Щит маны Уровень 7
            if (level >= 69 && !character->HasSpell(38699) && character->HasSpell(5143)) character->learnSpell(38699, false); //Чародейские стрелы Уровень 10
            if (level >= 69 && !character->HasSpell(33946) && character->HasSpell(1008)) character->learnSpell(33946, false); //Усиление магии Уровень 6
            if (level >= 69 && !character->HasSpell(27124) && character->HasSpell(7302)) character->learnSpell(27124, false); //Ледяной доспех Уровень 5
            if (level >= 69 && !character->HasSpell(27128) && character->HasSpell(543)) character->learnSpell(27128, false); //Защита от огня Уровень 6
            if (level >= 69 && !character->HasSpell(27072) && character->HasSpell(116)) character->learnSpell(27072, false); //Ледяная стрела Уровень 13
            if (level >= 69 && !character->HasSpell(27125) && character->HasSpell(6117)) character->learnSpell(27125, false); //Магический доспех Уровень 4
            if (level >= 70 && !character->HasSpell(27079) && character->HasSpell(2136)) character->learnSpell(27079, false); //Огненный взрыв Уровень 9
            if (level >= 70 && !character->HasSpell(38692) && character->HasSpell(133)) character->learnSpell(38692, false); //Огненный шар Уровень 14
            if (level >= 70 && !character->HasSpell(38697) && character->HasSpell(116)) character->learnSpell(38697, false); //Ледяная стрела Уровень 14
            if (level >= 70 && !character->HasSpell(27074) && character->HasSpell(2948)) character->learnSpell(27074, false); //Ожог Уровень 9
            if (level >= 70 && !character->HasSpell(38704) && character->HasSpell(5143)) character->learnSpell(38704, false); //Чародейские стрелы Уровень 11
            if (level >= 70 && !character->HasSpell(30449)) character->learnSpell(30449, false); //Чарокрад 
            if (level >= 70 && !character->HasSpell(55359) && character->HasSpell(44457)) character->learnSpell(55359, false); //Живая бомба Уровень 2
            if (level >= 70 && !character->HasSpell(44780) && character->HasSpell(44425)) character->learnSpell(44780, false); //Чародейский обстрел Уровень 2
            if (level >= 70 && !character->HasSpell(27082) && character->HasSpell(1449)) character->learnSpell(27082, false); //Чародейский взрыв Уровень 8
            if (level >= 70 && !character->HasSpell(27090) && character->HasSpell(5504)) character->learnSpell(27090, false); //Сотворение воды Уровень 9
            if (level >= 70 && !character->HasSpell(27127) && character->HasSpell(23028)) character->learnSpell(27127, false); //Чародейская гениальность Уровень 2
            if (level >= 70 && !character->HasSpell(27126) && character->HasSpell(1459)) character->learnSpell(27126, false); //Чародейский интеллект Уровень 6
            if (level >= 70 && !character->HasSpell(32796) && character->HasSpell(6143)) character->learnSpell(32796, false); //Защита от магии льда Уровень 6
            if (level >= 70 && !character->HasSpell(33043) && character->HasSpell(31661)) character->learnSpell(33043, false); //Дыхание дракона Уровень 4
            if (level >= 70 && !character->HasSpell(33405) && character->HasSpell(11426)) character->learnSpell(33405, false); //Ледяная преграда Уровень 6
            if (level >= 70 && !character->HasSpell(43987)) character->learnSpell(43987, false); //Обряд сотворения яств Уровень 1
            if (level >= 70 && !character->HasSpell(33938) && character->HasSpell(11366)) character->learnSpell(33938, false); //Огненная глыба Уровень 10
            if (level >= 70 && !character->HasSpell(33933) && character->HasSpell(11113)) character->learnSpell(33933, false); //Взрывная волна Уровень 7
            if (level >= 70 && !character->HasSpell(33717) && character->HasSpell(587)) character->learnSpell(33717, false); //Сотворение пищи Уровень 8
            if (level >= 71 && !character->HasSpell(43045) && character->HasSpell(30482)) character->learnSpell(43045, false); //Раскаленный доспех Уровень 2
            if (level >= 71 && !character->HasSpell(43023) && character->HasSpell(6117)) character->learnSpell(43023, false); //Магический доспех Уровень 5
            if (level >= 71 && !character->HasSpell(42894) && character->HasSpell(30451)) character->learnSpell(42894, false); //Чародейская вспышка Уровень 2
            if (level >= 72 && !character->HasSpell(42930) && character->HasSpell(120)) character->learnSpell(42930, false); //Конус холода Уровень 7
            if (level >= 72 && !character->HasSpell(42913) && character->HasSpell(30455)) character->learnSpell(42913, false); //Ледяное копье Уровень 2
            if (level >= 72 && !character->HasSpell(42925) && character->HasSpell(2120)) character->learnSpell(42925, false); //Огненный столб Уровень 8
            if (level >= 73 && !character->HasSpell(43019) && character->HasSpell(1463)) character->learnSpell(43019, false); //Щит маны Уровень 8
            if (level >= 73 && !character->HasSpell(42858) && character->HasSpell(2948)) character->learnSpell(42858, false); //Ожог Уровень 10
            if (level >= 73 && !character->HasSpell(42890) && character->HasSpell(11366)) character->learnSpell(42890, false); //Огненная глыба Уровень 11
            if (level >= 74 && !character->HasSpell(42872) && character->HasSpell(2136)) character->learnSpell(42872, false); //Огненный взрыв Уровень 10
            if (level >= 74 && !character->HasSpell(42939) && character->HasSpell(10)) character->learnSpell(42939, false); //Снежная буря Уровень 8
            if (level >= 74 && !character->HasSpell(42832) && character->HasSpell(133)) character->learnSpell(42832, false); //Огненный шар Уровень 15
            if (level >= 75 && !character->HasSpell(43038) && character->HasSpell(11426)) character->learnSpell(43038, false); //Ледяная преграда Уровень 7
            if (level >= 75 && !character->HasSpell(42955)) character->learnSpell(42955, false); //Сотворение яств Уровень 1
            if (level >= 75 && !character->HasSpell(44614)) character->learnSpell(44614, false); //Стрела ледяного огня Уровень 1
            if (level >= 75 && !character->HasSpell(42944) && character->HasSpell(11113)) character->learnSpell(42944, false); //Взрывная волна Уровень 8
            if (level >= 75 && !character->HasSpell(42949) && character->HasSpell(31661)) character->learnSpell(42949, false); //Дыхание дракона Уровень 5
            if (level >= 75 && !character->HasSpell(42841) && character->HasSpell(116)) character->learnSpell(42841, false); //Ледяная стрела Уровень 15
            if (level >= 75 && !character->HasSpell(42843) && character->HasSpell(5143)) character->learnSpell(42843, false); //Чародейские стрелы Уровень 12
            if (level >= 75 && !character->HasSpell(42917) && character->HasSpell(122)) character->learnSpell(42917, false); //Кольцо льда Уровень 6
            if (level >= 76 && !character->HasSpell(43015) && character->HasSpell(604)) character->learnSpell(43015, false); //Ослабление магии Уровень 7
            if (level >= 76 && !character->HasSpell(42896) && character->HasSpell(30451)) character->learnSpell(42896, false); //Чародейская вспышка Уровень 3
            if (level >= 76 && !character->HasSpell(42920) && character->HasSpell(1449)) character->learnSpell(42920, false); //Чародейский взрыв Уровень 9
            if (level >= 77 && !character->HasSpell(42891) && character->HasSpell(11366)) character->learnSpell(42891, false); //Огненная глыба Уровень 12
            if (level >= 77 && !character->HasSpell(42985) && character->HasSpell(759)) character->learnSpell(42985, false); //Сотворение самоцвета маны Уровень 6
            if (level >= 77 && !character->HasSpell(43017) && character->HasSpell(1008)) character->learnSpell(43017, false); //Усиление магии Уровень 7
            if (level >= 78 && !character->HasSpell(42859) && character->HasSpell(2948)) character->learnSpell(42859, false); //Ожог Уровень 11
            if (level >= 78 && !character->HasSpell(42914) && character->HasSpell(30455)) character->learnSpell(42914, false); //Ледяное копье Уровень 3
            if (level >= 78 && !character->HasSpell(43010) && character->HasSpell(543)) character->learnSpell(43010, false); //Защита от огня Уровень 7
            if (level >= 78 && !character->HasSpell(42833) && character->HasSpell(133)) character->learnSpell(42833, false); //Огненный шар Уровень 16
            if (level >= 79 && !character->HasSpell(43046) && character->HasSpell(30482)) character->learnSpell(43046, false); //Раскаленный доспех Уровень 3
            if (level >= 79 && !character->HasSpell(42842) && character->HasSpell(116)) character->learnSpell(42842, false); //Ледяная стрела Уровень 16
            if (level >= 79 && !character->HasSpell(43020) && character->HasSpell(1463)) character->learnSpell(43020, false); //Щит маны Уровень 9
            if (level >= 79 && !character->HasSpell(42931) && character->HasSpell(120)) character->learnSpell(42931, false); //Конус холода Уровень 8
            if (level >= 79 && !character->HasSpell(42926) && character->HasSpell(2120)) character->learnSpell(42926, false); //Огненный столб Уровень 9
            if (level >= 79 && !character->HasSpell(43012) && character->HasSpell(6143)) character->learnSpell(43012, false); //Защита от магии льда Уровень 7
            if (level >= 79 && !character->HasSpell(43008) && character->HasSpell(7302)) character->learnSpell(43008, false); //Ледяной доспех Уровень 6
            if (level >= 79 && !character->HasSpell(42846) && character->HasSpell(5143)) character->learnSpell(42846, false); //Чародейские стрелы Уровень 13
            if (level >= 79 && !character->HasSpell(43024) && character->HasSpell(6117)) character->learnSpell(43024, false); //Магический доспех Уровень 6
            if (level >= 80 && !character->HasSpell(42940) && character->HasSpell(10)) character->learnSpell(42940, false); //Снежная буря Уровень 9
            if (level >= 80 && !character->HasSpell(44781) && character->HasSpell(44425)) character->learnSpell(44781, false); //Чародейский обстрел Уровень 3
            if (level >= 80 && !character->HasSpell(55360) && character->HasSpell(44457)) character->learnSpell(55360, false); //Живая бомба Уровень 3
            if (level >= 80 && !character->HasSpell(47610) && character->HasSpell(44614)) character->learnSpell(47610, false); // unk
            if (level >= 80 && !character->HasSpell(55342)) character->learnSpell(55342, false); //Зеркальное изображение 
            if (level >= 80 && !character->HasSpell(43039) && character->HasSpell(11426)) character->learnSpell(43039, false); //Ледяная преграда Уровень 8
            if (level >= 80 && !character->HasSpell(42956) && character->HasSpell(42955)) character->learnSpell(42956, false); //Сотворение яств Уровень 2
            if (level >= 80 && !character->HasSpell(42921) && character->HasSpell(1449)) character->learnSpell(42921, false); //Чародейский взрыв Уровень 10
            if (level >= 80 && !character->HasSpell(42995) && character->HasSpell(1459)) character->learnSpell(42995, false); //Чародейский интеллект Уровень 7
            if (level >= 80 && !character->HasSpell(43002) && character->HasSpell(23028)) character->learnSpell(43002, false); //Чародейская гениальность Уровень 3
            if (level >= 80 && !character->HasSpell(42897) && character->HasSpell(30451)) character->learnSpell(42897, false); //Чародейская вспышка Уровень 4
            if (level >= 80 && !character->HasSpell(42873) && character->HasSpell(2136)) character->learnSpell(42873, false); //Огненный взрыв Уровень 11
            if (level >= 80 && !character->HasSpell(42945) && character->HasSpell(11113)) character->learnSpell(42945, false); //Взрывная волна Уровень 9
            if (level >= 80 && !character->HasSpell(42950) && character->HasSpell(31661)) character->learnSpell(42950, false); //Дыхание дракона Уровень 6
            if (level >= 80 && !character->HasSpell(58659) && character->HasSpell(43987)) character->learnSpell(58659, false); //Обряд сотворения яств Уровень 2
    }

    void learnSpellWarlock()
    {

            if (!character->HasSpell(348)) character->learnSpell(348, false); //Жертвенный огонь Уровень 1
            if (!character->HasSpell(688)) character->learnSpell(688, false); //Призыв беса Призыв

            if (level >= 4 && !character->HasSpell(172)) character->learnSpell(172, false); //Порча Уровень 1
            if (level >= 4 && !character->HasSpell(702)) character->learnSpell(702, false); //Проклятие слабости Уровень 1
            if (level >= 6 && !character->HasSpell(695) && character->HasSpell(686)) character->learnSpell(695, false); //Стрела Тьмы Уровень 2
            if (level >= 6 && !character->HasSpell(1454)) character->learnSpell(1454, false); //Жизнеотвод Уровень 1
            if (level >= 8 && !character->HasSpell(980)) character->learnSpell(980, false); //Проклятие агонии Уровень 1
            if (level >= 8 && !character->HasSpell(5782)) character->learnSpell(5782, false); //Страх Уровень 1

            if (level < 10) return;

            if (!character->HasSpell(697)) character->learnSpell(697, false); //Призыв демона Бездны

            if (!character->HasSpell(6201)) character->learnSpell(6201, false); //Создание камня здоровья Уровень 1
            if (!character->HasSpell(707) && character->HasSpell(348)) character->learnSpell(707, false); //Жертвенный огонь Уровень 2
            if (!character->HasSpell(696) && character->HasSpell(687)) character->learnSpell(696, false); //Шкура демона Уровень 2
            if (!character->HasSpell(1120)) character->learnSpell(1120, false); //Похищение души Уровень 1

            if (level >= 12 && !character->HasSpell(1108) && character->HasSpell(702)) character->learnSpell(1108, false); //Проклятие слабости Уровень 2
            if (level >= 12 && !character->HasSpell(705) && character->HasSpell(686)) character->learnSpell(705, false); //Стрела Тьмы Уровень 3
            if (level >= 12 && !character->HasSpell(755)) character->learnSpell(755, false); //Канал здоровья Уровень 1
            if (level >= 14 && !character->HasSpell(6222) && character->HasSpell(172)) character->learnSpell(6222, false); //Порча Уровень 2
            if (level >= 14 && !character->HasSpell(689)) character->learnSpell(689, false); //Похищение жизни Уровень 1
            if (level >= 16 && !character->HasSpell(1455) && character->HasSpell(1454)) character->learnSpell(1455, false); //Жизнеотвод Уровень 2
            if (level >= 16 && !character->HasSpell(5697)) character->learnSpell(5697, false); //Бесконечное дыхание 
            if (level >= 18 && !character->HasSpell(5676)) character->learnSpell(5676, false); //Жгучая боль Уровень 1
            if (level >= 18 && !character->HasSpell(1014) && character->HasSpell(980)) character->learnSpell(1014, false); //Проклятие агонии Уровень 2
            if (level >= 18 && !character->HasSpell(693)) character->learnSpell(693, false); //Создание камня души Уровень 1

            if (level < 20) return;

            if (!character->HasSpell(712)) character->learnSpell(712, false); //Призыв суккуба

            if (!character->HasSpell(706)) character->learnSpell(706, false); //Демонический доспех Уровень 1
            if (!character->HasSpell(1710)) character->learnSpell(1710, false); //Призывание коня Скверны Призыв
            if (!character->HasSpell(5740)) character->learnSpell(5740, false); //Огненный ливень Уровень 1
            if (!character->HasSpell(1094) && character->HasSpell(348)) character->learnSpell(1094, false); //Жертвенный огонь Уровень 3
            if (!character->HasSpell(1088) && character->HasSpell(686)) character->learnSpell(1088, false); //Стрела Тьмы Уровень 4
            if (!character->HasSpell(3698) && character->HasSpell(755)) character->learnSpell(3698, false); //Канал здоровья Уровень 2
            if (!character->HasSpell(698)) character->learnSpell(698, false); //Ритуал призыва 
            
            
            if (level >= 22 && !character->HasSpell(126)) character->learnSpell(126, false); //Око Килрогга Призыв
            if (level >= 22 && !character->HasSpell(699) && character->HasSpell(689)) character->learnSpell(699, false); //Похищение жизни Уровень 2
            if (level >= 22 && !character->HasSpell(6202) && character->HasSpell(6201)) character->learnSpell(6202, false); //Создание камня здоровья Уровень 2
            if (level >= 22 && !character->HasSpell(6205) && character->HasSpell(702)) character->learnSpell(6205, false); //Проклятие слабости Уровень 3
            if (level >= 24 && !character->HasSpell(5500)) character->learnSpell(5500, false); //Чутье на демонов 
            if (level >= 24 && !character->HasSpell(18867) && character->HasSpell(17877)) character->learnSpell(18867, false); //Ожог Тьмы Уровень 2
            if (level >= 24 && !character->HasSpell(5138)) character->learnSpell(5138, false); //Похищение маны 
            if (level >= 24 && !character->HasSpell(6223) && character->HasSpell(172)) character->learnSpell(6223, false); //Порча Уровень 3
            if (level >= 24 && !character->HasSpell(8288) && character->HasSpell(1120)) character->learnSpell(8288, false); //Похищение души Уровень 2
            if (level >= 26 && !character->HasSpell(1714)) character->learnSpell(1714, false); //Проклятие косноязычия Уровень 1
            if (level >= 26 && !character->HasSpell(17919) && character->HasSpell(5676)) character->learnSpell(17919, false); //Жгучая боль Уровень 2
            if (level >= 26 && !character->HasSpell(132)) character->learnSpell(132, false); //Обнаружение невидимости 
            if (level >= 26 && !character->HasSpell(1456) && character->HasSpell(1454)) character->learnSpell(1456, false); //Жизнеотвод Уровень 3
            if (level >= 28 && !character->HasSpell(6217) && character->HasSpell(980)) character->learnSpell(6217, false); //Проклятие агонии Уровень 3
            if (level >= 28 && !character->HasSpell(6366)) character->learnSpell(6366, false); //Создание камня огня Уровень 1
            if (level >= 28 && !character->HasSpell(3699) && character->HasSpell(755)) character->learnSpell(3699, false); //Канал здоровья Уровень 3
            if (level >= 28 && !character->HasSpell(1106) && character->HasSpell(686)) character->learnSpell(1106, false); //Стрела Тьмы Уровень 5
            if (level >= 28 && !character->HasSpell(710)) character->learnSpell(710, false); //Изгнание Уровень 1

            if (level < 30) return;

            if (!character->HasSpell(691)) character->learnSpell(691, false); //Призыв охотника Скверны
            
            if (!character->HasSpell(1098)) character->learnSpell(1098, false); //Порабощение демона Уровень 1
            if (!character->HasSpell(709) && character->HasSpell(689)) character->learnSpell(709, false); //Похищение жизни Уровень 3
            if (!character->HasSpell(1086) && character->HasSpell(706)) character->learnSpell(1086, false); //Демонический доспех Уровень 2
            if (!character->HasSpell(1949)) character->learnSpell(1949, false); //Адское Пламя Уровень 1
            if (!character->HasSpell(2941) && character->HasSpell(348)) character->learnSpell(2941, false); //Жертвенный огонь Уровень 4
            if (!character->HasSpell(20752) && character->HasSpell(693)) character->learnSpell(20752, false); //Создание камня души Уровень 2
            
            if (level >= 32 && !character->HasSpell(6229)) character->learnSpell(6229, false); //Заслон от темной магии Уровень 1
            if (level >= 32 && !character->HasSpell(18868) && character->HasSpell(17877)) character->learnSpell(18868, false); //Ожог Тьмы Уровень 3
            if (level >= 32 && !character->HasSpell(6213) && character->HasSpell(5782)) character->learnSpell(6213, false); //Страх Уровень 2
            if (level >= 32 && !character->HasSpell(1490)) character->learnSpell(1490, false); //Проклятие стихий Уровень 1
            if (level >= 32 && !character->HasSpell(7646) && character->HasSpell(702)) character->learnSpell(7646, false); //Проклятие слабости Уровень 4
            if (level >= 34 && !character->HasSpell(7648) && character->HasSpell(172)) character->learnSpell(7648, false); //Порча Уровень 4
            if (level >= 34 && !character->HasSpell(17920) && character->HasSpell(5676)) character->learnSpell(17920, false); //Жгучая боль Уровень 3
            if (level >= 34 && !character->HasSpell(6219) && character->HasSpell(5740)) character->learnSpell(6219, false); //Огненный ливень Уровень 2
            if (level >= 34 && !character->HasSpell(5699) && character->HasSpell(6201)) character->learnSpell(5699, false); //Создание камня здоровья Уровень 3
            if (level >= 36 && !character->HasSpell(17951) && character->HasSpell(6366)) character->learnSpell(17951, false); //Создание камня огня Уровень 2
            if (level >= 36 && !character->HasSpell(3700) && character->HasSpell(755)) character->learnSpell(3700, false); //Канал здоровья Уровень 4
            if (level >= 36 && !character->HasSpell(11687) && character->HasSpell(1454)) character->learnSpell(11687, false); //Жизнеотвод Уровень 4
            if (level >= 36 && !character->HasSpell(2362)) character->learnSpell(2362, false); //Создание камня чар Уровень 1
            if (level >= 36 && !character->HasSpell(7641) && character->HasSpell(686)) character->learnSpell(7641, false); //Стрела Тьмы Уровень 6
            if (level >= 38 && !character->HasSpell(8289) && character->HasSpell(1120)) character->learnSpell(8289, false); //Похищение души Уровень 3
            if (level >= 38 && !character->HasSpell(11711) && character->HasSpell(980)) character->learnSpell(11711, false); //Проклятие агонии Уровень 4
            if (level >= 38 && !character->HasSpell(7651) && character->HasSpell(689)) character->learnSpell(7651, false); //Похищение жизни Уровень 4
            if (level >= 40 && !character->HasSpell(11733) && character->HasSpell(706)) character->learnSpell(11733, false); //Демонический доспех Уровень 3
            if (level >= 40 && !character->HasSpell(5484)) character->learnSpell(5484, false); //Вой ужаса Уровень 1
            if (level >= 40 && !character->HasSpell(11665) && character->HasSpell(348)) character->learnSpell(11665, false); //Жертвенный огонь Уровень 5
            if (level >= 40 && !character->HasSpell(20755) && character->HasSpell(693)) character->learnSpell(20755, false); //Создание камня души Уровень 3
            if (level >= 40 && !character->HasSpell(18869) && character->HasSpell(17877)) character->learnSpell(18869, false); //Ожог Тьмы Уровень 4
            //if (level >= 40 && !character->HasSpell(23160)) character->learnSpell(23160, false); //Изучить призывание коня погибели Призыв
            if (level >= 42 && !character->HasSpell(11739) && character->HasSpell(6229)) character->learnSpell(11739, false); //Заслон от темной магии Уровень 2
            if (level >= 42 && !character->HasSpell(11683) && character->HasSpell(1949)) character->learnSpell(11683, false); //Адское Пламя Уровень 2
            if (level >= 42 && !character->HasSpell(17921) && character->HasSpell(5676)) character->learnSpell(17921, false); //Жгучая боль Уровень 4
            if (level >= 42 && !character->HasSpell(11707) && character->HasSpell(702)) character->learnSpell(11707, false); //Проклятие слабости Уровень 5
            if (level >= 42 && !character->HasSpell(6789)) character->learnSpell(6789, false); //Лик смерти Уровень 1
            if (level >= 44 && !character->HasSpell(11671) && character->HasSpell(172)) character->learnSpell(11671, false); //Порча Уровень 5
            if (level >= 44 && !character->HasSpell(11725) && character->HasSpell(1098)) character->learnSpell(11725, false); //Порабощение демона Уровень 2
            if (level >= 44 && !character->HasSpell(11659) && character->HasSpell(686)) character->learnSpell(11659, false); //Стрела Тьмы Уровень 7
            if (level >= 44 && !character->HasSpell(11693) && character->HasSpell(755)) character->learnSpell(11693, false); //Канал здоровья Уровень 5
            if (level >= 46 && !character->HasSpell(17952) && character->HasSpell(6366)) character->learnSpell(17952, false); //Создание камня огня Уровень 3
            if (level >= 46 && !character->HasSpell(11729) && character->HasSpell(6201)) character->learnSpell(11729, false); //Создание камня здоровья Уровень 4
            if (level >= 46 && !character->HasSpell(11721) && character->HasSpell(1490)) character->learnSpell(11721, false); //Проклятие стихий Уровень 2
            if (level >= 46 && !character->HasSpell(11688) && character->HasSpell(1454)) character->learnSpell(11688, false); //Жизнеотвод Уровень 5
            if (level >= 46 && !character->HasSpell(11677) && character->HasSpell(5740)) character->learnSpell(11677, false); //Огненный ливень Уровень 3
            if (level >= 46 && !character->HasSpell(11699) && character->HasSpell(689)) character->learnSpell(11699, false); //Похищение жизни Уровень 5
            if (level >= 48 && !character->HasSpell(17727) && character->HasSpell(2362)) character->learnSpell(17727, false); //Создание камня чар Уровень 2
            if (level >= 48 && !character->HasSpell(18870) && character->HasSpell(17877)) character->learnSpell(18870, false); //Ожог Тьмы Уровень 5
            if (level >= 48 && !character->HasSpell(18647) && character->HasSpell(710)) character->learnSpell(18647, false); //Изгнание Уровень 2
            if (level >= 48 && !character->HasSpell(6353)) character->learnSpell(6353, false); //Ожог души Уровень 1
            if (level >= 48 && !character->HasSpell(11712) && character->HasSpell(980)) character->learnSpell(11712, false); //Проклятие агонии Уровень 5

            if (level < 50) return;

            if (!character->HasSpell(1122)) character->learnSpell(1122, false); //Призыв инфернала
            
            if (!character->HasSpell(17925) && character->HasSpell(6789)) character->learnSpell(17925, false); //Лик смерти Уровень 2
            if (!character->HasSpell(11734) && character->HasSpell(706)) character->learnSpell(11734, false); //Демонический доспех Уровень 4
            if (!character->HasSpell(11719) && character->HasSpell(1714)) character->learnSpell(11719, false); //Проклятие косноязычия Уровень 2
            if (!character->HasSpell(20756) && character->HasSpell(693)) character->learnSpell(20756, false); //Создание камня души Уровень 4
            if (!character->HasSpell(11667) && character->HasSpell(348)) character->learnSpell(11667, false); //Жертвенный огонь Уровень 6
            if (!character->HasSpell(18937) && character->HasSpell(18220)) character->learnSpell(18937, false); //Темный союз Уровень 2
            if (!character->HasSpell(17922) && character->HasSpell(5676)) character->learnSpell(17922, false); //Жгучая боль Уровень 5
            
            if (level >= 52 && !character->HasSpell(11740) && character->HasSpell(6229)) character->learnSpell(11740, false); //Заслон от темной магии Уровень 3
            if (level >= 52 && !character->HasSpell(11660) && character->HasSpell(686)) character->learnSpell(11660, false); //Стрела Тьмы Уровень 8
            if (level >= 52 && !character->HasSpell(11675) && character->HasSpell(1120)) character->learnSpell(11675, false); //Похищение души Уровень 4
            if (level >= 52 && !character->HasSpell(11708) && character->HasSpell(702)) character->learnSpell(11708, false); //Проклятие слабости Уровень 6
            if (level >= 52 && !character->HasSpell(11694) && character->HasSpell(755)) character->learnSpell(11694, false); //Канал здоровья Уровень 6
            if (level >= 54 && !character->HasSpell(17928) && character->HasSpell(5484)) character->learnSpell(17928, false); //Вой ужаса Уровень 2
            if (level >= 54 && !character->HasSpell(11672) && character->HasSpell(172)) character->learnSpell(11672, false); //Порча Уровень 6
            if (level >= 54 && !character->HasSpell(11700) && character->HasSpell(689)) character->learnSpell(11700, false); //Похищение жизни Уровень 6
            if (level >= 54 && !character->HasSpell(11684) && character->HasSpell(1949)) character->learnSpell(11684, false); //Адское Пламя Уровень 3
            if (level >= 56 && !character->HasSpell(6215) && character->HasSpell(5782)) character->learnSpell(6215, false); //Страх Уровень 3
            if (level >= 56 && !character->HasSpell(17924) && character->HasSpell(6353)) character->learnSpell(17924, false); //Ожог души Уровень 2
            if (level >= 56 && !character->HasSpell(11689) && character->HasSpell(1454)) character->learnSpell(11689, false); //Жизнеотвод Уровень 6
            if (level >= 56 && !character->HasSpell(17953) && character->HasSpell(6366)) character->learnSpell(17953, false); //Создание камня огня Уровень 4
            if (level >= 56 && !character->HasSpell(18871) && character->HasSpell(17877)) character->learnSpell(18871, false); //Ожог Тьмы Уровень 6
            if (level >= 58 && !character->HasSpell(11678) && character->HasSpell(5740)) character->learnSpell(11678, false); //Огненный ливень Уровень 4
            if (level >= 58 && !character->HasSpell(17923) && character->HasSpell(5676)) character->learnSpell(17923, false); //Жгучая боль Уровень 6
            if (level >= 58 && !character->HasSpell(17926) && character->HasSpell(6789)) character->learnSpell(17926, false); //Лик смерти Уровень 3
            if (level >= 58 && !character->HasSpell(11730) && character->HasSpell(6201)) character->learnSpell(11730, false); //Создание камня здоровья Уровень 5
            if (level >= 58 && !character->HasSpell(11713) && character->HasSpell(980)) character->learnSpell(11713, false); //Проклятие агонии Уровень 6
            if (level >= 58 && !character->HasSpell(11726) && character->HasSpell(1098)) character->learnSpell(11726, false); //Порабощение демона Уровень 3
            if (level >= 60 && !character->HasSpell(20757) && character->HasSpell(693)) character->learnSpell(20757, false); //Создание камня души Уровень 5
            if (level >= 60 && !character->HasSpell(30404) && character->HasSpell(30108)) character->learnSpell(30404, false); //Нестабильное колдовство Уровень 2
            if (level >= 60 && !character->HasSpell(603)) character->learnSpell(603, false); //Проклятие рока Уровень 1
            if (level >= 60 && !character->HasSpell(28610) && character->HasSpell(6229)) character->learnSpell(28610, false); //Заслон от темной магии Уровень 4
            if (level >= 60 && !character->HasSpell(25309) && character->HasSpell(348)) character->learnSpell(25309, false); //Жертвенный огонь Уровень 8
            if (level >= 60 && !character->HasSpell(30413) && character->HasSpell(30283)) character->learnSpell(30413, false); //Неистовство Тьмы Уровень 2
            if (level >= 60 && !character->HasSpell(25311) && character->HasSpell(172)) character->learnSpell(25311, false); //Порча Уровень 7
            if (level >= 60 && !character->HasSpell(18938) && character->HasSpell(18220)) character->learnSpell(18938, false); //Темный союз Уровень 3
            if (level >= 60 && !character->HasSpell(11695) && character->HasSpell(755)) character->learnSpell(11695, false); //Канал здоровья Уровень 7
            if (level >= 60 && !character->HasSpell(11735) && character->HasSpell(706)) character->learnSpell(11735, false); //Демонический доспех Уровень 5
            if (level >= 60 && !character->HasSpell(11668) && character->HasSpell(348)) character->learnSpell(11668, false); //Жертвенный огонь Уровень 7
            if (level >= 60 && !character->HasSpell(11722) && character->HasSpell(1490)) character->learnSpell(11722, false); //Проклятие стихий Уровень 3
            if (level >= 60 && !character->HasSpell(11661) && character->HasSpell(686)) character->learnSpell(11661, false); //Стрела Тьмы Уровень 9
            if (level >= 60 && !character->HasSpell(17728) && character->HasSpell(2362)) character->learnSpell(17728, false); //Создание камня чар Уровень 3
            if (level >= 61 && !character->HasSpell(27224) && character->HasSpell(702)) character->learnSpell(27224, false); //Проклятие слабости Уровень 7
            if (level >= 62 && !character->HasSpell(25307) && character->HasSpell(686)) character->learnSpell(25307, false); //Стрела Тьмы Уровень 10
            if (level >= 62 && !character->HasSpell(28176)) character->learnSpell(28176, false); //Доспех Скверны Уровень 1
            if (level >= 62 && !character->HasSpell(27219) && character->HasSpell(689)) character->learnSpell(27219, false); //Похищение жизни Уровень 7
            if (level >= 63 && !character->HasSpell(27263) && character->HasSpell(17877)) character->learnSpell(27263, false); //Ожог Тьмы Уровень 7
            if (level >= 64 && !character->HasSpell(29722)) character->learnSpell(29722, false); //Испепеление Уровень 1
            if (level >= 64 && !character->HasSpell(27211) && character->HasSpell(6353)) character->learnSpell(27211, false); //Ожог души Уровень 3
            if (level >= 65 && !character->HasSpell(27210) && character->HasSpell(5676)) character->learnSpell(27210, false); //Жгучая боль Уровень 7
            if (level >= 65 && !character->HasSpell(27216) && character->HasSpell(172)) character->learnSpell(27216, false); //Порча Уровень 8
            if (level >= 66 && !character->HasSpell(28172) && character->HasSpell(2362)) character->learnSpell(28172, false); //Создание камня чар Уровень 4
            if (level >= 66 && !character->HasSpell(29858)) character->learnSpell(29858, false); //Раскол души 
            if (level >= 66 && !character->HasSpell(27250) && character->HasSpell(6366)) character->learnSpell(27250, false); //Создание камня огня Уровень 5
            if (level >= 67 && !character->HasSpell(27218) && character->HasSpell(980)) character->learnSpell(27218, false); //Проклятие агонии Уровень 7
            if (level >= 67 && !character->HasSpell(27259) && character->HasSpell(755)) character->learnSpell(27259, false); //Канал здоровья Уровень 8
            if (level >= 67 && !character->HasSpell(27217) && character->HasSpell(1120)) character->learnSpell(27217, false); //Похищение души Уровень 5
            if (level >= 68 && !character->HasSpell(27213) && character->HasSpell(1949)) character->learnSpell(27213, false); //Адское Пламя Уровень 4
            if (level >= 68 && !character->HasSpell(27222) && character->HasSpell(1454)) character->learnSpell(27222, false); //Жизнеотвод Уровень 7
            if (level >= 68 && !character->HasSpell(29893)) character->learnSpell(29893, false); //Ритуал душ Уровень 1
            if (level >= 68 && !character->HasSpell(27223) && character->HasSpell(6789)) character->learnSpell(27223, false); //Лик смерти Уровень 4
            if (level >= 68 && !character->HasSpell(27230) && character->HasSpell(6201)) character->learnSpell(27230, false); //Создание камня здоровья Уровень 6
            if (level >= 69 && !character->HasSpell(27212) && character->HasSpell(5740)) character->learnSpell(27212, false); //Огненный ливень Уровень 5
            if (level >= 69 && !character->HasSpell(30909) && character->HasSpell(702)) character->learnSpell(30909, false); //Проклятие слабости Уровень 8
            if (level >= 69 && !character->HasSpell(27209) && character->HasSpell(686)) character->learnSpell(27209, false); //Стрела Тьмы Уровень 11
            if (level >= 69 && !character->HasSpell(27215) && character->HasSpell(348)) character->learnSpell(27215, false); //Жертвенный огонь Уровень 9
            if (level >= 69 && !character->HasSpell(28189) && character->HasSpell(28176)) character->learnSpell(28189, false); //Доспех Скверны Уровень 2
            if (level >= 69 && !character->HasSpell(27220) && character->HasSpell(689)) character->learnSpell(27220, false); //Похищение жизни Уровень 8
            if (level >= 69 && !character->HasSpell(27228) && character->HasSpell(1490)) character->learnSpell(27228, false); //Проклятие стихий Уровень 4
            if (level >= 70 && !character->HasSpell(30910) && character->HasSpell(603)) character->learnSpell(30910, false); //Проклятие рока Уровень 2
            if (level >= 70 && !character->HasSpell(32231) && character->HasSpell(29722)) character->learnSpell(32231, false); //Испепеление Уровень 2
            if (level >= 70 && !character->HasSpell(59170) && character->HasSpell(50796)) character->learnSpell(59170, false); //Стрела Хаоса Уровень 2
            if (level >= 70 && !character->HasSpell(59161) && character->HasSpell(48181)) character->learnSpell(59161, false); //Блуждающий дух Уровень 2
            if (level >= 70 && !character->HasSpell(30546) && character->HasSpell(17877)) character->learnSpell(30546, false); //Ожог Тьмы Уровень 8
            if (level >= 70 && !character->HasSpell(30545) && character->HasSpell(6353)) character->learnSpell(30545, false); //Ожог души Уровень 4
            if (level >= 70 && !character->HasSpell(30459) && character->HasSpell(5676)) character->learnSpell(30459, false); //Жгучая боль Уровень 8
            if (level >= 70 && !character->HasSpell(30414) && character->HasSpell(30283)) character->learnSpell(30414, false); //Неистовство Тьмы Уровень 3
            if (level >= 70 && !character->HasSpell(30405) && character->HasSpell(30108)) character->learnSpell(30405, false); //Нестабильное колдовство Уровень 3
            if (level >= 70 && !character->HasSpell(27238) && character->HasSpell(693)) character->learnSpell(27238, false); //Создание камня души Уровень 6
            if (level >= 70 && !character->HasSpell(27265) && character->HasSpell(18220)) character->learnSpell(27265, false); //Темный союз Уровень 4
            if (level >= 70 && !character->HasSpell(27243)) character->learnSpell(27243, false); //Семя порчи Уровень 1
            if (level >= 70 && !character->HasSpell(27260) && character->HasSpell(706)) character->learnSpell(27260, false); //Демонический доспех Уровень 6
            if (level >= 71 && !character->HasSpell(50511) && character->HasSpell(702)) character->learnSpell(50511, false); // unk
            if (level >= 71 && !character->HasSpell(47812) && character->HasSpell(172)) character->learnSpell(47812, false); // unk
            if (level >= 72 && !character->HasSpell(47886) && character->HasSpell(2362)) character->learnSpell(47886, false); // unk
            if (level >= 72 && !character->HasSpell(61191) && character->HasSpell(1098)) character->learnSpell(61191, false); //Порабощение демона Уровень 4
            if (level >= 72 && !character->HasSpell(47890) && character->HasSpell(6229)) character->learnSpell(47890, false); // unk
            if (level >= 72 && !character->HasSpell(47819) && character->HasSpell(5740)) character->learnSpell(47819, false); // unk
            if (level >= 73 && !character->HasSpell(47863) && character->HasSpell(980)) character->learnSpell(47863, false); // unk
            if (level >= 73 && !character->HasSpell(47859) && character->HasSpell(6789)) character->learnSpell(47859, false); // unk
            if (level >= 73 && !character->HasSpell(47871) && character->HasSpell(6201)) character->learnSpell(47871, false); // unk
            if (level >= 74 && !character->HasSpell(47837) && character->HasSpell(29722)) character->learnSpell(47837, false); // unk
            if (level >= 74 && !character->HasSpell(47892) && character->HasSpell(28176)) character->learnSpell(47892, false); // unk
            if (level >= 74 && !character->HasSpell(47814) && character->HasSpell(5676)) character->learnSpell(47814, false); // unk
            if (level >= 74 && !character->HasSpell(47808) && character->HasSpell(686)) character->learnSpell(47808, false); // unk
            if (level >= 74 && !character->HasSpell(60219) && character->HasSpell(6366)) character->learnSpell(60219, false); //Создание камня огня Уровень 6
            if (level >= 75 && !character->HasSpell(47846) && character->HasSpell(30283)) character->learnSpell(47846, false); // unk
            if (level >= 75 && !character->HasSpell(47841) && character->HasSpell(30108)) character->learnSpell(47841, false); // unk
            if (level >= 75 && !character->HasSpell(47897)) character->learnSpell(47897, false); // unk
            if (level >= 75 && !character->HasSpell(47835) && character->HasSpell(27243)) character->learnSpell(47835, false); // unk
            if (level >= 75 && !character->HasSpell(47826) && character->HasSpell(17877)) character->learnSpell(47826, false); // unk
            if (level >= 75 && !character->HasSpell(59163) && character->HasSpell(48181)) character->learnSpell(59163, false); //Блуждающий дух Уровень 3
            if (level >= 75 && !character->HasSpell(47824) && character->HasSpell(6353)) character->learnSpell(47824, false); // unk
            if (level >= 75 && !character->HasSpell(59171) && character->HasSpell(50796)) character->learnSpell(59171, false); //Стрела Хаоса Уровень 3
            if (level >= 75 && !character->HasSpell(47810) && character->HasSpell(348)) character->learnSpell(47810, false); // unk
            if (level >= 76 && !character->HasSpell(47856) && character->HasSpell(755)) character->learnSpell(47856, false); // unk
            if (level >= 76 && !character->HasSpell(47884) && character->HasSpell(693)) character->learnSpell(47884, false); // unk
            if (level >= 76 && !character->HasSpell(47793) && character->HasSpell(706)) character->learnSpell(47793, false); // unk
            if (level >= 77 && !character->HasSpell(47813) && character->HasSpell(172)) character->learnSpell(47813, false); // unk
            if (level >= 77 && !character->HasSpell(47855) && character->HasSpell(1120)) character->learnSpell(47855, false); // unk
            if (level >= 78 && !character->HasSpell(47888) && character->HasSpell(2362)) character->learnSpell(47888, false); // unk
            if (level >= 78 && !character->HasSpell(47891) && character->HasSpell(6229)) character->learnSpell(47891, false); // unk
            if (level >= 78 && !character->HasSpell(47865) && character->HasSpell(1490)) character->learnSpell(47865, false); // unk
            if (level >= 78 && !character->HasSpell(47860) && character->HasSpell(6789)) character->learnSpell(47860, false); // unk
            if (level >= 78 && !character->HasSpell(47823) && character->HasSpell(1949)) character->learnSpell(47823, false); // unk
            if (level >= 78 && !character->HasSpell(47857) && character->HasSpell(689)) character->learnSpell(47857, false); // unk
            if (level >= 79 && !character->HasSpell(47820) && character->HasSpell(5740)) character->learnSpell(47820, false); // unk
            if (level >= 79 && !character->HasSpell(47893) && character->HasSpell(28176)) character->learnSpell(47893, false); // unk
            if (level >= 79 && !character->HasSpell(47809) && character->HasSpell(686)) character->learnSpell(47809, false); // unk
            if (level >= 79 && !character->HasSpell(47815) && character->HasSpell(5676)) character->learnSpell(47815, false); // unk
            if (level >= 79 && !character->HasSpell(47864) && character->HasSpell(980)) character->learnSpell(47864, false); // unk
            if (level >= 79 && !character->HasSpell(47878) && character->HasSpell(6201)) character->learnSpell(47878, false); // unk
            if (level >= 80 && !character->HasSpell(47825) && character->HasSpell(6353)) character->learnSpell(47825, false); // unk
            if (level >= 80 && !character->HasSpell(59164) && character->HasSpell(48181)) character->learnSpell(59164, false); //Блуждающий дух Уровень 4
            if (level >= 80 && !character->HasSpell(47867) && character->HasSpell(603)) character->learnSpell(47867, false); // unk
            if (level >= 80 && !character->HasSpell(59172) && character->HasSpell(50796)) character->learnSpell(59172, false); //Стрела Хаоса Уровень 4
            if (level >= 80 && !character->HasSpell(47811) && character->HasSpell(348)) character->learnSpell(47811, false); // unk
            if (level >= 80 && !character->HasSpell(60220) && character->HasSpell(6366)) character->learnSpell(60220, false); //Создание камня огня Уровень 7
            if (level >= 80 && !character->HasSpell(47827) && character->HasSpell(17877)) character->learnSpell(47827, false); // unk
            if (level >= 80 && !character->HasSpell(59092) && character->HasSpell(18220)) character->learnSpell(59092, false); //Темный союз Уровень 5
            if (level >= 80 && !character->HasSpell(58887) && character->HasSpell(29893)) character->learnSpell(58887, false); //Ритуал душ Уровень 2
            if (level >= 80 && !character->HasSpell(47889) && character->HasSpell(706)) character->learnSpell(47889, false); // unk
            if (level >= 80 && !character->HasSpell(47843) && character->HasSpell(30108)) character->learnSpell(47843, false); // unk
            if (level >= 80 && !character->HasSpell(47838) && character->HasSpell(29722)) character->learnSpell(47838, false); // unk
            if (level >= 80 && !character->HasSpell(47847) && character->HasSpell(30283)) character->learnSpell(47847, false); // unk
            if (level >= 80 && !character->HasSpell(48018)) character->learnSpell(48018, false); // unk
            if (level >= 80 && !character->HasSpell(48020)) character->learnSpell(48020, false); // unk
            if (level >= 80 && !character->HasSpell(47836) && character->HasSpell(27243)) character->learnSpell(47836, false); // unk
            if (level >= 80 && !character->HasSpell(57946) && character->HasSpell(1454)) character->learnSpell(57946, false); //Жизнеотвод Уровень 8
            if (level >= 80 && !character->HasSpell(61290) && character->HasSpell(47897)) character->learnSpell(61290, false); //Пламя Тьмы Уровень 2
    }

    void learnSpellDruid()
    {
            if (!character->HasSpell(1126)) character->learnSpell(1126, false); //Знак дикой природы Уровень 1

            if (level < 4) return;

            if (!character->HasSpell(774)) character->learnSpell(774, false); //Омоложение Уровень 1
            if (!character->HasSpell(8921)) character->learnSpell(8921, false); //Лунный огонь Уровень 1

            if (level < 6) return;

            if (!character->HasSpell(467)) character->learnSpell(467, false); //Шипы Уровень 1
            if (!character->HasSpell(5177) && character->HasSpell(5176)) character->learnSpell(5177, false); //Гнев Уровень 2

            if (level < 8) return;

            if (!character->HasSpell(339)) character->learnSpell(339, false); //Гнев деревьев Уровень 1
            if (!character->HasSpell(5186) && character->HasSpell(5185)) character->learnSpell(5186, false); //Целительное прикосновение Уровень 2

            if (level < 10) return;

            if (!character->HasSpell(5487)) character->learnSpell(5487, false); //Облик медведя
            if (!character->HasSpell(6807)) character->learnSpell(6807, false); //Трепка
            if (!character->HasSpell(6795)) character->learnSpell(6795, false); //Рык

            if (!character->HasSpell(8924) && character->HasSpell(8921)) character->learnSpell(8924, false); //Лунный огонь Уровень 2
            if (!character->HasSpell(16689)) character->learnSpell(16689, false); //Хватка природы Уровень 1
            if (!character->HasSpell(5232) && character->HasSpell(1126)) character->learnSpell(5232, false); //Знак дикой природы Уровень 2
            if (!character->HasSpell(1058) && character->HasSpell(774)) character->learnSpell(1058, false); //Омоложение Уровень 2
            if (!character->HasSpell(99)) character->learnSpell(99, false); //Устрашающий рев Уровень 1

            if (level < 12) return;

            if (!character->HasSpell(8936)) character->learnSpell(8936, false); //Восстановление Уровень 1
            if (!character->HasSpell(50769)) character->learnSpell(50769, false); // unk
            if (!character->HasSpell(5229)) character->learnSpell(5229, false); //Исступление 

            if (level < 14) return;

            if (!character->HasSpell(782) && character->HasSpell(467)) character->learnSpell(782, false); //Шипы Уровень 2
            if (!character->HasSpell(5187) && character->HasSpell(5185)) character->learnSpell(5187, false); //Целительное прикосновение Уровень 3
            if (!character->HasSpell(5178) && character->HasSpell(5176)) character->learnSpell(5178, false); //Гнев Уровень 3
            if (!character->HasSpell(5211)) character->learnSpell(5211, false); //Оглушить Уровень 1

            if (level < 16) return;

            if (!character->HasSpell(8925) && character->HasSpell(8921)) character->learnSpell(8925, false); //Лунный огонь Уровень 3
            if (!character->HasSpell(779)) character->learnSpell(779, false); //Размах (медведь) Уровень 1
            if (!character->HasSpell(783)) character->learnSpell(783, false); //Походный облик Смена облика
            if (!character->HasSpell(1066)) character->learnSpell(1066, false); //Водный облик Смена облика
            if (!character->HasSpell(1430) && character->HasSpell(774)) character->learnSpell(1430, false); //Омоложение Уровень 3

            if (level < 18) return;

            if (!character->HasSpell(1062) && character->HasSpell(339)) character->learnSpell(1062, false); //Гнев деревьев Уровень 2
            if (!character->HasSpell(2637)) character->learnSpell(2637, false); //Спячка Уровень 1
            if (!character->HasSpell(770)) character->learnSpell(770, false); //Волшебный огонь 
            if (!character->HasSpell(8938) && character->HasSpell(8936)) character->learnSpell(8938, false); //Восстановление Уровень 2
            if (!character->HasSpell(16810) && character->HasSpell(16689)) character->learnSpell(16810, false); //Хватка природы Уровень 2
            if (!character->HasSpell(16857)) character->learnSpell(16857, false); //Волшебный огонь (зверь) 
            if (!character->HasSpell(6808) && character->HasSpell(6807)) character->learnSpell(6808, false); //Трепка Уровень 2

            if (level < 20) return;

            if (!character->HasSpell(5215)) character->learnSpell(5215, false); //Крадущийся зверь 
            if (!character->HasSpell(5188) && character->HasSpell(5185)) character->learnSpell(5188, false); //Целительное прикосновение Уровень 4
            if (!character->HasSpell(2912)) character->learnSpell(2912, false); //Звездный огонь Уровень 1
            if (!character->HasSpell(1735) && character->HasSpell(99)) character->learnSpell(1735, false); //Устрашающий рев Уровень 2
            if (!character->HasSpell(1079)) character->learnSpell(1079, false); //Разорвать Уровень 1
            if (!character->HasSpell(1082)) character->learnSpell(1082, false); //Цапнуть Уровень 1
            if (!character->HasSpell(6756) && character->HasSpell(1126)) character->learnSpell(6756, false); //Знак дикой природы Уровень 3
            if (!character->HasSpell(768)) character->learnSpell(768, false); //Облик кошки Смена облика
            if (!character->HasSpell(20484)) character->learnSpell(20484, false); //Возрождение Уровень 1

            if (level < 22) return;

            if (!character->HasSpell(8926) && character->HasSpell(8921)) character->learnSpell(8926, false); //Лунный огонь Уровень 4
            if (!character->HasSpell(2908)) character->learnSpell(2908, false); //Умиротворение животного Уровень 1
            if (!character->HasSpell(5179) && character->HasSpell(5176)) character->learnSpell(5179, false); //Гнев Уровень 4
            if (!character->HasSpell(2090) && character->HasSpell(774)) character->learnSpell(2090, false); //Омоложение Уровень 4
            if (!character->HasSpell(5221)) character->learnSpell(5221, false); //Полоснуть Уровень 1

            if (level < 24) return;

            if (!character->HasSpell(5217)) character->learnSpell(5217, false); //Тигриное неистовство Уровень 1
            if (!character->HasSpell(8939) && character->HasSpell(8936)) character->learnSpell(8939, false); //Восстановление Уровень 3
            if (!character->HasSpell(1075) && character->HasSpell(467)) character->learnSpell(1075, false); //Шипы Уровень 3
            if (!character->HasSpell(2782)) character->learnSpell(2782, false); //Снятие проклятия 
            if (!character->HasSpell(780) && character->HasSpell(779)) character->learnSpell(780, false); //Размах (медведь) Уровень 2
            if (!character->HasSpell(50768) && character->HasSpell(50769)) character->learnSpell(50768, false); // unk
            if (!character->HasSpell(1822)) character->learnSpell(1822, false); //Глубокая рана Уровень 1

            if (level < 26) return;

            if (!character->HasSpell(6809) && character->HasSpell(6807)) character->learnSpell(6809, false); //Трепка Уровень 3
            if (!character->HasSpell(8949) && character->HasSpell(2912)) character->learnSpell(8949, false); //Звездный огонь Уровень 2
            if (!character->HasSpell(1850)) character->learnSpell(1850, false); //Порыв Уровень 1
            if (!character->HasSpell(5189) && character->HasSpell(5185)) character->learnSpell(5189, false); //Целительное прикосновение Уровень 5
            if (!character->HasSpell(2893)) character->learnSpell(2893, false); //Устранение яда 

            if (level < 28) return;

            if (!character->HasSpell(16811) && character->HasSpell(16689)) character->learnSpell(16811, false); //Хватка природы Уровень 3
            if (!character->HasSpell(8927) && character->HasSpell(8921)) character->learnSpell(8927, false); //Лунный огонь Уровень 5
            if (!character->HasSpell(8998)) character->learnSpell(8998, false); //Попятиться Уровень 1
            if (!character->HasSpell(9492) && character->HasSpell(1079)) character->learnSpell(9492, false); //Разорвать Уровень 2
            if (!character->HasSpell(2091) && character->HasSpell(774)) character->learnSpell(2091, false); //Омоложение Уровень 5
            if (!character->HasSpell(5195) && character->HasSpell(339)) character->learnSpell(5195, false); //Гнев деревьев Уровень 3
            if (!character->HasSpell(5209)) character->learnSpell(5209, false); //Вызывающий рев 
            if (!character->HasSpell(3029) && character->HasSpell(1082)) character->learnSpell(3029, false); //Цапнуть Уровень 2

            if (level < 30) return;

            if (!character->HasSpell(5180) && character->HasSpell(5176)) character->learnSpell(5180, false); //Гнев Уровень 5
            if (!character->HasSpell(8940) && character->HasSpell(8936)) character->learnSpell(8940, false); //Восстановление Уровень 4
            if (!character->HasSpell(5234) && character->HasSpell(1126)) character->learnSpell(5234, false); //Знак дикой природы Уровень 4
            if (!character->HasSpell(20739) && character->HasSpell(20484)) character->learnSpell(20739, false); //Возрождение Уровень 2
            if (!character->HasSpell(740)) character->learnSpell(740, false); //Спокойствие Уровень 1
            if (!character->HasSpell(24974) && character->HasSpell(5570)) character->learnSpell(24974, false); //Рой насекомых Уровень 2
            if (!character->HasSpell(6800) && character->HasSpell(5221)) character->learnSpell(6800, false); //Полоснуть Уровень 2
            if (!character->HasSpell(6798) && character->HasSpell(5211)) character->learnSpell(6798, false); //Оглушить Уровень 2

            if (level < 32) return;

            if (!character->HasSpell(6785)) character->learnSpell(6785, false); //Накинуться Уровень 1
            if (!character->HasSpell(5225)) character->learnSpell(5225, false); //Выслеживание гуманоидов 
            if (!character->HasSpell(6778) && character->HasSpell(5185)) character->learnSpell(6778, false); //Целительное прикосновение Уровень 6
            if (!character->HasSpell(22568)) character->learnSpell(22568, false); //Свирепый укус Уровень 1
            if (!character->HasSpell(9490) && character->HasSpell(99)) character->learnSpell(9490, false); //Устрашающий рев Уровень 3

            if (level < 34) return;

            if (!character->HasSpell(8950) && character->HasSpell(2912)) character->learnSpell(8950, false); //Звездный огонь Уровень 3
            if (!character->HasSpell(8928) && character->HasSpell(8921)) character->learnSpell(8928, false); //Лунный огонь Уровень 6
            if (!character->HasSpell(1823) && character->HasSpell(1822)) character->learnSpell(1823, false); //Глубокая рана Уровень 2
            if (!character->HasSpell(8914) && character->HasSpell(467)) character->learnSpell(8914, false); //Шипы Уровень 4
            if (!character->HasSpell(769) && character->HasSpell(779)) character->learnSpell(769, false); //Размах (медведь) Уровень 3
            if (!character->HasSpell(3627) && character->HasSpell(774)) character->learnSpell(3627, false); //Омоложение Уровень 6
            if (!character->HasSpell(8972) && character->HasSpell(6807)) character->learnSpell(8972, false); //Трепка Уровень 4

            if (level < 36) return;

            if (!character->HasSpell(9493) && character->HasSpell(1079)) character->learnSpell(9493, false); //Разорвать Уровень 3
            if (!character->HasSpell(9005)) character->learnSpell(9005, false); //Наскок Уровень 1
            if (!character->HasSpell(50767) && character->HasSpell(50769)) character->learnSpell(50767, false); // unk
            if (!character->HasSpell(22842)) character->learnSpell(22842, false); //Неистовое восстановление 
            if (!character->HasSpell(6793) && character->HasSpell(5217)) character->learnSpell(6793, false); //Тигриное неистовство Уровень 2
            if (!character->HasSpell(8941) && character->HasSpell(8936)) character->learnSpell(8941, false); //Восстановление Уровень 5

            if (level < 38) return;

            if (!character->HasSpell(5201) && character->HasSpell(1082)) character->learnSpell(5201, false); //Цапнуть Уровень 3
            if (!character->HasSpell(16812) && character->HasSpell(16689)) character->learnSpell(16812, false); //Хватка природы Уровень 4
            if (!character->HasSpell(8903) && character->HasSpell(5185)) character->learnSpell(8903, false); //Целительное прикосновение Уровень 7
            if (!character->HasSpell(5196) && character->HasSpell(339)) character->learnSpell(5196, false); //Гнев деревьев Уровень 4
            if (!character->HasSpell(18657) && character->HasSpell(2637)) character->learnSpell(18657, false); //Спячка Уровень 2
            if (!character->HasSpell(8955) && character->HasSpell(2908)) character->learnSpell(8955, false); //Умиротворение животного Уровень 2
            if (!character->HasSpell(8992) && character->HasSpell(5221)) character->learnSpell(8992, false); //Полоснуть Уровень 3
            if (!character->HasSpell(6780) && character->HasSpell(5176)) character->learnSpell(6780, false); //Гнев Уровень 6

            if (level < 40) return;

            if (!character->HasSpell(29166)) character->learnSpell(29166, false); //Озарение 
            if (!character->HasSpell(24975) && character->HasSpell(5570)) character->learnSpell(24975, false); //Рой насекомых Уровень 3
            if (!character->HasSpell(20719)) character->learnSpell(20719, false); //Кошачья грация Пассивная
            if (!character->HasSpell(20742) && character->HasSpell(20484)) character->learnSpell(20742, false); //Возрождение Уровень 3
            if (!character->HasSpell(16914)) character->learnSpell(16914, false); //Гроза Уровень 1
            if (!character->HasSpell(22827) && character->HasSpell(22568)) character->learnSpell(22827, false); //Свирепый укус Уровень 2
            if (!character->HasSpell(62600)) character->learnSpell(62600, false); //Дикая защита Пассивная
            if (!character->HasSpell(8910) && character->HasSpell(774)) character->learnSpell(8910, false); //Омоложение Уровень 7
            if (!character->HasSpell(6783)) character->learnSpell(6783, false); //Крадущийся зверь Уровень 2
            if (!character->HasSpell(8907) && character->HasSpell(1126)) character->learnSpell(8907, false); //Знак дикой природы Уровень 5
            if (!character->HasSpell(8918) && character->HasSpell(740)) character->learnSpell(8918, false); //Спокойствие Уровень 2
            if (!character->HasSpell(8929) && character->HasSpell(8921)) character->learnSpell(8929, false); //Лунный огонь Уровень 7
            if (!character->HasSpell(9000) && character->HasSpell(8998)) character->learnSpell(9000, false); //Попятиться Уровень 2
            if (!character->HasSpell(9634) && character->HasSpell(5487)) character->learnSpell(9634, false); //Облик лютого медведя Смена облика

            if (level < 42) return;

            if (!character->HasSpell(8951) && character->HasSpell(2912)) character->learnSpell(8951, false); //Звездный огонь Уровень 4
            if (!character->HasSpell(6787) && character->HasSpell(6785)) character->learnSpell(6787, false); //Накинуться Уровень 2
            if (!character->HasSpell(9745) && character->HasSpell(6807)) character->learnSpell(9745, false); //Трепка Уровень 5
            if (!character->HasSpell(9747) && character->HasSpell(99)) character->learnSpell(9747, false); //Устрашающий рев Уровень 4
            if (!character->HasSpell(9750) && character->HasSpell(8936)) character->learnSpell(9750, false); //Восстановление Уровень 6

            if (level < 44) return;

            if (!character->HasSpell(9756) && character->HasSpell(467)) character->learnSpell(9756, false); //Шипы Уровень 5
            if (!character->HasSpell(9754) && character->HasSpell(779)) character->learnSpell(9754, false); //Размах (медведь) Уровень 4
            if (!character->HasSpell(9752) && character->HasSpell(1079)) character->learnSpell(9752, false); //Разорвать Уровень 4
            if (!character->HasSpell(22812)) character->learnSpell(22812, false); //Дубовая кожа 
            if (!character->HasSpell(1824) && character->HasSpell(1822)) character->learnSpell(1824, false); //Глубокая рана Уровень 3
            if (!character->HasSpell(9758) && character->HasSpell(5185)) character->learnSpell(9758, false); //Целительное прикосновение Уровень 8

            if (level < 46) return;

            if (!character->HasSpell(9839) && character->HasSpell(774)) character->learnSpell(9839, false); //Омоложение Уровень 8
            if (!character->HasSpell(9833) && character->HasSpell(8921)) character->learnSpell(9833, false); //Лунный огонь Уровень 8
            if (!character->HasSpell(9823) && character->HasSpell(9005)) character->learnSpell(9823, false); //Наскок Уровень 2
            if (!character->HasSpell(8905) && character->HasSpell(5176)) character->learnSpell(8905, false); //Гнев Уровень 7
            if (!character->HasSpell(9829) && character->HasSpell(5221)) character->learnSpell(9829, false); //Полоснуть Уровень 4
            if (!character->HasSpell(9821) && character->HasSpell(1850)) character->learnSpell(9821, false); //Порыв Уровень 2
            if (!character->HasSpell(8983) && character->HasSpell(5211)) character->learnSpell(8983, false); //Оглушить Уровень 3

            if (level < 48) return;

            if (!character->HasSpell(9845) && character->HasSpell(5217)) character->learnSpell(9845, false); //Тигриное неистовство Уровень 3
            if (!character->HasSpell(50766) && character->HasSpell(50769)) character->learnSpell(50766, false); // unk
            if (!character->HasSpell(22828) && character->HasSpell(22568)) character->learnSpell(22828, false); //Свирепый укус Уровень 3
            if (!character->HasSpell(9852) && character->HasSpell(339)) character->learnSpell(9852, false); //Гнев деревьев Уровень 5
            if (!character->HasSpell(16813) && character->HasSpell(16689)) character->learnSpell(16813, false); //Хватка природы Уровень 5
            if (!character->HasSpell(9849) && character->HasSpell(1082)) character->learnSpell(9849, false); //Цапнуть Уровень 4
            if (!character->HasSpell(9856) && character->HasSpell(8936)) character->learnSpell(9856, false); //Восстановление Уровень 7

            if (level < 50) return;
            
            if (!character->HasSpell(9875) && character->HasSpell(2912)) character->learnSpell(9875, false); //Звездный огонь Уровень 5
            if (!character->HasSpell(9888) && character->HasSpell(5185)) character->learnSpell(9888, false); //Целительное прикосновение Уровень 9
            if (!character->HasSpell(17401) && character->HasSpell(16914)) character->learnSpell(17401, false); //Гроза Уровень 2
            if (!character->HasSpell(9866) && character->HasSpell(6785)) character->learnSpell(9866, false); //Накинуться Уровень 3
            if (!character->HasSpell(9884) && character->HasSpell(1126)) character->learnSpell(9884, false); //Знак дикой природы Уровень 6
            if (!character->HasSpell(9880) && character->HasSpell(6807)) character->learnSpell(9880, false); //Трепка Уровень 6
            if (!character->HasSpell(20747) && character->HasSpell(20484)) character->learnSpell(20747, false); //Возрождение Уровень 4
            if (!character->HasSpell(21849)) character->learnSpell(21849, false); //Дар дикой природы Уровень 1
            if (!character->HasSpell(9862) && character->HasSpell(740)) character->learnSpell(9862, false); //Спокойствие Уровень 3
            if (!character->HasSpell(24976) && character->HasSpell(5570)) character->learnSpell(24976, false); //Рой насекомых Уровень 4

            if (level < 52) return;
            
            if (!character->HasSpell(9840) && character->HasSpell(774)) character->learnSpell(9840, false); //Омоложение Уровень 9
            if (!character->HasSpell(9894) && character->HasSpell(1079)) character->learnSpell(9894, false); //Разорвать Уровень 5
            if (!character->HasSpell(9898) && character->HasSpell(99)) character->learnSpell(9898, false); //Устрашающий рев Уровень 5
            if (!character->HasSpell(9892) && character->HasSpell(8998)) character->learnSpell(9892, false); //Попятиться Уровень 3
            if (!character->HasSpell(9834) && character->HasSpell(8921)) character->learnSpell(9834, false); //Лунный огонь Уровень 9
            
            if (level < 54) return;
            
            if (!character->HasSpell(9908) && character->HasSpell(779)) character->learnSpell(9908, false); //Размах (медведь) Уровень 5
            if (!character->HasSpell(9901) && character->HasSpell(2908)) character->learnSpell(9901, false); //Умиротворение животного Уровень 3
            if (!character->HasSpell(9904) && character->HasSpell(1822)) character->learnSpell(9904, false); //Глубокая рана Уровень 4
            if (!character->HasSpell(9830) && character->HasSpell(5221)) character->learnSpell(9830, false); //Полоснуть Уровень 5
            if (!character->HasSpell(9910) && character->HasSpell(467)) character->learnSpell(9910, false); //Шипы Уровень 6
            if (!character->HasSpell(9912) && character->HasSpell(5176)) character->learnSpell(9912, false); //Гнев Уровень 8
            if (!character->HasSpell(9857) && character->HasSpell(8936)) character->learnSpell(9857, false); //Восстановление Уровень 8

            if (level < 56) return;
            
            if (!character->HasSpell(22829) && character->HasSpell(22568)) character->learnSpell(22829, false); //Свирепый укус Уровень 4
            if (!character->HasSpell(9889) && character->HasSpell(5185)) character->learnSpell(9889, false); //Целительное прикосновение Уровень 10
            if (!character->HasSpell(9827) && character->HasSpell(9005)) character->learnSpell(9827, false); //Наскок Уровень 3

            if (level < 58) return;
            
            if (!character->HasSpell(9835) && character->HasSpell(8921)) character->learnSpell(9835, false); //Лунный огонь Уровень 10
            if (!character->HasSpell(9867) && character->HasSpell(6785)) character->learnSpell(9867, false); //Накинуться Уровень 4
            if (!character->HasSpell(9881) && character->HasSpell(6807)) character->learnSpell(9881, false); //Трепка Уровень 7
            if (!character->HasSpell(9853) && character->HasSpell(339)) character->learnSpell(9853, false); //Гнев деревьев Уровень 6
            if (!character->HasSpell(9841) && character->HasSpell(774)) character->learnSpell(9841, false); //Омоложение Уровень 10
            if (!character->HasSpell(9876) && character->HasSpell(2912)) character->learnSpell(9876, false); //Звездный огонь Уровень 6
            if (!character->HasSpell(18658) && character->HasSpell(2637)) character->learnSpell(18658, false); //Спячка Уровень 3
            if (!character->HasSpell(9850) && character->HasSpell(1082)) character->learnSpell(9850, false); //Цапнуть Уровень 5
            if (!character->HasSpell(33986) && character->HasSpell(33878)) character->learnSpell(33986, false); //Увечье (медведь) Уровень 2
            if (!character->HasSpell(17329) && character->HasSpell(16689)) character->learnSpell(17329, false); //Хватка природы Уровень 6
            if (!character->HasSpell(33982) && character->HasSpell(33876)) character->learnSpell(33982, false); //Увечье (кошка) Уровень 2

            if (level < 60) return;
            
            if (!character->HasSpell(53223) && character->HasSpell(50516)) character->learnSpell(53223, false); //Тайфун Уровень 2
            if (!character->HasSpell(31018) && character->HasSpell(22568)) character->learnSpell(31018, false); //Свирепый укус Уровень 5
            if (!character->HasSpell(50765) && character->HasSpell(50769)) character->learnSpell(50765, false); // unk
            if (!character->HasSpell(33950)) character->learnSpell(33950, false); //Воздушный облик Смена облика
            if (!character->HasSpell(31709) && character->HasSpell(8998)) character->learnSpell(31709, false); //Попятиться Уровень 4
            if (!character->HasSpell(25299) && character->HasSpell(774)) character->learnSpell(25299, false); //Омоложение Уровень 11
            if (!character->HasSpell(25298) && character->HasSpell(2912)) character->learnSpell(25298, false); //Звездный огонь Уровень 7
            if (!character->HasSpell(20748) && character->HasSpell(20484)) character->learnSpell(20748, false); //Возрождение Уровень 5
            if (!character->HasSpell(21850) && character->HasSpell(21849)) character->learnSpell(21850, false); //Дар дикой природы Уровень 2
            if (!character->HasSpell(24977) && character->HasSpell(5570)) character->learnSpell(24977, false); //Рой насекомых Уровень 5
            if (!character->HasSpell(9846) && character->HasSpell(5217)) character->learnSpell(9846, false); //Тигриное неистовство Уровень 4
            if (!character->HasSpell(17402) && character->HasSpell(16914)) character->learnSpell(17402, false); //Гроза Уровень 3
            if (!character->HasSpell(25297) && character->HasSpell(5185)) character->learnSpell(25297, false); //Целительное прикосновение Уровень 11
            if (!character->HasSpell(9913)) character->learnSpell(9913, false); //Крадущийся зверь Уровень 3
            if (!character->HasSpell(9858) && character->HasSpell(8936)) character->learnSpell(9858, false); //Восстановление Уровень 9
            if (!character->HasSpell(9885) && character->HasSpell(1126)) character->learnSpell(9885, false); //Знак дикой природы Уровень 7
            if (!character->HasSpell(9896) && character->HasSpell(1079)) character->learnSpell(9896, false); //Разорвать Уровень 6
            if (!character->HasSpell(9863) && character->HasSpell(740)) character->learnSpell(9863, false); //Спокойствие Уровень 4

            if (level < 61) return;
            
            if (!character->HasSpell(26984) && character->HasSpell(5176)) character->learnSpell(26984, false); //Гнев Уровень 9
            if (!character->HasSpell(27001) && character->HasSpell(5221)) character->learnSpell(27001, false); //Полоснуть Уровень 6

            if (level < 62) return;
            
            if (!character->HasSpell(26978) && character->HasSpell(5185)) character->learnSpell(26978, false); //Целительное прикосновение Уровень 12
            if (!character->HasSpell(22570)) character->learnSpell(22570, false); //Калечение Уровень 1
            if (!character->HasSpell(26998) && character->HasSpell(99)) character->learnSpell(26998, false); //Устрашающий рев Уровень 6

            if (level < 63) return;
            
            if (!character->HasSpell(26987) && character->HasSpell(8921)) character->learnSpell(26987, false); //Лунный огонь Уровень 11
            if (!character->HasSpell(26981) && character->HasSpell(774)) character->learnSpell(26981, false); //Омоложение Уровень 12
            if (!character->HasSpell(24248) && character->HasSpell(22568)) character->learnSpell(24248, false); //Свирепый укус Уровень 6

            if (level < 64) return;
            
            if (!character->HasSpell(26997) && character->HasSpell(779)) character->learnSpell(26997, false); //Размах (медведь) Уровень 6
            if (!character->HasSpell(33763)) character->learnSpell(33763, false); //Жизнецвет Уровень 1
            if (!character->HasSpell(26992) && character->HasSpell(467)) character->learnSpell(26992, false); //Шипы Уровень 7
            if (!character->HasSpell(27003) && character->HasSpell(1822)) character->learnSpell(27003, false); //Глубокая рана Уровень 5

            if (level < 65) return;
            
            if (!character->HasSpell(33357) && character->HasSpell(1850)) character->learnSpell(33357, false); //Порыв Уровень 3
            if (!character->HasSpell(26980) && character->HasSpell(8936)) character->learnSpell(26980, false); //Восстановление Уровень 10

            if (level < 66) return;
            
            if (!character->HasSpell(27005) && character->HasSpell(6785)) character->learnSpell(27005, false); //Накинуться Уровень 5
            if (!character->HasSpell(27006) && character->HasSpell(9005)) character->learnSpell(27006, false); //Наскок Уровень 4
            if (!character->HasSpell(33745)) character->learnSpell(33745, false); //Растерзать Уровень 1

            if (level < 67) return;
            
            if (!character->HasSpell(26996) && character->HasSpell(6807)) character->learnSpell(26996, false); //Трепка Уровень 8
            if (!character->HasSpell(27000) && character->HasSpell(1082)) character->learnSpell(27000, false); //Цапнуть Уровень 6
            if (!character->HasSpell(26986) && character->HasSpell(2912)) character->learnSpell(26986, false); //Звездный огонь Уровень 8
            if (!character->HasSpell(27008) && character->HasSpell(1079)) character->learnSpell(27008, false); //Разорвать Уровень 7

            if (level < 68) return;
            
            if (!character->HasSpell(33987) && character->HasSpell(33878)) character->learnSpell(33987, false); //Увечье (медведь) Уровень 3
            if (!character->HasSpell(26989) && character->HasSpell(339)) character->learnSpell(26989, false); //Гнев деревьев Уровень 7
            if (!character->HasSpell(27009) && character->HasSpell(16689)) character->learnSpell(27009, false); //Хватка природы Уровень 7
            if (!character->HasSpell(33983) && character->HasSpell(33876)) character->learnSpell(33983, false); //Увечье (кошка) Уровень 3

            if (level < 69) return;
            
            if (!character->HasSpell(26979) && character->HasSpell(5185)) character->learnSpell(26979, false); //Целительное прикосновение Уровень 13
            if (!character->HasSpell(26985) && character->HasSpell(5176)) character->learnSpell(26985, false); //Гнев Уровень 10
            if (!character->HasSpell(26982) && character->HasSpell(774)) character->learnSpell(26982, false); //Омоложение Уровень 13
            if (!character->HasSpell(26994) && character->HasSpell(20484)) character->learnSpell(26994, false); //Возрождение Уровень 6
            if (!character->HasSpell(27004) && character->HasSpell(8998)) character->learnSpell(27004, false); //Попятиться Уровень 5
            if (!character->HasSpell(50764) && character->HasSpell(50769)) character->learnSpell(50764, false); // unk

            if (level < 70) return;
            
            if (!character->HasSpell(53248) && character->HasSpell(48438)) character->learnSpell(53248, false); //Буйный рост Уровень 2
            if (!character->HasSpell(53225) && character->HasSpell(50516)) character->learnSpell(53225, false); //Тайфун Уровень 3
            if (!character->HasSpell(53199) && character->HasSpell(48505)) character->learnSpell(53199, false); // unk
            if (!character->HasSpell(27013) && character->HasSpell(5570)) character->learnSpell(27013, false); //Рой насекомых Уровень 6
            if (!character->HasSpell(27002) && character->HasSpell(5221)) character->learnSpell(27002, false); //Полоснуть Уровень 7
            if (!character->HasSpell(26991) && character->HasSpell(21849)) character->learnSpell(26991, false); //Дар дикой природы Уровень 3
            if (!character->HasSpell(27012) && character->HasSpell(16914)) character->learnSpell(27012, false); //Гроза Уровень 4
            if (!character->HasSpell(26990) && character->HasSpell(1126)) character->learnSpell(26990, false); //Знак дикой природы Уровень 8
            if (!character->HasSpell(26983) && character->HasSpell(740)) character->learnSpell(26983, false); //Спокойствие Уровень 5
            if (!character->HasSpell(26988) && character->HasSpell(8921)) character->learnSpell(26988, false); //Лунный огонь Уровень 12
            if (!character->HasSpell(33786)) character->learnSpell(33786, false); //Смерч 
            if (!character->HasSpell(26995) && character->HasSpell(2908)) character->learnSpell(26995, false); //Умиротворение животного Уровень 4

            if (level < 71) return;
            
            if (!character->HasSpell(49799) && character->HasSpell(1079)) character->learnSpell(49799, false); // unk
            if (!character->HasSpell(48559) && character->HasSpell(99)) character->learnSpell(48559, false); // unk
            if (!character->HasSpell(50212) && character->HasSpell(5217)) character->learnSpell(50212, false); // unk
            if (!character->HasSpell(62078)) character->learnSpell(62078, false); //Размах (кошка) Уровень 1
            if (!character->HasSpell(40120) && character->HasSpell(33943)) character->learnSpell(40120, false); //Облик стремительной птицы Смена облика
            if (!character->HasSpell(48442) && character->HasSpell(8936)) character->learnSpell(48442, false); // unk

            if (level < 72) return;
            
            if (!character->HasSpell(48573) && character->HasSpell(1822)) character->learnSpell(48573, false); // unk
            if (!character->HasSpell(48450) && character->HasSpell(33763)) character->learnSpell(48450, false); // unk
            if (!character->HasSpell(48576) && character->HasSpell(22568)) character->learnSpell(48576, false); // unk
            if ( !character->HasSpell(48464) && character->HasSpell(2912)) character->learnSpell(48464, false); // unk
            if (!character->HasSpell(48561) && character->HasSpell(779)) character->learnSpell(48561, false); // unk

            if (level < 73) return;
            
            if (!character->HasSpell(48578) && character->HasSpell(6785)) character->learnSpell(48578, false); // unk
            if (!character->HasSpell(48479) && character->HasSpell(6807)) character->learnSpell(48479, false); // unk
            if (!character->HasSpell(48567) && character->HasSpell(33745)) character->learnSpell(48567, false); // unk
            if (!character->HasSpell(48569) && character->HasSpell(1082)) character->learnSpell(48569, false); // unk

            if (level < 74) return;
            
            if (!character->HasSpell(48377) && character->HasSpell(5185)) character->learnSpell(48377, false); // unk
            if (!character->HasSpell(53307) && character->HasSpell(467)) character->learnSpell(53307, false); //Шипы Уровень 8
            if (!character->HasSpell(48459) && character->HasSpell(5176)) character->learnSpell(48459, false); // unk
            if (!character->HasSpell(49802) && character->HasSpell(22570)) character->learnSpell(49802, false); // unk

            if (level < 75) return;
            
            if (!character->HasSpell(53226) && character->HasSpell(50516)) character->learnSpell(53226, false); //Тайфун Уровень 4
            if (!character->HasSpell(53200) && character->HasSpell(48505)) character->learnSpell(53200, false); // unk
            if (!character->HasSpell(52610)) character->learnSpell(52610, false); // unk
            if (!character->HasSpell(48571) && character->HasSpell(5221)) character->learnSpell(48571, false); // unk
            if (!character->HasSpell(53249) && character->HasSpell(48438)) character->learnSpell(53249, false); //Буйный рост Уровень 3
            if (!character->HasSpell(48440) && character->HasSpell(774)) character->learnSpell(48440, false); // unk
            if (!character->HasSpell(48462) && character->HasSpell(8921)) character->learnSpell(48462, false); // unk
            if (!character->HasSpell(48565) && character->HasSpell(33876)) character->learnSpell(48565, false); // unk
            if (!character->HasSpell(48563) && character->HasSpell(33878)) character->learnSpell(48563, false); // unk
            if (!character->HasSpell(48446) && character->HasSpell(740)) character->learnSpell(48446, false); // unk

            if (level < 76) return;
            
            if (!character->HasSpell(48575) && character->HasSpell(8998)) character->learnSpell(48575, false); // unk

            if (level < 77) return;
            
            if (!character->HasSpell(49803) && character->HasSpell(9005)) character->learnSpell(49803, false); // unk
            if (!character->HasSpell(48443) && character->HasSpell(8936)) character->learnSpell(48443, false); // unk
            if (!character->HasSpell(48560) && character->HasSpell(99)) character->learnSpell(48560, false); // unk
            if (!character->HasSpell(48562) && character->HasSpell(779)) character->learnSpell(48562, false); // unk

            if (level < 78) return;
            
            if (!character->HasSpell(48465) && character->HasSpell(2912)) character->learnSpell(48465, false); // unk
            if (!character->HasSpell(53312) && character->HasSpell(16689)) character->learnSpell(53312, false); //Хватка природы Уровень 8
            if (!character->HasSpell(53308) && character->HasSpell(339)) character->learnSpell(53308, false); //Гнев деревьев Уровень 8
            if (!character->HasSpell(48577) && character->HasSpell(22568)) character->learnSpell(48577, false); // unk
            if (!character->HasSpell(48574) && character->HasSpell(1822)) character->learnSpell(48574, false); // unk

            if (level < 79) return;
            
            if (!character->HasSpell(48378) && character->HasSpell(5185)) character->learnSpell(48378, false); // unk
            if (!character->HasSpell(48477) && character->HasSpell(20484)) character->learnSpell(48477, false); // unk
            if (!character->HasSpell(48461) && character->HasSpell(5176)) character->learnSpell(48461, false); // unk
            if (!character->HasSpell(50213) && character->HasSpell(5217)) character->learnSpell(50213, false); // unk
            if (!character->HasSpell(48480) && character->HasSpell(6807)) character->learnSpell(48480, false); // unk
            if (!character->HasSpell(48570) && character->HasSpell(1082)) character->learnSpell(48570, false); // unk
            if (!character->HasSpell(48579) && character->HasSpell(6785)) character->learnSpell(48579, false); // unk

            if (level < 80) return;
            
            if (!character->HasSpell(61384) && character->HasSpell(50516)) character->learnSpell(61384, false); //Тайфун Уровень 5
            if (!character->HasSpell(48451) && character->HasSpell(33763)) character->learnSpell(48451, false); // unk
            if (!character->HasSpell(48447) && character->HasSpell(740)) character->learnSpell(48447, false); // unk
            if (!character->HasSpell(48441) && character->HasSpell(774)) character->learnSpell(48441, false); // unk
            if (!character->HasSpell(53251) && character->HasSpell(48438)) character->learnSpell(53251, false); //Буйный рост Уровень 4
            if (!character->HasSpell(53201) && character->HasSpell(48505)) character->learnSpell(53201, false); // unk
            if (!character->HasSpell(48463) && character->HasSpell(8921)) character->learnSpell(48463, false); // unk
            if (!character->HasSpell(48568) && character->HasSpell(33745)) character->learnSpell(48568, false); // unk
            if (!character->HasSpell(48566) && character->HasSpell(33876)) character->learnSpell(48566, false); // unk
            if (!character->HasSpell(49800) && character->HasSpell(1079)) character->learnSpell(49800, false); // unk
            if (!character->HasSpell(48564) && character->HasSpell(33878)) character->learnSpell(48564, false); // unk
            if (!character->HasSpell(50464)) character->learnSpell(50464, false); // unk
            if (!character->HasSpell(50763) && character->HasSpell(50769)) character->learnSpell(50763, false); // unk
            if (!character->HasSpell(48470) && character->HasSpell(21849)) character->learnSpell(48470, false); // unk
            if (!character->HasSpell(48469) && character->HasSpell(1126)) character->learnSpell(48469, false); // unk
            if (!character->HasSpell(48468) && character->HasSpell(5570)) character->learnSpell(48468, false); // unk
            if (!character->HasSpell(48467) && character->HasSpell(16914)) character->learnSpell(48467, false); // unk
            if (!character->HasSpell(48572) && character->HasSpell(5221)) character->learnSpell(48572, false); // unk
    }
};

void AddSC_Mod_AutoLearn()
{
    new Mod_AutoLearn_PlayerScript;
    new Mod_AutoLearn_WorldScript;
}