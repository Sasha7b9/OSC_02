#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/Osci_Settings.h"


#define LINKING_RSHIFT              (set.disp.linkingRShift)

#define MENU_AUTO_HIDE              (set.disp.menuAutoHide)

#define BRIGHTNESS_DISPLAY          (set.disp.brightness)

#define BRIGHTNESS_GRID             (set.disp.brightnessGrid)

#define ENUM_MIN_MAX                (set.disp.ENumMinMax)
#define MIN_MAX_ENABLED             (ENUM_MIN_MAX != Display::ENumMinMax::_1)
#define NUM_MIN_MAX                 (1 << (int)ENUM_MIN_MAX)        /* ���������� ���������� ��������� �������� ��� ������� ��������� � ����������. */

#define NUM_AVE_MAX                 256

#define LAST_AFFECTED_CH            (set.disp.lastAffectedChannel)
#define LAST_AFFECTED_CH_IS_A       (LAST_AFFECTED_CH == Chan::A)


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


struct SettingsDisplay
{
    ModeDrawSignal::E      modeDrawSignal;
    Background::E          background;                     ///< ���� ����.
    ENumMinMax::E          ENumMinMax;                     ///< ������������ ���������� ��������� ��� ����������� ��������� � ���������.
    uint8                  notUsing0;
    ENumAverage::E         ENumAverage;           ///< ����� ���������� �������.
    ENumAccum::E           ENumAccum;             ///< ����� ���������� ������� �� ������.
    ModeAccumulation::E    modeAccumulation;      ///< ����� ���������� ��������.
    ENumSmoothing          ENumSmoothing;         ///< ������������ ���������� ����� ��� ����������� �������.
    ENumSignalsInSec       ENumSignalsInSec;      ///< ������������ ����� ���������� ������� � �������.
    TypeGrid::E            typeGrid;              ///< ��� �����
    int                             brightnessGrid;        ///< ������� ����� �� 0 �� 100.
    Display::LinkingRShift::E       linkingRShift;         ///< ��� �������� � �������� �� ���������.
    int16                           brightness;            ///< ������� �������.
    Menu::AutoHide::E               menuAutoHide;          ///< ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    int16                           shiftInMemory;         ///< \brief ���������� �������� ������ ���� ����� ������������ �������� ����� ������ � 
                                                           ///< ������. �.�. ��� �������� ��������� ����� � ��� ���� ������ ���������� ����� �� ������.
    Chan::E                         lastAffectedChannel;   ///< \brief ��������� ����������� �����. ������������ ��� ����������� ������ ��������
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
