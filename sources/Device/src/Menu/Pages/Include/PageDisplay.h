#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"


#define TYPE_GRID                   (set.disp.typeGrid)
#define TYPE_GRID_1                 (TYPE_GRID == Display::TypeGrid::_1)
#define TYPE_GRID_2                 (TYPE_GRID == Display::TypeGrid::_2)
#define TYPE_GRID_3                 (TYPE_GRID == Display::TypeGrid::_3)
#define TYPE_GRID_4                 (TYPE_GRID == Display::TypeGrid::_4)

#define LINKING_RSHIFT              (set.disp.linkingRShift)

#define MENU_AUTO_HIDE              (set.disp.menuAutoHide)

#define BRIGHTNESS_DISPLAY          (set.disp.brightness)

#define BRIGHTNESS_GRID             (set.disp.brightnessGrid)

#define BACKGROUND                  (set.disp.background)
#define BACKGROUND_BLACK            (BACKGROUND == Display::Background::Black)

#define MODE_DRAW_SIGNAL            (set.disp.modeDrawSignal)
#define MODE_DRAW_SIGNAL_IS_LINES   (MODE_DRAW_SIGNAL == Display::ModeDrawSignal::Lines)
#define MODE_DRAW_SIGNAL_IS_POINTS  (MODE_DRAW_SIGNAL == Display::ModeDrawSignal::Points)

#define ENUM_MIN_MAX                (set.disp.ENumMinMax)
#define MIN_MAX_ENABLED             (ENUM_MIN_MAX != Display::ENumMinMax::_1)
#define NUM_MIN_MAX                 (1 << (int)ENUM_MIN_MAX)        /* ���������� ���������� ��������� �������� ��� ������� ��������� � ����������. */

#define ENUM_SMOOTHING              (set.disp.ENumSmoothing)
#define SMOOTHING_ENABLED           (ENUM_SMOOTHING != Display::ENumSmoothing::Disable)

#define ACCUMULATION                (set.disp.modeAccumulation)
#define ACCUMULATION_IS_ENABLED     (ACCUMULATION == ::Display::ModeAccumulation::Reset)

#define ENUM_ACCUM                  (set.disp.ENumAccum)
#define NUM_ACCUM                   (1 << (int)ENUM_ACCUM)                   /* ���������� ����� ���������� */

#define MODE_AVE                    (set.disp.modeAveraging)
#define ENUM_AVE                    (set.disp.ENumAverage)
#define NUM_AVE                     (1 << (int)ENUM_AVE)
#define NUM_AVE_MAX                 256
#define IN_AVERAGING_MODE           (ENUM_AVE > Display::ENumAverage::_1 && (!IN_P2P_MODE))

#define ENUM_SIGNALS_IN_SEC         (set.disp.ENumSignalsInSec)

#define LAST_AFFECTED_CH            (set.disp.lastAffectedChannel)
#define LAST_AFFECTED_CH_IS_A       (LAST_AFFECTED_CH == Chan::A)


namespace Display
{
    struct ModeDrawSignal
    {
        enum E
        {
            Lines,
            Points
        } value;
    };

    /// ����� ����� ����.
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
}

struct SettingsDisplay
{
    Display::ModeDrawSignal::E      modeDrawSignal;
    Display::Background::E          background;            ///< ���� ����.
    Display::ENumMinMax::E          ENumMinMax;            ///< ������������ ���������� ��������� ��� ����������� ��������� � ���������.
    uint8                           notUsing0;
    Display::ENumAverage::E         ENumAverage;           ///< ����� ���������� �������.
    Display::ENumAccum::E           ENumAccum;             ///< ����� ���������� ������� �� ������.
    Display::ModeAccumulation::E    modeAccumulation;      ///< ����� ���������� ��������.
    Display::ENumSmoothing          ENumSmoothing;         ///< ������������ ���������� ����� ��� ����������� �������.
    Display::ENumSignalsInSec       ENumSignalsInSec;      ///< ������������ ����� ���������� ������� � �������.
    Display::TypeGrid::E            typeGrid;              ///< ��� �����
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
    static void OnChanged_RefreshFPS(bool);

    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PageView
    {
    public:
        //static const Page * const self;
    };

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
