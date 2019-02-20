#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MULTI_MEASURE           (set.multi_meas)
#define MULTI_RANGE_VOLTAGE_AC  (set.multi_rangeAC)
#define MULTI_RANGE_VOLTAGE_DC  (set.multi_rangeDC)
#define MULTI_RANGE_CURRENT_AC  (set.multi_rangeCurrentAC)
#define MULTI_RANGE_CURRENT_DC  (set.multi_rangeCurrentDC)
#define MULTI_RANGE_RESISTANCE  (set.multi_rangeResist)
#define MULTI_AVP               (set.multi_avp)

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
            Number
        } value;
        explicit Measure(E v) : value(v) { };
        char Symbol() const
        {
            static const char symbols[Number] = {'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
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
