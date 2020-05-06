#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"


// �������� �������������� ��������� �� ������ A
#define VIEW_MEASURES_A                 (S_CHANNEL_ENABLED_A && (S_MEAS_SOURCE_IS_A || S_MEAS_SOURCE_IS_A_B))
// �������� �������������� ��������� �� ������ B
#define VIEW_MEASURES_B                 (S_CHANNEL_ENABLED_B && (S_MEAS_SOURCE_IS_B || S_MEAS_SOURCE_IS_A_B))


// ������� �������������� ��������� ���������� �� �����
struct MeasuresOnDisplay
{
    enum E
    {
        _1,      // 1 ��������� ����� �����.
        _2,      // 2 ��������� ����� �����.
        _1_5,    // 1 ������ � 5 �����������.
        _2_5,    // 2 ������ �� 5 ���������.
        _3_5,    // 3 ������ �� 5 ���������.
        _6_1,    // 6 ����� �� 1 ���������.
        _6_2     // 6 ����� �� 2 ���������.
    };

    // ���������� true, ���� ��������� ����������� ����������� � ����� ������� ������
    static bool IsVertical();
};


struct MeasuresSource
{
#define S_MEAS_SOURCE           (set.meas.source)
#define S_MEAS_SOURCE_IS_A      (S_MEAS_SOURCE == MeasuresSource::A)
#define S_MEAS_SOURCE_IS_B      (S_MEAS_SOURCE == MeasuresSource::B)
#define S_MEAS_SOURCE_IS_A_B    (S_MEAS_SOURCE == MeasuresSource::A_B)

    enum E
    {
        A,
        B,
        A_B
    };
};


// ������� �� ������� ��� ������ ���������.
struct MeasuresModeViewSignals
{
    enum E
    {
        AsIs,       // ���������� ������� ��� ����.
        Compress    // ������� ����� � ���������.
    };

    static bool IsCompress();
};


struct SettingsAutoMeasures
{
    bool                        show;              // ���������� �� ���������.
    MeasuresOnDisplay::E        number;            // ������� ��������� ��������.
    MeasuresSource::E           source;            // ��� ����� ������� �������� ���������.
    MeasuresModeViewSignals::E  modeViewSignals;   // ������� �� ������� ��� ������ ���������.
    TypeMeasure::E              measures[15];      // ��������� ��� ��������� ���������.
    TypeMeasure::E              marked;            // ���������, �� ������� ����� �������� �������.
};



// ����� �������� ��������.
struct CursorsLookMode
{
#define S_CURS_LOOK_MODE(chan)              (set.curs._lookMode[chan])
#define S_CURS_LOOK_MODE_IS_VOLTAGE(chan)   (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Voltage)
#define S_CURS_LOOK_MODE_IS_TIME(chan)      (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Time)
#define S_CURS_LOOK_MODE_IS_BOTH(chan)      (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Both)

    enum E
    {
        None,      // ������� �� ������.
        Voltage,   // ������� ������ �� ����������� �������������.
        Time,      // ������� ������ �� �������� �������������.
        Both       // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
    };
};

// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
struct CursorsActive
{
    enum E
    {
        U,
        T,
        None
    };
};


struct CursorsSource
{
    CursorsSource() {}
    operator Chan::E();
};


// ����� �������� ���������
struct CursorsControl
{
    enum E
    {
        _1,        // ������
        _2,        // ������
        Both,      // ������
        Disabled    // �������
    };

    static CursorsControl::E &RefU(Chan::E ch);
    static CursorsControl::E &RefT(Chan::E ch);

    static bool IsDisabledT() { return RefT(CursorsSource()) == Disabled; }
    static bool IsEnabled1T() { return RefT(CursorsSource()) == _1 || RefT(CursorsSource()) == Both; }
    static bool IsEnabled2T() { return RefT(CursorsSource()) == _2 || RefT(CursorsSource()) == Both; }

    static bool IsDisabledU() { return RefU(CursorsSource()) == Disabled; }
    static bool IsEnabled1U() { return RefU(CursorsSource()) == _1 || RefU(CursorsSource()) == Both; }
    static bool IsEnabled2U() { return RefU(CursorsSource()) == _2 || RefU(CursorsSource()) == Both; }
};


