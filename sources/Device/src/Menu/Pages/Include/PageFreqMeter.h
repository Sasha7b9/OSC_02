#pragma once
#include "Osci/FreqMeter.h"


class Page;
class Choice;


/// ��� ���������� - ������ ��� �������
struct FreqMeterModeView
{
    enum E
    {
        Frequency,
        Period,
        Count
    };
    static FreqMeterModeView::E &Ref();
    static bool IsFrequency() { return Ref() == Frequency; }
    static bool IsPeriod()    { return Ref() == Period; }
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
    static FreqMeterTimeCounting::E &Ref();
    operator FreqMeterTimeCounting::E() { return Ref(); }
    static bool Is100ms()               { return Ref() == _100ms; }
};


struct SettingsFreqMeter
{
    FreqMeter::Enabled::E        enabled;
    FreqMeterModeView::E         modeView;
    FreqMeterTimeCounting::E     timeCounting;  ///< ����� ����� �������.
    FreqMeter::FreqClc::E        freqClc;       ///< ������� ����������.
    FreqMeter::NumberPeriods::E  numberPeriods; ///< ���������� ��������.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
