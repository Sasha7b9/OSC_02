#pragma once


struct DisplayMultimeter
{
    static void Update();
    // ����� ��� ������� ��������� ��������� �� �������
    static void SetMeasure(const uint8 buffer[13]);

    static void ChangedMode();
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

    // ��� - ��������� �������
    struct AVP
    {
        enum E
        {
            Off,
            On
        };

        static AVP::E &Ref();
        static bool IsEnabled() { return Ref() == On; }
    };

    // ������ ��������� ����������� ����������
    struct RangeDC
    {
        enum E
        {
            _2V,
            _20V,
            _500V
        };
        static RangeDC::E &Ref();
        operator RangeDC::E() { return Ref(); }
    };

    // ������ ��������� ����������� ����������
    struct RangeAC
    {
        enum E
        {
            _2V,
            _20V,
            _400V
        };
        static RangeAC::E &Ref();
        operator RangeAC::E() { return Ref(); }
    };

    // ������ ��������� ����������� ����
    struct RangeCurrentAC
    {
        enum E
        {
            _20mA,
            _2A
        };
        static RangeCurrentAC::E &Ref();
        operator RangeCurrentAC::E() { return Ref(); }
    };

    // ������ ��������� ����������� ����
    struct RangeCurrentDC
    {
        enum E
        {
            _20mA,
            _2A
        };
        static RangeCurrentDC::E &Ref();
        operator RangeCurrentDC::E() { return Ref(); }
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
        static RangeResistance::E &Ref();
        operator RangeResistance::E() { return Ref(); }
    };
};