// ������������ ����������� ��������.
struct CursorsMovement
{
    enum E
    {
        Pixels,    // �� �������� ������
        Percents   // �� ���������
    };
};


#define S_CURS_SHOW     (set.curs._showCursors)


struct SettingsCursorsMeasures
{
    bool               _showCursors;             // ���������� �� �������.
    CursorsLookMode::E _lookMode[Chan::Count];   // ������ �������� �� ��������� ��� ���� ��� ��������.
    bool               showFreq;                // ������������� � true ��������, ��� ����� ���������� �� ������ 1/dT ����� ���������.
    CursorsActive::E   active;                  // ����� ������� ����� �������.
    Chan::E            source;                  // �������� - � ������ ������ ��������� �������.
    CursorsControl::E  cntrlU[Chan::Count];     // �������� ������� ����������.
    CursorsControl::E  cntrlT[Chan::Count];     // �������� ������� �������.
    CursorsMovement::E movement;                // ��� ������������ �������� - �� ������ ��� ���������.
    float              deltaU100percents[2];    // ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float              deltaT100percents[2];    // ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    float              posCurU[Chan::Count][2]; // ������� ������� �������� ���������� ����� �������.
    float              posCurT[Chan::Count][2]; // ������� ������� �������� ������� ����� �������.
};


#define S_MATH_MODE_DRAW                (set.math.modeDraw)
#define S_MATH_MODE_DRAW_IS_SEPARATE    (S_MATH_MODE_DRAW == FuncModeDraw::Separate)
#define S_MATH_MODE_DRAW_IS_DISABLED    (S_MATH_MODE_DRAW == FuncModeDraw::Disabled)
struct FuncModeDraw
{
    enum E
    {
        Disabled,
        Separate,
        Together
    };
};


#define S_MATH_FUNCTION         (set.math.function)
#define S_MATH_FUNCTION_IS_SUM  (S_MATH_FUNCTION == MathFunction::Sum)
#define S_MATH_FUNCTION_IS_MUL  (S_MATH_FUNCTION == MathFunction::Mul)
struct MathFunction
{
    enum E
    {
        Sum,
        Mul
    };
};


#define S_MATH_MODE_REG_SET             (set.math.modeRegSet)
#define S_MATH_MODE_REG_SET_IS_RSHIFT   (S_MATH_MODE_REG_SET == ModeRegSet::RShift)
#define S_MATH_MODE_REG_SET_IS_RANGE    (S_MATH_MODE_REG_SET == ModeRegSet::Range)

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
    FuncModeDraw::E modeDraw;       // ���������� ��� ����� ������� � ������ �������������� �������.
    MathFunction::E function;
    ModeRegSet::E   modeRegSet;     // ������� ����� ��������� - ������� �� ������� ��� �������� �� ���������.
    Divider::E      divider;
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


    class Tune
    {
    public:
        static const Page * const self;
    };
};



struct PageCursorsMeasures
{
    static const Page * const self;


    struct Set
    {
        static const Page * const self;

        static void Draw_Channel(int x, int y);

        static void OnPress_Channel();

        // ������� ��������� ������
        static void IncCursCntrlU(Chan::E ch);

        // ������� ��������� ������
        static void IncCursCntrlT(Chan::E ch);

        static void OnPress_U();

        static void OnPress_T();

        static void SetCursPos100(Chan::E ch);

        // �������� �������� ������� ������� ���������� �� delta �����
        static void SetShiftCursPosU(Chan::E ch, int numCur, float delta);

        // �������� �������� ������� ������� ������� �� delta �����
        static void SetShiftCursPosT(Chan::E ch, int numCur, float delta);

        static bool HandlerKey(const KeyEvent &event);

        static void UpdateCursorsForLook();

        // ���������� ������� ������� ����������
        static void SetCursorU(Chan::E ch, int numCur, float pos);

        // ���������� ������� ������� �������
        static void SetCursorT(Chan::E ch, int numCur, float pos);

        // ���������� true, ���� �������� ����� ��������� ����� ���������� �������/�������
        static bool IsRegSetActiveOnCursors();

    private:
        static const float MAX_POS_U;
        static const float MAX_POS_T;

        // ���������� �������� ��������� ���������
        static void SetCursSource(Chan::E ch);
    };
};



struct PageMath
{
    static const Page * const self;
};
