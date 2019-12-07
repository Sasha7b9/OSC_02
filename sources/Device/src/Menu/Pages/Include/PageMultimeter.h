#pragma once


/// ������ ��������� ����
struct MultimeterRangeCurrent
{
    enum E
    {
        _20mA,
        _2A
    };
};

/// ������ ��������� ������������ ����������� ����
struct MultimeterRangeResistance
{
    enum E
    {
        _2k,
        _20k,
        _200k,
        _10M
    };
};

struct SettingsMultimeter
{
    Multimeter::AVP::E            avp;
    Multimeter::Measure::E        meas;
    Multimeter::RangeDC::E        rangeVoltageDC;
    Multimeter::RangeAC::E        rangeVoltageAC;
    MultimeterRangeCurrent::E     rangeCurrentAC;     ///< ������ ��������� ����������� ����
    MultimeterRangeCurrent::E     rangeCurrentDC;     ///< ������ ��������� ����������� ����
    MultimeterRangeResistance::E  rangeResist;
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
