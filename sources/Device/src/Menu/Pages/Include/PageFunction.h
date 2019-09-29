#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"
#include "Osci/Measurements/Measures.h"
#include "PageDisplay.h"
#include "Multimeter/Multimeter.h"


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

struct ScaleFFT
{
    enum E
    {
        Log,           ///< ��� �������� �������� ��������������� ����� ������ �������.
        Linear         ///< ��� �������� �������� �������� ����� ������ �������.
    } value;
    explicit ScaleFFT(E v) : value(v) {};
};

struct SourceFFT
{
    enum E
    {
        A,
        B,
        Both
    } value;
    explicit SourceFFT(E v) : value(v) {};
};

struct WindowFFT
{
    enum E
    {
        Rectangle,
        Hamming,
        Blackman,
        Hann
    } value;
    explicit WindowFFT(E v) : value(v) {};
};

struct MaxDBFFT
{
    enum E
    {
        _40,
        _60,
        _80
    } value;
    explicit MaxDBFFT(E v) : value(v) {};
    static float MaxDBforFFT(MaxDBFFT::E maxDB)
    {
        static const float arrayMAX_DB_FOR_FFT[] = { -40.0F, -60.0F, -80.0F };

        return arrayMAX_DB_FOR_FFT[static_cast<int>(maxDB)];
    }
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
    uint16          rShift;
    Range::E        range;
};

struct SettingsFFT
{
    bool            enabled;
    uint8           posCur[2];      ///< ������� ������� �������. ���������� 0...256.
    ScaleFFT::E     scale;
    SourceFFT::E    source;
    WindowFFT::E    window;
    MaxDBFFT::E     maxDB;
    uint8           cursor;         ///< ����������, ����� �������� ������� ��������� ����� ���������.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct FreqMeterEnabled
{
    enum E
    {
        Off,
        On
    } value;
    explicit FreqMeterEnabled(E v) : value(v) {};
};

/// ��� ���������� - ������ ��� �������
struct FreqMeterModeView
{
    enum E
    {
        Frequency,
        Period,
        Number
    } value;
    explicit FreqMeterModeView(E v) : value(v) {};
};

/// ����� ����� �������.
struct FreqMeterTimeCounting
{
    enum E
    {
        _100ms,
        _1s,
        _10s
    } value;
    explicit FreqMeterTimeCounting(E v) : value(v) {};
};

/// ������� ����������� ��������� ��� ����� �������.
struct FreqMeterFreqClc
{
    enum E
    {
        _100kHz,
        _1MHz,
        _10MHz,
        _100MHz,
        Number
    } value;
    explicit FreqMeterFreqClc(E v) : value(v) {};
};

/// ���������� ��������.
struct FreqMeterNumberPeriods
{
    enum E
    {
        _1,
        _10,
        _100,
        _1k,
        _10k,
        _100k,
        Number
    } value;
    explicit FreqMeterNumberPeriods(E v) : value(v) {};
};

struct SettingsFreqMeter
{
    FreqMeterEnabled::E        enabled;
    FreqMeterModeView::E       modeView;
    FreqMeterTimeCounting::E   timeCounting;  ///< ����� ����� �������.
    FreqMeterFreqClc::E        freqClc;       ///< ������� ����������.
    FreqMeterNumberPeriods::E  numberPeriods; ///< ���������� ��������.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��� ����� ��������� � ������-���������� - ����������� ��� �����
struct TesterControl
{
    enum E
    {
        Voltage,
        Current
    } value;
    explicit TesterControl(E v) : value(v) {};
};

struct TesterPolarity
{
    enum E
    {
        Positive,
        Negative
    } value;
    explicit TesterPolarity(E v) : value(v) {};
};

struct TesterStepU
{
    enum E
    {
        _100mV,
        _500mV
    } value;
    explicit TesterStepU(E v) : value(v) {};
};

struct TesterStepI
{
    enum E
    {
        _4mA,
        _20mA
    } value;
    explicit TesterStepI(E v) : value(v) {};
};

struct TesterViewMode
{
    enum E
    {
        Lines,
        Points
    } value;
    explicit TesterViewMode(E v) : value(v) {};
};

struct SettingsTester
{
    TesterControl::E    control;
    TesterPolarity::E   polarity;
    TesterStepU::E      stepU;
    TesterStepI::E      stepI;
    ENumAverage::E      ave;                   ///< ���������� ����������
    TesterViewMode::E   viewMode;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��� - ��������� �������
struct MultimeterAVP
{
    enum E
    {
        Off,
        On
    } value;
    explicit MultimeterAVP(E v) : value(v) { };
};

/// ������ ��������� ����������� ����������
struct MultimeterRangeDC
{
    enum E
    {
        _2V,
        _20V,
        _500V
    } value;
    explicit MultimeterRangeDC(E v) : value(v) { };
};

/// ������ ��������� ����������� ����������
struct MultimeterRangeAC
{
    enum E
    {
        _2V,
        _20V,
        _400V
    } value;
    explicit MultimeterRangeAC(E v) : value(v) {};
};

/// ������ ��������� ����
struct MultimeterRangeCurrent
{
    enum E
    {
        _20mA,
        _2A
    } value;
    explicit MultimeterRangeCurrent(E v) : value(v) {};
};

/// ������ ��������� ������������ ����������� ����
struct MultimeterRangeResistance
{
    enum E
    {
        _2k,
        _20k,
        _200k,
        _10M
    } value;
    explicit MultimeterRangeResistance(E v) : value(v) {};
};

struct SettingsMultimeter
{
    MultimeterAVP::E              avp;
    Multimeter::Measure::E        meas;
    MultimeterRangeDC::E          rangeVoltageDC;
    MultimeterRangeAC::E          rangeVoltageAC;
    MultimeterRangeCurrent::E     rangeCurrentAC;     ///< ������ ��������� ����������� ����
    MultimeterRangeCurrent::E     rangeCurrentDC;     ///< ������ ��������� ����������� ����
    MultimeterRangeResistance::E  rangeResist;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageFunction
{
    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageFrequencyCounter
{
    static const Page * const self;

    static void Init();

    /// \todo ����� ���������� ��������. ������ ���������� �� ������������ ��������

    /// ���������� ��������� �� ��������� - ���������� - ����� ����� F
    static const Choice *GetChoiceTimeF();
    /// ���������� ��������� �� ��������� - ���������� - ����� �������
    static const Choice *GetChoiceFreqClc();
    /// ���������� ��������� �� ��������� - ���������� - ���-�� ��������
    static const Choice *GetChoiceNumPeriods();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageFFT
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageCursors
    {
        static const Page * const self;
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool);
    /// ���� ��������� ������� ���������� ��� �������� � ���� ����������
    static void DecodePassword(const KeyEvent &event);

    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageCalibration
    {
        static const Page * const self;
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageTester
{
    static void Init();

    static void OnChanged_Control(bool);

    static const Page * const self;
};
