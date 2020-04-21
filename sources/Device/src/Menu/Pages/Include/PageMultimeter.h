#pragma once


struct SettingsMultimeter
{
    ModeAVP::E              avp;
    MultimeterMeasure::E    meas;
    RangeVoltageDC::E       rangeVoltageDC;
    RangeVoltageAC::E       rangeVoltageAC;
    RangeCurrentAC::E       rangeCurrentAC;     // ������ ��������� ����������� ����
    RangeCurrentDC::E       rangeCurrentDC;     // ������ ��������� ����������� ����
    RangeResistance::E      rangeResist;
};



struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool = true);
    // ���� ��������� ������� ���������� ��� �������� � ���� ����������
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;


    struct Calibration
    {
        static const Page *const self;
    };
};
