#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"


#define TYPE_GRID       (set.disp.typeGrid)
#define TYPE_GRID_1     (TYPE_GRID == Grid::Type::_1)
#define TYPE_GRID_2     (TYPE_GRID == Grid::Type::_2)
#define TYPE_GRID_3     (TYPE_GRID == Grid::Type::_3)
#define TYPE_GRID_4     (TYPE_GRID == Grid::Type::_4)


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
}

namespace Grid
{
    /// ��� ����� �� ������.
    struct Type
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
    Grid::Type::E                   typeGrid;              ///< ��� �����
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
