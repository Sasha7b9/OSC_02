#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


extern const PageBase pageHoldOff;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTrig::PageHoldOff::pointer = &pageHoldOff;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_HoldOff(bool)
{
    Osci::Settings::LoadHoldfOff();
}

DEF_CHOICE_2( cHoldOff,                                                                                                                               //--- ����� - ��������� - ��������� ---
    "���������",
    "��������/��������� ����� �������� �������������"
    ,
    DISABLE_RU,
    ENABLE_RU,
    TRIG_HOLDOFF_ENABLED, pageHoldOff, FuncActive, OnChanged_HoldOff, Choice::EmptyDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Duration()
{
    Osci::Settings::LoadHoldfOff();
}

DEF_GOVERNOR( gDuration,                                                                                                                           //--- ����� - ��������� - ������������ ---
    "������������",
    "����� ������������ ���������� ����� ���������� �������������",
    TRIG_HOLDOFF, 10, 15000000, pageHoldOff, FuncActive, OnChanged_Duration, FuncBeforeDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pageHoldOff, // -V641 // -V1027                                                                                                                       //--- ����� - ��������� ---
    "���������",
    "",
    &cHoldOff,          ///< ����� - ��������� - ���������
    &gDuration,         ///< ����� - ��������� - ������������
    Page::Name::Trig_HoldOff, PageTrig::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
