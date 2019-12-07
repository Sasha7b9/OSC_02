#pragma once


struct DisplayMultimeter
{
    static void Update();
    /// ����� ��� ������� ��������� ��������� �� �������
    static void SetMeasure(const uint8 buffer[13]);

    static void ChangedMode();
};


struct Multimeter
{
    /// �������������
    static void Init();

    static void DeInit();

    static void Update();
    /// �������� �����������, ��� ��������� ��������� ������ � ����� ��������� ���������� ��������� ����� �������
    static void ChangeMode();

    static void Calibrate(int calibr);

    static void ChangeAVP();

    /// ��� - ��������� �������
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

    /// ����� ��������� �����������
    struct Measure
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
        Measure() {}
        static Measure::E &Ref();
        operator Measure::E() { return Ref(); }
        static char Symbol()
        {
            static const char symbols[Count] = { 'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
            return symbols[Ref()]; //-V2006
        }
        /// �������� ��� ��������� �� ��������� ������
        static Measure::E GetCode(const char buffer[13]);
        static bool IsVoltageDC()  { return Ref() == VoltageDC; }
        static bool IsVoltageAC()  { return Ref() == VoltageAC; }
        static bool IsResistance() { return Ref() == Resistance; }
        static bool IsCurrentDC()  { return Ref() == CurrentDC; }
        static bool IsCurrentAC()  { return Ref() == CurrentAC; }
        static bool IsTestDiode()  { return Ref() == TestDiode; }
    };
};
