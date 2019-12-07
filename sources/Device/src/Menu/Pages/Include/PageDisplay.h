#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Settings/SettingsOsci.h"


#define NUM_AVE_MAX     256


struct DisplayMapping
{
    enum E
    {
        Lines,
        Dots,
        Count
    };

    DisplayMapping() {};
    static DisplayMapping::E &Ref();
    DisplayMapping(DisplayMapping::E v) { Ref() = v; }
    operator DisplayMapping::E()        { return Ref(); }
    static bool IsLines()               { return Ref() == Lines; }
    static bool IsDots()                { return Ref() == Dots; }
};

/// ���������� ��������� ��� ������� ������������ � ������������� ��������.
struct ENumMinMax
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
        Count
    };

    ENumMinMax::E &Ref();
    int Number() { return 1 << Ref(); }
};

/// ���������� ���������� �� ����������.
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
    ENumAverage() {};
    operator ENumAverage::E() { return Ref(); };
    int Number()              { return (1 << Ref()); };
    static ENumAverage::E &Ref();
    static void Set(ENumAverage::E v);
};

/// ���������� ����������.
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
    static ENumAccum::E &Ref();
    operator ENumAccum::E() { return Ref(); }
    int Number()            { return (1 << Ref()); };
};

struct ModeAccumulation
{
    enum E
    {
        NoReset,   /// � ���� ������ ������������ ������ N ���������.
        Reset      /// � ���� ������ ���������� N ��������� � ����� ������������.
    };

    static ModeAccumulation::E &Ref();
    static bool IsReset() { return (Ref() == Reset); }
};

/// ���������� ����� ��� ������� �����������.
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
    static ENumSmoothing::E &Ref();
    static uint ToNumber() { return static_cast<uint>(Ref() + 1); };
};

/// ����������� FPS.
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
    ENumSignalsInSec() {}
    static ENumSignalsInSec::E &Ref();
    /// ���������� ���������� ������������ ����� �������
    static uint TimeBetweenFramesMS();
    operator ENumSignalsInSec::E() { return Ref(); }
    static bool Is25()             { return Ref() == _25; }
};

/// ��� ����� �� ������.
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

    static TypeGrid::E &Ref();
    static bool Is1() { return (Ref() == _1); };
    static bool Is2() { return (Ref() == _2); };
    static bool Is3() { return (Ref() == _3); };
};

/// ��� �������� � �������� �� ���������
struct LinkingRShift
{
    enum E
    {
        Voltage,      ///< �������� ��������� � ����������
        Position      ///< �������� ��������� � �������
    };

    static LinkingRShift::E &Ref();
};

/// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide
{
    enum E
    {
        None = 0,   ///< �������.
        _5 = 5,   ///< ����� 5 ������.
        _10 = 10,  ///< ����� 10 ������.
        _15 = 15,  ///< ����� 15 ������.
        _30 = 30,  ///< ����� 30 ������.
        _60 = 60   ///< ����� 60 ������.
    };

    MenuAutoHide() {};
    static MenuAutoHide::E &Ref();
    operator MenuAutoHide::E() { return Ref(); };
};


struct SettingsDisplay
{ //-V802
    DisplayMapping::E      mapping;
    ENumMinMax::E          enumMinMax;            ///< ������������ ���������� ��������� ��� ����������� ��������� � ���������.
    uint8                  _;
    ENumAverage::E         enumAverage;           ///< ����� ���������� �������.
    ENumAccum::E           enumAccum;             ///< ����� ���������� ������� �� ������.
    ModeAccumulation::E    modeAccumulation;      ///< ����� ���������� ��������.
    ENumSmoothing::E       enumSmoothing;         ///< ������������ ���������� ����� ��� ����������� �������.
    ENumSignalsInSec::E    enumSignalsInSec;      ///< ������������ ����� ���������� ������� � �������.
    TypeGrid::E            typeGrid;              ///< ��� �����
    LinkingRShift::E       linkingRShift;         ///< ��� �������� � �������� �� ���������.
    uint8                  brightness;            ///< ������� �������.
    MenuAutoHide::E        menuAutoHide;          ///< ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    int16                  shiftInMemory;         ///< \brief ���������� �������� ������ ���� ����� ������������ �������� ����� ������ � 
                                                  ///< ������. �.�. ��� �������� ��������� ����� � ��� ���� ������ ���������� ����� �� ������.
    Chan::E                lastAffectedChannel;   ///< \brief ��������� ����������� �����. ������������ ��� ����������� ������ ��������
                                                  ///< �� ����� � ���������� ���� ������ �������
    void SetLastAffectedChannel(Chan::E ch)
    {
        lastAffectedChannel = ch;
    }
};


struct PageDisplay
{
    static const Page * const self;


    struct PageAccumulation
    {
        static const Page * const self;

        static void OnPress_Accumulation_Clear();
    };


    struct PageSettings
    {
        static const Page * const self;


        struct PageColors
        {
            static const Page * const self;
            /// ��� �����
            static ColorType colorTypeGrid;
            /// ��� ������� ������
            static ColorType colorTypeA;
            /// ��� ������� ������
            static ColorType colorTypeB;
        };
    };
};
