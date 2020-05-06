#pragma once
#include "Menu/MenuItems.h"


#define S_DBG_SHOW_CONSOLE              (set.dbg.showConsole)
#define S_DBG_NUM_STRINGS_IN_CONSOLE    (set.dbg.numStrings)


struct SettingsDebug
{ //-V802
    int8   showConsole;         // ���������� �� �������
    int16  numStrings;          // ����� ����� � �������.
    int8   sizeFont;            // ������ ������ ������� - 0 - 5, 1 - 8.
    bool   modePauseConsole;    // ���� true, �� ����� � ������� ��������������� �������� ������ ����/����.
    bool   showAll;
    bool   showFlag;
    bool   showRShift[2];
    bool   showTrigLev;
    bool   showRange[2];
    bool   showChanParam[2];
    bool   showTrigParam;
    bool   showTShift;
    bool   showTBase;
    int16  numMeasuresForGates; // ����� ��������� ��� �����.
    bool   showRandInfo;        // �������� ���������� �� ������������� - ������ � ��������� ��������.
    bool   showRandStat;        // �������� ������ ����������.
    int16  timeCompensation;    // �������������� �������� �� ������� ��� ������ �������� ������ �������������.
    int16  pretriggered;        // ������������ �������� ����������� ��� ������������ �������������.
    bool   ShowStats;           // ���������� ���������� �� ������ (fps, ��������).
    bool   runTest;             // ���� true, �� ����� ��������� ���� ��� �������
    bool   showBattery;         // ���������� ��� ��� ��������� �������
    bool   showRandGate;        // �������� �� �������� ����� � ������ �������������
    bool   showRandPredPost;    // �������� �� �������� ����- � �����- ������� � ������ �������������
};


struct PageDebug
{
    static const Page *const self;

    struct PageConsole
    {
        static const Page *const self;
    };


    struct PageRegisters
    {
        static const Page *const self;
    };


    struct PageRand
    {
        static const Page *const self;
    };


    struct PageSettings
    {
        static const Page *const self;
    };


    struct PageSerialNumber
    {
        static const Page *const self;
    };


    struct PageTests
    {
        static const Page *const self;

        static void OnPress_Run();
    };


    struct PageADC
    {
        static const Page *const self;


        struct PageBalance
        {
            static const Page *const self;
        };


        struct PageStretch
        {
            static const Page *const self;

            static void OnChanged_Mode(bool);
        };


        struct PageShift
        {
            static const Page *const self;
        };

    };
};
