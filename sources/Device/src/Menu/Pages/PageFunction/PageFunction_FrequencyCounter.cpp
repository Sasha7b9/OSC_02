#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Osci/FrequencyCounter.h"
#include "Settings/Settings.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_FreqMeter_Enable(bool)
{
    FrequencyCounter::Init();
}

static void OnChanged_FreqMeter_Frequency(bool)
{
    FrequencyCounter::LoadFreqSettings();
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
    FREQ_METER_ENABLED, &PageFrequencyCounter::self, E_BtV, OnChanged_FreqMeter_Enable, E_VII
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ModeView()
{
    return FREQ_METER_IS_ENABLED;
}

DEF_CHOICE_2( cModeView,                                                                                                                               //--- ������� - ���������� - ����� ---
    "�����",
    "",
    "�������",
    "������",
    FREQ_METER_MODE_VIEW, &PageFrequencyCounter::self, IsActive_ModeView, E_VB, E_VII
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SettingsFrequency()
{
    return FREQ_METER_IS_ENABLED && FREQ_METER_MODE_VIEW_IS_PERIOD;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SettingsPeriod()
{
    return FREQ_METER_IS_ENABLED && FREQ_METER_MODE_VIEW_IS_FREQUENCY;
}

DEF_CHOICE_3( cTimeF,                                                                                                                          //--- ������� - ���������� - ����� ����� F ---
    "����� ����� F",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "100��",
    "1�",
    "10�",
    FREQ_METER_TIMECOUNTING, &PageFrequencyCounter::self, IsActive_SettingsPeriod, OnChanged_FreqMeter_Frequency, E_VII
)


DEF_CHOICE_4( cFreqClc,                                                                                                                        //--- ������� - ���������� - ����� ������� ---
    "����� �������",
    "����� ������� ���������� ������� ���������",
    "100���",
    "1���",
    "10���",
    "100���",
    FREQ_METER_FREQ_CLC, &PageFrequencyCounter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_6( cNumPeriods,                                                                                                                   //--- ������� - ���������� - ���-�� �������� ---
    "��� ��������",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "1",
    "10",
    "100",
    "1000",
    "10000",
    "100000",
    FREQ_METER_NUM_PERIODS, &PageFrequencyCounter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, E_VII
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Choice *PageFrequencyCounter::GetChoiceTimeF()
{
    return (const Choice *)&cTimeF;
}

const Choice *PageFrequencyCounter::GetChoiceFreqClc()
{
    return (const Choice *)&cFreqClc;
}

const Choice *PageFrequencyCounter::GetChoiceNumPeriods()
{
    return (const Choice *)&cNumPeriods;
}

DEF_PAGE_5( pFreqMeter, // -V641                                                                                                                               //--- ������� - ���������� ---
    "����������",
    "",
    &cEnable,           ///< ��������� - ���������� - ����������
    &cModeView,         ///< ��������� - ���������� - �����
    &cTimeF,            ///< ��������� - ���������� - ����� ����� F
    &cFreqClc,          ///< ��������� - ���������� - ����� �������
    &cNumPeriods,       ///< ��������� - ���������� - ���-�� ��������
    PageName::Function_FrequencyCounter, &PageFunction::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageFrequencyCounter::self = (const Page *)&pFreqMeter;
