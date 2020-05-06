#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/ParametersOsci.h"


#define NUM_AVE_MAX     256


#define S_DISP_MAPPING          (set.disp._mapping)
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
#define S_DISP_ENUM_AVERAGE         (set.disp._enumAverage)
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
#define S_DISP_ENUM_ACCUM       (set.disp._enumAccum)
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


#define S_DISP_MODE_ACCUM           (set.disp._modeAccumulation)
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
#define S_DISP_ENUM_SMOOTH  (set.disp._enumSmoothing)
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
#define S_DISP_ENUM_FPS (set.disp._enumSignalsInSec)
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
#define S_DISP_TYPE_GRID        (set.disp._typeGrid)
#define S_DISP_TYPE_GRID_IS_1   (S_DISP_TYPE_GRID == TypeGrid::_1)
#define S_DISP_TYPE_GRID_IS_2   (S_DISP_TYPE_GRID == TypeGrid::_2)
#define S_DISP_TYPE_GRID_IS_3   (S_DISP_TYPE_GRID == TypeGrid::_3)
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
#define S_MENU_AUTOHIDE (set.disp._menuAutoHide)
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


#define S_DISP_BRIGHTNESS                   (set.disp._brightness)
#define S_DISP_SHIFT_IN_MEMORY              (set.disp._shiftInMemory)
#define S_DISP_LAST_AFFECTED_CHANNEL        (set.disp._lastAffectedChannel)
#define S_DISP_LAST_AFFECTED_CHANNEL_IS_A   (S_DISP_LAST_AFFECTED_CHANNEL == Chan::A)

struct SettingsDisplay
{ //-V802
    DisplayMapping::E      _mapping;
    ENumAverage::E         _enumAverage;           // ����� ���������� �������.
    ENumAccum::E           _enumAccum;             // ����� ���������� ������� �� ������.
    ModeAccumulation::E    _modeAccumulation;      // ����� ���������� ��������.
    ENumSmoothing::E       _enumSmoothing;         // ������������ ���������� ����� ��� ����������� �������.
    ENumSignalsInSec::E    _enumSignalsInSec;      // ������������ ����� ���������� ������� � �������.
    TypeGrid::E            _typeGrid;              // ��� �����
    uint8                  _brightness;            // ������� �������.
    MenuAutoHide::E        _menuAutoHide;          // ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    int16                  _shiftInMemory;         // \brief ���������� �������� ������ ���� ����� ������������ �������� ����� ������ � 
                                                  // ������. �.�. ��� �������� ��������� ����� � ��� ���� ������ ���������� ����� �� ������.
    Chan::E                _lastAffectedChannel;   // \brief ��������� ����������� �����. ������������ ��� ����������� ������ ��������
                                                  // �� ����� � ���������� ���� ������ �������
    void SetLastAffectedChannel(Chan::E ch)
    {
        _lastAffectedChannel = ch;
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
