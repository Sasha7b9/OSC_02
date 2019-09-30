#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Osci/FrequencyCounter.h"
#include "Settings/Settings.h"



static void OnChanged_Enable(bool)
{
    FrequencyCounter::Init();
}

static void OnChanged_FreqMeter_Period(bool)
{
    FrequencyCounter::LoadPeriodSettings();
}


DEF_CHOICE_2( cEnable,                                                                                                                            //--- ������� - ���������� - ���������� ---
    "����������",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.freq.enabled, &PageFreqMeter::self, Item::Active, OnChanged_Enable, Choice::AfterDraw
)



static bool IsActive_ModeView()
{
    return (set.freq.enabled == FreqMeterEnabled::On);
}

static void OnChanged_ModeView(bool)
{
    PageFreqMeter::Init();
}

DEF_CHOICE_2( cModeView,                                                                                                                               //--- ������� - ���������� - ����� ---
    "�����",
    "",
    "�������",
    "������",
    set.freq.modeView, &PageFreqMeter::self, IsActive_ModeView, OnChanged_ModeView, Choice::AfterDraw
)



static bool IsActive_SettingsFrequency()
{
    return (set.freq.enabled == FreqMeterEnabled::On) && (set.freq.modeView == FreqMeterModeView::Period);
}


static bool IsActive_TimeF()
{
    return (set.freq.enabled == FreqMeterEnabled::On) && (set.freq.modeView == FreqMeterModeView::Frequency);
}

static void OnChanged_TimeF(bool)
{
    FrequencyCounter::LoadFreqSettings();
}

DEF_CHOICE_3( cTimeF,                                                                                                                          //--- ������� - ���������� - ����� ����� F ---
    "����� ����� F",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "100��",
    "1�",
    "10�",
    set.freq.timeCounting, &PageFreqMeter::self, IsActive_TimeF, OnChanged_TimeF, Choice::AfterDraw
)


DEF_CHOICE_4( cFreqClc,                                                                                                                        //--- ������� - ���������� - ����� ������� ---
    "����� �������",
    "����� ������� ���������� ������� ���������",
    "100���",
    "1���",
    "10���",
    "100���",
    set.freq.freqClc, &PageFreqMeter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
)


DEF_CHOICE_6( cNumPeriods,                                                                                                                   //--- ������� - ���������� - ���-�� �������� ---
    "��� ��������",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "1",
    "10",
    "100",
    "1000",
    "10000",
    "100000",
    set.freq.numberPeriods, &PageFreqMeter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
)



const Choice *PageFreqMeter::GetChoiceTimeF()
{
    return (const Choice *)&cTimeF;
}

const Choice *PageFreqMeter::GetChoiceFreqClc()
{
    return (const Choice *)&cFreqClc;
}

const Choice *PageFreqMeter::GetChoiceNumPeriods()
{
    return (const Choice *)&cNumPeriods;
}

DEF_PAGE_5_VAR( pFreqMeter, // -V641                                                                                                                               //--- ������� - ���������� ---
    "����������",
    "",
    &cEnable,           ///< ��������� - ���������� - ����������
    &cModeView,         ///< ��������� - ���������� - �����
    &Item::empty,       ///< ��������� - ���������� - ����� ����� F / ����� �������
    &Item::empty,       ///< ��������� - ���������� - ���/�� ��������
    &Item::empty,
    PageName::Function_FrequencyCounter, &PageFunction::self, Item::Active, Choice::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageFreqMeter::self = (const Page *)&pFreqMeter;



void PageFreqMeter::Init()
{
    Page *page = (Page *)PageFreqMeter::self;

    Item **items = (Item **)page->OwnData()->items;

    if (set.freq.modeView == FreqMeterModeView::Frequency)
    {
        items[2] = (Item *)&cTimeF;
        items[3] = &Item::empty;
    }
    else if (set.freq.modeView == FreqMeterModeView::Period)
    {
        items[2] = (Item *)&cFreqClc;
        items[3] = (Item *)&cNumPeriods;
    }
    else
    {
        // ����� ������
    }
}
