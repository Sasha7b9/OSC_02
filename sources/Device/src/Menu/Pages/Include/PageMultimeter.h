#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��� - ��������� �������
struct MultimeterAVP
{
    enum E
    {
        Off,
        On
    } value;
    explicit MultimeterAVP(E v) : value(v) { };
};

/// ������ ��������� ����������� ����������
struct MultimeterRangeDC
{
    enum E
    {
        _2V,
        _20V,
        _500V
    } value;
    explicit MultimeterRangeDC(E v) : value(v) { };
};

/// ������ ��������� ����������� ����������
struct MultimeterRangeAC
{
    enum E
    {
        _2V,
        _20V,
        _400V
    } value;
    explicit MultimeterRangeAC(E v) : value(v) {};
};

/// ������ ��������� ����
struct MultimeterRangeCurrent
{
    enum E
    {
        _20mA,
        _2A
    } value;
    explicit MultimeterRangeCurrent(E v) : value(v) {};
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
    } value;
    explicit MultimeterRangeResistance(E v) : value(v) {};
};

/// ����� ��������� �����������
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
        Size
    } value;
    explicit MultimeterMeasure(E v) : value(v) { };
    char Symbol() const
    {
        static const char symbols[Size] = { 'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
        return symbols[value]; //-V2006
    }
    /// �������� ��� ��������� �� ��������� ������
    static MultimeterMeasure::E GetCode(const char buffer[13]);
};

struct SettingsMultimeter
{
    MultimeterAVP::E              avp;
    MultimeterMeasure::E          meas;
    MultimeterRangeDC::E          rangeVoltageDC;
    MultimeterRangeAC::E          rangeVoltageAC;
    MultimeterRangeCurrent::E     rangeCurrentAC;     ///< ������ ��������� ����������� ����
    MultimeterRangeCurrent::E     rangeCurrentDC;     ///< ������ ��������� ����������� ����
    MultimeterRangeResistance::E  rangeResist;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool);
    /// ���� ��������� ������� ���������� ��� �������� � ���� ����������
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageCalibration
    {
        static const Page *const self;
    };
};
