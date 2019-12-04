#pragma once


class Page;
class Choice;


struct FreqMeterEnabled
{
    enum E
    {
        Off,
        On
    };

    operator bool();
};

/// ��� ���������� - ������ ��� �������
struct FreqMeterModeView
{
    enum E
    {
        Frequency,
        Period,
        Count
    };

    static bool IsFrequency();
    static bool IsPeriod();
};

/// ����� ����� �������.
struct FreqMeterTimeCounting
{
    enum E
    {
        _100ms,
        _1s,
        _10s,
        Count
    };

    FreqMeterTimeCounting() {};
    bool Is100ms() const;
    operator FreqMeterTimeCounting::E();
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
        Count
    };

    FreqMeterFreqClc() {};
    operator FreqMeterFreqClc::E();
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
        Count
    };

    FreqMeterNumberPeriods() {};
    operator FreqMeterNumberPeriods::E();
};

struct SettingsFreqMeter
{
    FreqMeterEnabled::E        enabled;
    FreqMeterModeView::E       modeView;
    FreqMeterTimeCounting::E   timeCounting;  ///< ����� ����� �������.
    FreqMeterFreqClc::E        freqClc;       ///< ������� ����������.
    FreqMeterNumberPeriods::E  numberPeriods; ///< ���������� ��������.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
