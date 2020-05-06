#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/ParametersOsci.h"


#define NUM_AVE_MAX     256


#define S_DISP_MAPPING          (set.disp.mapping)
#define S_DISP_MAPPING_IS_LINES (S_DISP_MAPPING == DisplayMapping::Lines)
#define S_DISP_MAPPING_IS_DOTS  (S_DISP_MAPPING == DisplayMapping::Dots)
struct DisplayMapping
{
    enum E
    {
        Lines,
        Dots,
        Count
    };
};

// ���������� ���������� �� ����������.
#define S_DISP_ENUM_AVERAGE         (set.disp.enumAverage)
#define S_DISP_AVERAGING_IS_ENABLED (S_DISP_ENUM_AVERAGE != ENumAverage::_1)
#define S_DISP_ENUM_AVERAGE_U16     (static_cast<uint16>(S_DISP_ENUM_AVERAGE))
#define S_DISP_NUM_AVERAGE          (1 << S_DISP_ENUM_AVERAGE)
struct ENumAverage
{
    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _256,
        Count
    };
    static void Set(ENumAverage::E v);
};

// ���������� ����������.
#define S_DISP_ENUM_ACCUM       (set.disp.enumAccum)
#define S_DISP_NUM_ACCUM        (1 << S_DISP_ENUM_ACCUM)
struct ENumAccum
{
    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _Infinity
    };
};


#define S_DISP_MODE_ACCUM           (set.disp.modeAccumulation)
#define S_DISP_MODE_ACCUM_IS_RESET  (S_DISP_MODE_ACCUM == ModeAccumulation::Reset)
struct ModeAccumulation
{
    enum E
    {
        NoReset,   // � ���� ������ ������������ ������ N ���������.
        Reset      // � ���� ������ ���������� N ��������� � ����� ������������.
    };
};

// ���������� ����� ��� ������� �����������.
#define S_DISP_ENUM_SMOOTH  (set.disp.enumSmoothing)
#define S_DISP_NUM_SMOOTH   (S_DISP_ENUM_SMOOTH + 1)
struct ENumSmoothing
{
    enum E
    {
        Disable,
        _2points,
        _3points,
        _4points,
        _5points,
        _6points,
        _7points,
        _8points,
        _9points,
        _10points
    };
};

// ����������� FPS.
#define S_DISP_ENUM_FPS (set.disp.enumSignalsInSec)
struct ENumSignalsInSec
{
    enum E
    {
        _25,
        _10,
        _5,
        _2,
        _1
    };
    // ���������� ���������� ������������ ����� �������
    static uint TimeBetweenFramesMS();
};

// ��� ����� �� ������.
struct TypeGrid
{
    enum E
    {
        _1,
        _2,
        _3,
        _4,
        Count
    };
};

// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide
{
    enum E
    {
        None,   // �������.
        _5,     // ����� 5 ������.
        _10,    // ����� 10 ������.
        _15,    // ����� 15 ������.
        _30,    // ����� 30 ������.
        _60     // ����� 60 ������.
    };
};


struct SettingsDisplay
{ //-V802
    DisplayMapping::E      mapping;
    ENumAverage::E         enumAverage;           // ����� ���������� �������.
    ENumAccum::E           enumAccum;             // ����� ���������� ������� �� ������.
    ModeAccumulation::E    modeAccumulation;      // ����� ���������� ��������.
    ENumSmoothing::E       enumSmoothing;         // ������������ ���������� ����� ��� ����������� �������.
    ENumSignalsInSec::E    enumSignalsInSec;      // ������������ ����� ���������� ������� � �������.
    TypeGrid::E            typeGrid;              // ��� �����
    uint8                  brightness;            // ������� �������.
    MenuAutoHide::E        menuAutoHide;          // ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    int16                  shiftInMemory;         // \brief ���������� �������� ������ ���� ����� ������������ �������� ����� ������ � 
                                                  // ������. �.�. ��� �������� ��������� ����� � ��� ���� ������ ���������� ����� �� ������.
    Chan::E                lastAffectedChannel;   // \brief ��������� ����������� �����. ������������ ��� ����������� ������ ��������
                                                  // �� ����� � ���������� ���� ������ �������
    void SetLastAffectedChannel(Chan::E ch)
    {
        lastAffectedChannel = ch;
    }
};


struct PageDisplay
{
    static const Page * const self;


    struct Accumulation
    {
        static const Page * const self;

        static void OnPress_Accumulation_Clear();
    };


    struct Settings
    {
        static const Page * const self;


        struct Colors
        {
            static const Page * const self;
            // ��� �����
            static ColorType colorTypeGrid;
            // ��� ������� ������
            static ColorType colorTypeA;
            // ��� ������� ������
            static ColorType colorTypeB;
        };
    };
};
