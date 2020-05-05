#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"


// �������� �������������� ��������� �� ������ A
#define VIEW_MEASURES_A                 (setA.enabled && ((set.meas.source == MeasuresSource::A) || (set.meas.source == MeasuresSource::A_B)))
// �������� �������������� ��������� �� ������ B
#define VIEW_MEASURES_B                 (setB.enabled && ((set.meas.source == MeasuresSource::B) || (set.meas.source == MeasuresSource::A_B)))


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


struct SettingsCursorsMeasures
{
    bool               showCursors;             // ���������� �� �������.
    CursorsLookMode::E lookMode[Chan::Count];   // ������ �������� �� ��������� ��� ���� ��� ��������.
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



struct FuncModeDraw
{
    enum E
    {
        Disabled,
        Separate,
        Together
    };
    FuncModeDraw() {}
    static FuncModeDraw::E &Ref();
    operator FuncModeDraw::E() { return Ref(); }
    static bool IsDisabled()   { return Ref() == Disabled; }
    static bool IsSeparate()   { return Ref() == Separate; }
};

struct MathFunction
{
    enum E
    {
        Sum,
        Mul
    };
    MathFunction() {}
    static MathFunction::E &Ref();
    operator MathFunction::E() { return Ref(); }
    static bool IsSum() { return Ref() == Sum; }
    static bool IsMul() { return Ref() == Mul; }
};

struct ModeRegSet
{
    enum E
    {
        Range,
        RShift
    };
    ModeRegSet() {}
    static ModeRegSet::E &Ref();
    operator ModeRegSet::E() { return Ref(); }
    static bool IsRange()    { return Ref() == Range; }
    static bool IsRShift()   { return Ref() == RShift; }
};

struct SettingsMath
{ //-V802
    FuncModeDraw::E modeDraw;       // ���������� ��� ����� ������� � ������ �������������� �������.
    MathFunction::E function;
    ModeRegSet::E   modeRegSet;     // ������� ����� ��������� - ������� �� ������� ��� �������� �� ���������.
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
