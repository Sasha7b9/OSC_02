#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageMultimeter;
extern const PageBase pageCalibration;

const PageBase *PageFunction::PageMultimeter::pointer = &pageMultimeter;
const PageBase *PageFunction::PageMultimeter::PageCalibration::pointer = &pageCalibration;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool FuncActive_RangesVoltageDC()
{
    return MULTI_MEASURE == Multimeter::Measure::VoltageDC;
}

static void OnChange_VoltageDC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3( cRangesVoltageDC,                                                                                                                                 //--- ���������� - ������ ---
    "������",
    "�������� ���������",
    "2 �",
    "20 �",
    "500 �",
    MULTI_RANGE_VOLTAGE_DC, pageMultimeter, FuncActive_RangesVoltageDC, OnChange_VoltageDC, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncActive_RnagesVoltageAC()
{
    return MULTI_MEASURE == Multimeter::Measure::VoltageAC;
}

static void OnChange_VoltageAC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3( cRangesVoltageAC,                                                                                                                                 //--- ���������� - ������ ---
    "������",
    "�������� ���������",
    "2 �",
    "20 �",
    "400 �",
    MULTI_RANGE_VOLTAGE_AC, pageMultimeter, FuncActive_RnagesVoltageAC, OnChange_VoltageAC, 0
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_CurrentDC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentDC,
    "������",
    "�������� ���������",
    "20��",
    "2�",
    MULTI_RANGE_CURRENT_DC, pageMultimeter, 0, OnChange_CurrentDC, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_CurrentAC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentAC,
    "������",
    "�������� ���������",
    "20��",
    "2�",
    MULTI_RANGE_CURRENT_AC, pageMultimeter, 0, OnChange_CurrentAC, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncActive_RangesReistance()
{
    return MULTI_MEASURE == Multimeter::Measure::Resistance;
}

static void OnChange_Resistance(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_4(   cRangesResistance,                                                                                                                              //--- ���������� - ������ ---
    "������",
    "�������� ���������",
    "2 ���",
    "20 ���",
    "200 ���",
    "10 ���",
    MULTI_RANGE_RESISTANCE, pageMultimeter, FuncActive_RangesReistance, OnChange_Resistance, 0
)

DEF_CHOICE_7(   cMode,
    "�����"
    ,
    "����� ���� ����������\n"
    "U= - ���������� ����������\n"
    "U~ - ���������� ����������\n"
    "I= - ���������� ���\n"
    "I~ - ���������� ���\n"
    "R - ������������� ����������� ����\n"
    "VD - ���� ������\n"
    "BELL - ���������"
    ,
    "U=",
    "U~",
    "I=",
    "I~",
    "R",
    "VD",
    "���������",
    MULTI_MEASURE, pageMultimeter, 0, PageFunction::PageMultimeter::OnChanged_Mode, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_AVP(bool)
{
    Multimeter::ChangeAVP();
}

DEF_CHOICE_2 (cAVP,
    "���",
    "��������� �������",
    DISABLE_RU,
    ENABLE_RU,
    MULTI_AVP, pageMultimeter, 0, OnChanged_AVP, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Zero(bool)
{
    //Multimeter::LoadZero();
}

static int8 zero = 0;

DEF_CHOICE_2(cZero,
    "����",
    "",
    DISABLE_RU,
    ENABLE_RU,
    zero, pageMultimeter, 0, OnChanged_Zero, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageFunction::PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

static void OnPress_Page(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Multimeter : Device::Mode::Osci);
}

DEF_PAGE_5( pageMultimeter, // -V641
    "����������",
    "���������� �������� � ������ �����������",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    &cZero,
    &PageFunction::PageMultimeter::PageCalibration::pointer,
    Page::Name::Function_Multimeter, PageFunction::pointer, 0, OnPress_Page, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageFunction::PageMultimeter::OnChanged_Mode(bool)
{
    PageBase *page = (PageBase *)&pageMultimeter;

    Control **items = (Control **)page->items;

    if (MULTI_MEASURE == Multimeter::Measure::VoltageDC)
    {
        items[1] = (Control *)&cRangesVoltageDC; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::VoltageAC)
    {
        items[1] = (Control *)&cRangesVoltageAC; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentDC)
    {
        items[1] = (Control *)&cRangesCurrentDC;  // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentAC)
    {
        items[1] = (Control *)&cRangesCurrentAC;    // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::Resistance)
    {
        items[1] = (Control *)&cRangesResistance; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::TestDiode)
    {
        items[1] = nullptr;
    }
    else
    {
        // ����� ������
    }

    Multimeter::ChangeMode();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Calibrate0()
{
    Multimeter::Calibrate(0);
}

DEF_BUTTON(bCalibrate0,
    "������ 0",
    "",
    pageMultimeter, 0, OnPress_Calibrate0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    Multimeter::Calibrate(1);
}

DEF_BUTTON(bCalibrate1,
    "������ 1",
    "",
    pageMultimeter, 0, OnPress_Calibrate1, 0
)


DEF_PAGE_2( pageCalibration,
    "����������",
    "���������� �����������",
    &bCalibrate0,
    &bCalibrate1,
    Page::Name::Function_Multimeter_Cal, PageFunction::PageMultimeter::pointer, 0, 0, 0, 0
)
