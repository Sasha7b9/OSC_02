#pragma once


struct DisplayMultimeter
{
    static void Update();
    
    // ����� ��� ������� ��������� ��������� �� �������
    static void SetMeasure(const uint8 buffer[13]);

    static void ChangedMode();
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
struct AVP
{
    enum E
    {
        Off,
        On
    };

    static bool IsEnabled();
};


// ������ ��������� ����������� ����������
struct RangeVoltageDC
{
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
    enum E
    {
        _20mA,
        _2A
    };
};


// ������ ��������� ����������� ����
struct RangeCurrentDC
{
    enum E
    {
        _20mA,
        _2A
    };
};


// ������ ��������� ������������ ����������� ����
struct RangeResistance
{
    enum E
    {
        _2k,
        _20k,
        _200k,
        _10M
    };
};
