#pragma once


struct SettingsMultimeter
{
    AVP::E                          avp;
    MultimeterMeasure::E            meas;
    RangeDC::E                      rangeVoltageDC;
    Multimeter::RangeAC::E          rangeVoltageAC;
    Multimeter::RangeCurrentAC::E   rangeCurrentAC;     ///< ������ ��������� ����������� ����
    Multimeter::RangeCurrentDC::E   rangeCurrentDC;     ///< ������ ��������� ����������� ����
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
