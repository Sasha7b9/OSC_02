#pragma once
#include "FreqMeter/FreqMeter.h"


class Page;
class Choice;


struct SettingsFreqMeter
{
    uint8                        enabled;
    FreqMeter::ModeView::E       modeView;
    FreqMeter::TimeCounting::E   timeCounting;  // ����� ����� �������.
    FreqMeter::FreqClc::E        freqClc;       // ������� ����������.
    FreqMeter::NumberPeriods::E  numberPeriods; // ���������� ��������.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
