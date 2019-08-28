#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


extern const Page pageHoldOff;

const Page * const PageTrig::PageHoldOff::self = (const Page *)&pageHoldOff;


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
    TRIG_HOLDOFF_ENABLED,
    &PageTrig::PageHoldOff::self, 0, OnChanged_HoldOff, 0
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Duration()
{
    Osci::Settings::LoadHoldfOff();
}

DEF_GOVERNOR( gDuration,                                                                                                                           //--- ����� - ��������� - ������������ ---
    "������������",
    "����� ������������ ���������� ����� ���������� �������������",
    TRIG_HOLDOFF, 10, 32000,
    &PageTrig::PageHoldOff::self, 0, OnChanged_Duration, 0
)

/// \todo gDuration ������ ������������ ����������� �������� �������� ��������, ��� 32000


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pageHoldOff, // -V641 // -V1027                                                                                                                       //--- ����� - ��������� ---
    "���������",
    "",
    &cHoldOff,          ///< ����� - ��������� - ���������
    &gDuration,         ///< ����� - ��������� - ������������
    PageName::Trig_HoldOff,
    &PageTrig::self, E_BtV, E_VB, E_VV, E_BfKE
)
