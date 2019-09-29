#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/Osci_Settings.h"


#define NUM_AVE_MAX                 256


struct ModeDrawSignal
{
    enum E
    {
        Lines,
        Points
    } value;
};

/// ���� ����
struct Background
{
    enum E
    {
        Black,
        White
    } value;
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
        _128
    } value;
    explicit ENumMinMax(E v) : value(v) {};
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
        _256
    } value;
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
    } value;
};

struct ModeAccumulation
{
    enum E
    {
        NoReset,   /// � ���� ������ ������������ ������ N ���������.
        Reset      /// � ���� ������ ���������� N ��������� � ����� ������������.
    } value;
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
    } value;
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
    } value;
};


struct SettingsDisplay
{ //-V802
    ModeDrawSignal::E      modeDrawSignal;
    Background::E          background;            ///< ���� ����.
    ENumMinMax::E          ENumMinMax;            ///< ������������ ���������� ��������� ��� ����������� ��������� � ���������.
    uint8                  notUsing0;
    ENumAverage::E         ENumAverage;           ///< ����� ���������� �������.
    ENumAccum::E           ENumAccum;             ///< ����� ���������� ������� �� ������.
    ModeAccumulation::E    modeAccumulation;      ///< ����� ���������� ��������.
    ENumSmoothing          ENumSmoothing;         ///< ������������ ���������� ����� ��� ����������� �������.
    ENumSignalsInSec       ENumSignalsInSec;      ///< ������������ ����� ���������� ������� � �������.
    TypeGrid::E            typeGrid;              ///< ��� �����
    int                    brightnessGrid;        ///< ������� ����� �� 0 �� 100.
    LinkingRShift::E       linkingRShift;         ///< ��� �������� � �������� �� ���������.
    int16                  brightness;            ///< ������� �������.
    MenuAutoHide::E        menuAutoHide;          ///< ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    int16                  shiftInMemory;         ///< \brief ���������� �������� ������ ���� ����� ������������ �������� ����� ������ � 
                                                  ///< ������. �.�. ��� �������� ��������� ����� � ��� ���� ������ ���������� ����� �� ������.
    Chan::E                lastAffectedChannel;   ///< \brief ��������� ����������� �����. ������������ ��� ����������� ������ ��������
                                                  ///< �� ����� � ���������� ���� ������ �������
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct PageDisplay
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageAccumulation
    {
        static const Page * const self;

        static void OnPress_Accumulation_Clear();
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSettings
    {
        static const Page * const self;

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
