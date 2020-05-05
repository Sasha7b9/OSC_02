#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/ParametersOsci.h"


#define NUM_AVE_MAX     256


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
    static int Number();
    static void Set(ENumAverage::E v);
};

// ���������� ����������.
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
    static int Number();
};

struct ModeAccumulation
{
    enum E
    {
        NoReset,   // � ���� ������ ������������ ������ N ���������.
        Reset      // � ���� ������ ���������� N ��������� � ����� ������������.
    };
};

// ���������� ����� ��� ������� �����������.
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
    static uint ToNumber();
};

// ����������� FPS.
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

    static TypeGrid::E &Ref();
    static bool Is1() { return (Ref() == _1); };
    static bool Is2() { return (Ref() == _2); };
    static bool Is3() { return (Ref() == _3); };
};

// ��� �������� � �������� �� ���������
struct LinkingRShift
{
    enum E
    {
        Voltage,      // �������� ��������� � ����������
        Position      // �������� ��������� � �������
    };

    static LinkingRShift::E &Ref();
};

// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide
{
    enum E
    {
        None = 0,   // �������.
        _5 = 5,   // ����� 5 ������.
        _10 = 10,  // ����� 10 ������.
        _15 = 15,  // ����� 15 ������.
        _30 = 30,  // ����� 30 ������.
        _60 = 60   // ����� 60 ������.
    };

    MenuAutoHide() {};
    static MenuAutoHide::E &Ref();
    operator MenuAutoHide::E() { return Ref(); };
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
    LinkingRShift::E       linkingRShift;         // ��� �������� � �������� �� ���������.
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
