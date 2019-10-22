#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


DEF_CHOICE_16( gGateMax,                                                                                                                           //--- ������� - ����-��� - ������ ���� ---
    "������ ����",
    "������������� ����������� ���������� �� ��������� ����� ��� �������������",
    "0",
    "10",
    "20",
    "30",
    "40",
    "50",
    "60",
    "70",
    "80",
    "90",
    "100",
    "110",
    "120",
    "130",
    "140",
    "150",
    set.dbg.enum_gate_max, &PageDebug::PageRand::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



DEF_CHOICE_16( gGateMin,                                                                                                                            //--- ������� - ����-��� - ������ ��� ---
    "������ ���",
    "������������� ����������� ���������� �� �������� ����� ��� �������������",
    "0",
    "10",
    "20",
    "30",
    "40",
    "50",
    "60",
    "70",
    "80",
    "90",
    "100",
    "110",
    "120",
    "130",
    "140",
    "150",
    set.dbg.enum_gate_min, &PageDebug::PageRand::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



DEF_PAGE_2( pRand,                                                                                                                                               //--- ������� - ����-��� ---
    "����-���",
    "",
    &gGateMin,
    &gGateMax,
    PageName::Debug_Rand, &PageDebug::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDebug::PageRand::self = static_cast<const Page *>(&pRand);
