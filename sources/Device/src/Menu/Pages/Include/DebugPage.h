#pragma once
#include "Menu/MenuItems.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CONSOLE_NUM_STRINGS     (set.dbg.numStrings)
#define CONSOLE_SIZE_FONT       (set.dbg.sizeFont ? 8 : 5)
/// ���� tru�, �� ������� ����� �������� �������, � ������� ������� ����/���� ���������������� ����� � �������.
#define MODE_PAUSE_CONSOLE      (set.dbg.modePauseConsole)
#define NUM_MEASURES_FOR_GATES  (set.dbg.numMeasuresForGates)
#define TIME_COMPENSATION       (set.dbg.timeCompensation)
#define SHOW_RAND_INFO          (set.dbg.showRandInfo)
#define SHOW_RAND_STAT          (set.dbg.showRandStat)
#define SHOW_STAT               (set.dbg.ShowStats)
#define PRETRIGGERED            (set.dbg.pretriggered)
#define BANDWIDTH_DEBUG(ch)     (set.dbg.bandwidth[ch])

#define DBG_SHOW_ALL            (set.dbg.showAll)
#define DBG_SHOW_FLAG           (set.dbg.showFlag)

struct SettingsDebug
{
    int8                         showConsole;               ///< ���������� �� �������
    int16                        numStrings;                ///< ����� ����� � �������.
    int8                         sizeFont;                  ///< ������ ������ ������� - 0 - 5, 1 - 8.
    bool                         modePauseConsole;          ///< ���� true, �� ����� � ������� ��������������� �������� ������ ����/����.
    bool                         showAll;
    bool                         showFlag;
    bool                         showRShift[2];
    bool                         showTrigLev;
    bool                         showRange[2];
    bool                         showChanParam[2];
    bool                         showTrigParam;
    bool                         showTShift;
    bool                         showTBase;
    int16                        numMeasuresForGates;       ///< ����� ��������� ��� �����.
    bool                         showRandInfo;              ///< �������� ���������� �� ������������� - ������ � ��������� ��������.
    bool                         showRandStat;              ///< �������� ������ ����������.
    int16                        timeCompensation;          ///< �������������� �������� �� ������� ��� ������ �������� ������ �������������.
    int16                        pretriggered;              ///< ������������ �������� ����������� ��� ������������ �������������.
    FPGA::Settings::Bandwidth::E bandwidth[2];              ///< \brief ����� ������� ������, ������� ����� ���������� ��� ������ � "�����-������" 
                                                            ///< �������� "������".
    bool                         ShowStats;                 ///< ���������� ���������� �� ������ (fps, ��������).
};

float   GetStretchADC(Chan::E ch);

void    SetStretchADC(Chan::E ch, float kStretch);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageDebug
{
    static const Page *const self;


    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageConsole
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageRegisters
    {
        static const Page *const self;
    };


    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageRand
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSettings
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSerialNumber
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageADC
    {
        static const Page *const self;

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageBalance
        {
            static const Page *const self;
        };

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageStretch
        {
            static const Page *const self;

            static void OnChanged_Mode(bool);
        };

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageShift
        {
            static const Page *const self;
        };
    };
};
