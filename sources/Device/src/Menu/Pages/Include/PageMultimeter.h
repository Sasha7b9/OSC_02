#pragma once


struct SettingsMultimeter
{
    AVP::E                          avp;
    MultimeterMeasure::E            meas;
    RangeVoltageDC::E               rangeVoltageDC;
    RangeVoltageAC::E               rangeVoltageAC;
    RangeCurrentAC::E               rangeCurrentAC;     ///< ������ ��������� ����������� ����
    RangeCurrentDC::E               rangeCurrentDC;     ///< ������ ��������� ����������� ����
    Multimeter::RangeResistance::E  rangeResist;
};



struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool);
    // ���� ��������� ������� ���������� ��� �������� � ���� ����������
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;


    struct Calibration
    {
        static const Page *const self;
    };
};
