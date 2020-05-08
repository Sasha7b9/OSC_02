#pragma once


struct DisplayMultimeter
{
    static void Update();
    
    // ����� ��� ������� ��������� ��������� �� �������
    static void SetMeasure(const uint8 buffer[13]);

    static void ChangedMode();

    static const int HEIGHT = 50;
};


struct Multimeter
{
    // �������������
    static void Init();

    static void DeInit();

    static void Update();
    
    // �������� �����������, ��� ��������� ��������� ������ � ����� ��������� ���������� ��������� ����� �������
    static void ChangeMode();

    static void Calibrate(int calibr);

    static void ChangeAVP();
    
    // ���� (zero != 0) - ������� ����� ����
    static void LoadZero(int zero);
};


// ����� ��������� �����������
struct MultimeterMeasure
{
#define S_MULT_MEASURE                  (set.mult._meas)
#define S_MULT_MEASURE_IS_RESISTANCE    (S_MULT_MEASURE == MultimeterMeasure::Resistance)
#define S_MULT_MEASURE_IS_TEST_DIODE    (S_MULT_MEASURE == MultimeterMeasure::TestDiode)
#define S_MULT_MEASURE_IS_BELL          (S_MULT_MEASURE == MultimeterMeasure::Bell)

    enum E
    {
        VoltageDC,
        VoltageAC,
        CurrentDC,
        CurrentAC,
        Resistance,
        TestDiode,
        Bell,
        Count
    };
    static char Symbol();

    // �������� ��� ��������� �� ��������� ������
    static MultimeterMeasure::E GetCode(const char buffer[13]);
};


// ��� - ��������� �������
struct ModeAVP
{
#define S_MULT_MODE_AVP     (set.mult._avp)
#define S_MULT_AVP_DISABLED (S_MULT_MODE_AVP == ModeAVP::Off)
#define S_MULT_AVP_ENABLED  (S_MULT_MODE_AVP == ModeAVP::On)

    enum E
    {
        Off,
        On
    };
};


// ������ ��������� ����������� ����������
struct RangeVoltageDC
{
#define S_MULT_RANGE_VOLTAGE_DC (set.mult._rangeVoltageDC)

    enum E
    {
        _2V,
        _20V,
        _500V
    };
};


// ������ ��������� ����������� ����������
struct RangeVoltageAC
{
#define S_MULT_RANGE_VOLTAGE_AC     (set.mult._rangeVoltageAC)

    enum E
    {
        _2V,
        _20V,
        _400V
    };
};


// ������ ��������� ����������� ����
struct RangeCurrentAC
{
#define S_MULT_RANGE_CURRENT_AC     (set.mult._rangeCurrentAC)

    enum E
    {
        _20mA,
        _2A
    };
};


// ������ ��������� ����������� ����
struct RangeCurrentDC
{
#define S_MULT_RANGE_CURRENT_DC     (set.mult._rangeCurrentDC)

    enum E
    {
        _20mA,
        _2A
    };
};


// ������ ��������� ������������ ����������� ����
struct RangeResistance
{
#define S_MULT_RANGE_RESISTANCE     (set.mult._rangeResist)

    enum E
    {
        _2k,
        _20k,
        _200k,
        _10M
    };
};
