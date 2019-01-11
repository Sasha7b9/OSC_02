#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MOSI_HEIGHT             9
#define MOI_HEIGHT_TITLE        19
#define MOI_WIDTH_D_IP          34      /* ���������� ������ ��������� ip-������ � ������ ������� ����� */
#define WIDTH_SB                19
#define GRID_DELTA              20  /* ���������� ����� � ������ */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    struct DrawMode
    {
        enum E
        {
            Auto,
            Hand
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

    struct ModeDrawSignal
    {
        enum E
        {
            Lines,
            Points
        } value;
    };

    /// ������� ��������� �������
    struct ThicknessSignal
    {
        enum E
        {
            _1,         ///< ������ �������� ������ �������� ���� �����
            _3          ///< ������ �������� ������ �������� ��� �����
        } value;
    };

    /// ����� ������ ������ ���������
    struct ShowStrNavi
    {
        enum E
        {
            Temp,   ///< ���������� �� ��������� ������
            All,    ///< ������ ����������
            None    ///< ������� �� ����������
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
    };

    /// ��� ���������� �� ����������
    struct ModeAveraging
    {
        enum E
        {
            Accurately,   ///< ��������� �����.
            Around        ///< ��������� ��������������.
        } value;
    };

    struct Orientation
    {
        enum E
        {
            Direct,
            Back
        } value;
    };
    /// ����� ����������� �������������� ������� �������� �������� �� ����������.
    struct AltMarkers
    {
        enum E
        {
            Hide,        /// ������ �� ��������.
            Show,        /// ������ ��������.
            AutoHide     /// �������� � ������� ����� timeShowLevels.
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
        /// ���������� ���������� ������ � �������
        int ToFPS() const;
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
    };

    struct ModeAccumulation
    {
        enum E
        {
            NoReset,   /// � ���� ������ ������������ ������ N ���������.
            Reset      /// � ���� ������ ���������� N ��������� � ����� ������������.
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
            _512
        } value;
    };
}
