#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    class MultimeterWorker;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Multimeter
{
    class DisplayWorker;

    /// �������������
    void Init();

    void DeInit();

    void Update();
    /// �������� �����������, ��� ��������� ��������� ������ � ����� ��������� ���������� ��������� ����� �������
    void ChangeMode();

    void Calibrate(int calibr);

    void ChangeAVP();
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
            Size
        } value;
        explicit Measure(E v) : value(v) { };
        char Symbol() const
        {
            static const char symbols[Size] = {'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
            return symbols[value]; //-V2006
        }
        /// �������� ��� ��������� �� ��������� ������
        static Measure::E GetCode(const char buffer[13]);
    };

    /// ������������ ��� ���������
    class Display
    {
    friend class DisplayWorker;
    friend class ::Display::MultimeterWorker;
    public:
        static void Update();
        /// ����� ��� ������� ��������� ��������� �� �������
        static void SetMeasure(const uint8 buffer[13]);

        static void ChangedMode();
    };
};
