#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"


/// �������� �������������� ��������� �� ������ A
#define VIEW_MEASURES_A                 (set.ch[Chan::A].enabled && (MeasuresSource::IsA() || MeasuresSource::IsBoth()))
/// �������� �������������� ��������� �� ������ B
#define VIEW_MEASURES_B                 (set.ch[Chan::B].enabled && (MeasuresSource::IsB() || MeasuresSource::IsBoth()))


/// ������� �������������� ��������� ���������� �� �����
struct MeasuresOnDisplay
{
    enum E
    {
        _1,      ///< 1 ��������� ����� �����.
        _2,      ///< 2 ��������� ����� �����.
        _1_5,    ///< 1 ������ � 5 �����������.
        _2_5,    ///< 2 ������ �� 5 ���������.
        _3_5,    ///< 3 ������ �� 5 ���������.
        _6_1,    ///< 6 ����� �� 1 ���������.
        _6_2     ///< 6 ����� �� 2 ���������.
    };
    MeasuresOnDisplay() {};
    operator MeasuresOnDisplay::E();
    /// ���������� true, ���� ��������� ����������� ����������� � ����� ������� ������
    static bool IsVertical();
};

struct MeasuresSource
{
    enum E
    {
        A,
        B,
        A_B
    };

    static bool IsA();
    static bool IsB();
    static bool IsBoth();
};

/// ������� �� ������� ��� ������ ���������.
struct MeasuresModeViewSignals
{
    enum E
    {
        AsIs,       ///< ���������� ������� ��� ����.
        Compress    ///< ������� ����� � ���������.
    };

    static bool IsCompress();
};


struct SettingsAutoMeasures
{
    bool                        show;              ///< ���������� �� ���������.
    MeasuresOnDisplay::E        number;            ///< ������� ��������� ��������.
    MeasuresSource::E           source;            ///< ��� ����� ������� �������� ���������.
    MeasuresModeViewSignals::E  modeViewSignals;   ///< ������� �� ������� ��� ������ ���������.
    TypeMeasure::E              measures[15];      ///< ��������� ��� ��������� ���������.
    TypeMeasure::E              marked;            ///< ���������, �� ������� ����� �������� �������.
};



/// ����� �������� ��������.
struct CursorsLookMode
{
    enum E
    {
        None,      ///< ������� �� ������.
        Voltage,   ///< ������� ������ �� ����������� �������������.
        Time,      ///< ������� ������ �� �������� �������������.
        Both       ///< ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
    };

    static bool IsVoltage(Chan::E ch);
    static bool IsTime(Chan::E ch);
    static bool IsBoth(Chan::E ch);
};

/// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
struct CursorsActive
{
    enum E
    {
        U,
        T,
        None
    };

    static bool IsT();
    static bool IsU();
    static void Set(E v);
};

/// ����� �������� ���������
struct CursorsControl
{
    enum E
    {
        _1,        ///< ������
        _2,        ///< ������
        _1_2,      ///< ������
        Disable    ///< �������
    };
};

/// ������������ ����������� ��������.
struct CursorsMovement
{
    enum E
    {
        Pixels,    ///< �� �������� ������
        Percents   ///< �� ���������
    };

    static bool IsPercents();
    static bool IsPixels();
};

struct SettingsCursorsMeasures
{
    bool               showCursors;             ///< ���������� �� �������.
    CursorsLookMode::E lookMode[2];             ///< ������ �������� �� ��������� ��� ���� ��� ��������.
    bool               showFreq;                ///< ������������� � true ��������, ��� ����� ���������� �� ������ 1/dT ����� ���������.
    CursorsActive::E   active;                  ///< ����� ������� ����� �������.
    Chan::E            source;                  ///< �������� - � ������ ������ ��������� �������.
    CursorsControl::E  cntrlU[Chan::Count];      ///< �������� ������� ����������.
    CursorsControl::E  cntrlT[Chan::Count];      ///< �������� ������� �������.
    CursorsMovement::E movement;                ///< ��� ������������ �������� - �� ������ ��� ���������.
    float              deltaU100percents[2];    ///< ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float              deltaT100percents[2];    ///< ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    float              posCurU[Chan::Count][2];  ///< ������� ������� �������� ���������� ����� �������.
    float              posCurT[Chan::Count][2];  ///< ������� ������� �������� ������� ����� �������.
};



struct FuncModeDraw
{
    enum E
    {
        Disable,
        Separate,
        Together
    };
};

struct MathFunction
{
    enum E
    {
        Sum,
        Mul
    };
};

struct ModeRegSet
{
    enum E
    {
        Range,
        RShift
    };
};

struct SettingsMath
{ //-V802
    FuncModeDraw::E modeDraw;       ///< ���������� ��� ����� ������� � ������ �������������� �������.
    MathFunction::E function;
    ModeRegSet::E   modeRegSet;     ///< ������� ����� ��������� - ������� �� ������� ��� �������� �� ���������.
    int8            divider;
    int16           rShift;
    Range::E        range;
};



struct PageMeasures
{
    static const Page * const self;
};



struct PageAutoMeasures
{
    static const Page * const self;


    class PageTune
    {
    public:
        static const Page * const self;
    };
};



struct PageCursorsMeasures
{
    static const Page * const self;


    struct PageSet
    {
        static const Page * const self;

        static void Draw_Channel(int x, int y);

        static void OnPress_Channel();
        /// ������� ��������� ������
        static void IncCursCntrlU(Chan::E ch);
        /// ������� ��������� ������
        static void IncCursCntrlT(Chan::E ch);

        static void OnPress_U();

        static void OnPress_T();

        static void SetCursPos100(Chan::E ch);
        /// �������� �������� ������� ������� ���������� �� delta �����
        static void SetShiftCursPosU(Chan::E ch, int numCur, float delta);
        /// �������� �������� ������� ������� ������� �� delta �����
        static void SetShiftCursPosT(Chan::E ch, int numCur, float delta);

        static bool HandlerKey(const KeyEvent &event);

        static void UpdateCursorsForLook();
        /// ���������� ������� ������� ����������
        static void SetCursorU(Chan::E ch, int numCur, float pos);
        /// ���������� ������� ������� �������
        static void SetCursorT(Chan::E ch, int numCur, float pos);
        /// ���������� true, ���� �������� ����� ��������� ����� ���������� �������/�������
        static bool IsRegSetActiveOnCursors();

    private:
        static const float MAX_POS_U;
        static const float MAX_POS_T;

        /// ���������� �������� ��������� ���������
        static void SetCursSource(Chan::E ch);
    };
};



struct PageMath
{
    static const Page * const self;
};
