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
    DisplayMapping(DisplayMapping::E v);
    operator DisplayMapping::E();
    static DisplayMapping::E &Ref();
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
    ENumMinMax() {};
    int Number() const;
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
    operator ENumAverage::E();
    int Number() const;
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
    ENumAccum() {};
    operator ENumAccum::E();
    int Number() const;
};

struct ModeAccumulation
{
    enum E
    {
        NoReset,   /// � ���� ������ ������������ ������ N ���������.
        Reset      /// � ���� ������ ���������� N ��������� � ����� ������������.
    };

    static bool IsReset();
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
    } value;
    explicit ENumSmoothing(E v) : value(v) { };
    uint ToNumber() const;
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
    } value;
    explicit ENumSignalsInSec(E v) : value(v) { };
    /// ���������� ���������� ������������ ����� �������
    uint TimeBetweenFramesMS() const;
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
        Size
    } value;
};

/// ��� �������� � �������� �� ���������
struct LinkingRShift
{
    enum E
    {
        Voltage,      ///< �������� ��������� � ����������
        Position      ///< �������� ��������� � �������
    };
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
    operator MenuAutoHide::E();
    static MenuAutoHide::E &Ref();
};


struct SettingsDisplay
{ //-V802
    DisplayMapping::E      mapping;
    ENumMinMax::E          enumMinMax;            ///< ������������ ���������� ��������� ��� ����������� ��������� � ���������.
    uint8                  notUsing0;
    ENumAverage::E         enumAverage;           ///< ����� ���������� �������.
    ENumAccum::E           enumAccum;             ///< ����� ���������� ������� �� ������.
    ModeAccumulation::E    modeAccumulation;      ///< ����� ���������� ��������.
    ENumSmoothing          ENumSmoothing;         ///< ������������ ���������� ����� ��� ����������� �������.
    ENumSignalsInSec       ENumSignalsInSec;      ///< ������������ ����� ���������� ������� � �������.
    TypeGrid::E            typeGrid;              ///< ��� �����
    int                    brightnessGrid;        ///< ������� ����� �� 0 �� 100.
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
