#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct FreqMeterEnabled
{
    enum E
    {
        Off,
        On
    };
};

/// ��� ���������� - ������ ��� �������
struct FreqMeterModeView
{
    enum E
    {
        Frequency,
        Period,
        Number
    };
};

/// ����� ����� �������.
struct FreqMeterTimeCounting
{
    enum E
    {
        _100ms,
        _1s,
        _10s
    };
};

/// ������� ����������� ��������� ��� ����� �������.
struct FreqMeterFreqClc
{
    enum E
    {
        _100kHz,
        _1MHz,
        _10MHz,
        _100MHz,
        Number
    };
};

/// ���������� ��������.
struct FreqMeterNumberPeriods
{
    enum E
    {
        _1,
        _10,
        _100,
        _1k,
        _10k,
        _100k,
        Number
    };
};

struct SettingsFreqMeter
{
    FreqMeterEnabled::E        enabled;
    FreqMeterModeView::E       modeView;
    FreqMeterTimeCounting::E   timeCounting;  ///< ����� ����� �������.
    FreqMeterFreqClc::E        freqClc;       ///< ������� ����������.
    FreqMeterNumberPeriods::E  numberPeriods; ///< ���������� ��������.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageFreqMeter
{
    static const Page *const self;

    static void Init();

    /// \todo ����� ���������� ��������. ������ ���������� �� ������������ ��������

    /// ���������� ��������� �� ��������� - ���������� - ����� ����� F
    static const Choice *GetChoiceTimeF();
    /// ���������� ��������� �� ��������� - ���������� - ����� �������
    static const Choice *GetChoiceFreqClc();
    /// ���������� ��������� �� ��������� - ���������� - ���-�� ��������
    static const Choice *GetChoiceNumPeriods();
};
