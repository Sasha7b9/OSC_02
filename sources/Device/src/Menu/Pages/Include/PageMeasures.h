#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"
#include "Osci/Measurements/Measures.h"


/// ������� �� ����� ��� ������ ���������
#define MODE_VIEW_SIGNALS               (set.meas.modeViewSignals) 
#define MODE_VIEW_SIGNALS_IS_COMPRESS   (MODE_VIEW_SIGNALS == MeasuresModeViewSignals::Compress)

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
/// �������� �������������� ��������� �� ����� �������
#define VIEW_MEASURES_BOTH              (SET_ENABLED_BOTH && SOURCE_MEASURES_IS_BOTH)

#define SHOW_MEASURES                   (set.meas.show)

#define MEAS_MARKED                     (set.meas.marked)

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


struct SettingsMeasures
{
    bool                        show;              ///< ���������� �� ���������.
    MeasuresOnDisplay::E        number;            ///< ������� ��������� ��������.
    MeasuresSource::E           source;            ///< ��� ����� ������� �������� ���������.
    MeasuresModeViewSignals::E  modeViewSignals;   ///< ������� �� ������� ��� ������ ���������.
    Measure::Type::E            measures[15];      ///< ��������� ��� ��������� ���������.
    Measure::Type::E            marked;            ///< ���������, �� ������� ����� �������� �������.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasures
{
    static const Page * const self;

    static const Page *GetPageFrequencyCounter();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresAuto
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PageTune
    {
    public:
        static const Page * const self;
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresCursors
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

        static bool OnArrows(const KeyEvent &event);

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresMath
{
    static const Page * const self;
};
