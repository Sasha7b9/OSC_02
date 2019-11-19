#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"


/// �� ������ ������ ����������� �������������� ���������
#define SOURCE_MEASURES                 (set.meas.source)
/// �������������� ��������� ������������ ������ �� ������ A
#define SOURCE_MEASURES_IS_A            (SOURCE_MEASURES == MeasuresSource::A)
/// �������������� ��������� ������������ ������ �� ������ B
#define SOURCE_MEASURES_IS_B            (SOURCE_MEASURES == MeasuresSource::B)
/// �������������� ��������� ������������ �� ������� A � B
#define SOURCE_MEASURES_IS_BOTH         (SOURCE_MEASURES == MeasuresSource::A_B)
/// �������� �������������� ��������� �� ������ A
#define VIEW_MEASURES_A                 (set.ch[Chan::A].enabled && (SOURCE_MEASURES_IS_A || SOURCE_MEASURES_IS_BOTH))
/// �������� �������������� ��������� �� ������ B
#define VIEW_MEASURES_B                 (set.ch[Chan::B].enabled && (SOURCE_MEASURES_IS_B || SOURCE_MEASURES_IS_BOTH))


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
    } value;
    explicit MeasuresOnDisplay(E v) : value(v) {};
};

struct MeasuresSource
{
    enum E
    {
        A,
        B,
        A_B
    } value;
    explicit MeasuresSource(E v) : value(v) {};
};

/// ������� �� ������� ��� ������ ���������.
struct MeasuresModeViewSignals
{
    enum E
    {
        AsIs,       ///< ���������� ������� ��� ����.
        Compress    ///< ������� ����� � ���������.
    } value;
    explicit MeasuresModeViewSignals(E v) : value(v) {};
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
    } value;
    explicit CursorsLookMode(E v) : value(v) {};
};

/// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
struct CursorsActive
{
    enum E
    {
        U,
        T,
        None
    } value;
    explicit CursorsActive(E v) : value(v) {};
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
    } value;
    explicit CursorsControl(E v) : value(v) {};
};

/// ������������ ����������� ��������.
struct CursorsMovement
{
    enum E
    {
        Pixels,    ///< �� �������� ������
        Percents   ///< �� ���������
    } value;
    explicit CursorsMovement(E v) : value(v) {};
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
    } value;
    explicit FuncModeDraw(E v) : value(v) {};
};

struct MathFunction
{
    enum E
    {
        Sum,
        Mul
    } value;
    explicit MathFunction(E v) : value(v) {};
};

struct ModeRegSet
{
    enum E
    {
        Range,
        RShift
    } value;
    explicit ModeRegSet(E v) : value(v) {};
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
