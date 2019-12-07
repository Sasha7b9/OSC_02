#pragma once


struct SettingsMultimeter
{
    Multimeter::AVP::E             avp;
    Multimeter::Measure::E         meas;
    Multimeter::RangeDC::E         rangeVoltageDC;
    Multimeter::RangeAC::E         rangeVoltageAC;
    Multimeter::RangeCurrentAC::E  rangeCurrentAC;     ///< ������ ��������� ����������� ����
    Multimeter::RangeCurrentDC::E  rangeCurrentDC;     ///< ������ ��������� ����������� ����
    Multimeter::RangeResistance::E rangeResist;
};



struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool);
    /// ���� ��������� ������� ���������� ��� �������� � ���� ����������
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;


    struct PageCalibration
    {
        static const Page *const self;
    };
};
