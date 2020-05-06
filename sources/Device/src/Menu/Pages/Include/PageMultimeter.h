#pragma once


struct SettingsMultimeter
{
    ModeAVP::E              _avp;
    MultimeterMeasure::E    _meas;
    RangeVoltageDC::E       rangeVoltageDC;
    RangeVoltageAC::E       rangeVoltageAC;
    RangeCurrentAC::E       rangeCurrentAC;     // ������ ��������� ����������� ����
    RangeCurrentDC::E       rangeCurrentDC;     // ������ ��������� ����������� ����
    RangeResistance::E      rangeResist;
    bool                    showAlways;         // true - ���������� � ������ ������������ ����
};



struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool = true);

    // ���������� true, ���� "����" - "���"
    static bool ZeroEnabled();
    
    // ���� ��������� ������� ���������� ��� �������� � ���� ����������
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;


    struct Calibration
    {
        static const Page *const self;
    };
};
