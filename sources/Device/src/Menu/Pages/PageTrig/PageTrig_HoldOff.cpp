#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"



static void OnChanged_HoldOff(bool)
{
    Osci::LoadHoldfOff();
}

DEF_CHOICE_2( cHoldOff,                                                                                                                               //--- ����� - ��������� - ��������� ---
    "���������",
    "��������/��������� ����� �������� �������������"
    ,
    DISABLE_RU,
    ENABLE_RU,
    set.trig.holdOffEnabled, &PageTrig::PageHoldOff::self, Item::Active, OnChanged_HoldOff, Choice::AfterDraw
)



static void OnChanged_Duration()
{
    Osci::LoadHoldfOff();
}

DEF_GOVERNOR( gDuration,                                                                                                                           //--- ����� - ��������� - ������������ ---
    "������������",
    "����� ������������ ���������� ����� ���������� �������������",
    set.trig.holdOff, 10, 32000, &PageTrig::PageHoldOff::self, Item::Active, OnChanged_Duration
)

/// \todo gDuration ������ ������������ ����������� �������� �������� ��������, ��� 32000



DEF_PAGE_2( pHoldOff, // -V641 // -V1027                                                                                                                          //--- ����� - ��������� ---
    "���������",
    "",
    &cHoldOff,
    &gDuration,
    PageName::Trig_HoldOff,
    &PageTrig::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTrig::PageHoldOff::self = (const Page *)&pHoldOff;
