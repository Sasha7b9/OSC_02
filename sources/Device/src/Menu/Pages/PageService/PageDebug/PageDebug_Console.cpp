#include "defines.h"
#include "Display/Colors.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cShow,                                                                                                                                 //--- ������� - ������� - ���������� ---
    "����������",
    "",
    "���",
    "��",
    set.dbg.showConsole, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_GOVERNOR( gNumStrings,                                                                                                                          //--- ������� - ������� - ����� ����� ---
    "����� �����",
    "",
    set.dbg.numStrings, 0, 33, &PageDebug::PageConsole::self, Item::Active, Console::OnChanged_MaxStringsInConsole
)


DEF_CHOICE_2( cShowAll,                                                                                                               //--- ������� - ������� - �������� - ���������� ��� ---
    "���������� ���",
    "���������� ��� ��������, ���������� � ��������",
    "���",
    "��",
    set.dbg.showAll, &PageDebug::PageRegisters::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Registers()
{
    return set.dbg.showAll;
}

DEF_CHOICE_2( cRD_FL,                                                                                                                          //--- ������� - ������� - �������� - RD_FL ---
    "RD_FL",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showFlag, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRShiftA,                                                                                                                     //--- ������� - ������� - �������� - U ��. 1� ---
    "U ��. 1�",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRShift[ChanA], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRShiftB,                                                                                                                     //--- ������� - ������� - �������� - U ��. 2� ---
    "U ��. 2�",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRShift[ChanB], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTrigLev,                                                                                                                     //--- ������� - ������� - �������� - U �����. ---
    "U �����.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showTrigLev, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRangeA,                                                                                                                   //--- ������� - ������� - �������� - �����/��� 1 ---
    "�����/��� 1",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRange[ChanA], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRangeB,                                                                                                                   //--- ������� - ������� - �������� - �����/��� 2 ---
    "�����/��� 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRange[ChanB], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTrigParam,                                                                                                              //--- ������� - ������� - �������� - �����. �����. ---
    "�����. �����.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showTrigParam, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanParamA,                                                                                                             //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 1",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showChanParam[ChanA], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanParamB,                                                                                                             //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showChanParam[ChanB], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTBase,                                                                                                                      //--- ������� - ������� - �������� - �����/��� ---
    "�����/���",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showTBase, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTShift,                                                                                                                         //--- ������� - ������� - �������� - � ��. ---
    "� ��.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showTShift, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_12( pRegisters,                                                                                                                               //--- ������� - ������� - �������� ---
    "��������",
    "",
    &cShowAll,
    &cRD_FL,
    &cRShiftA,
    &cRShiftB,
    &cTrigLev,
    &cRangeA,
    &cRangeB,
    &cTrigParam,
    &cChanParamA,
    &cChanParamB,
    &cTBase,
    &cTShift,
    PageName::Debug_Console_Registers, &PageDebug::PageConsole::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageRegisters::self = static_cast<const Page *>(&pRegisters);


DEF_CHOICE_2( cShowGate,
    "������",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRandGate, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

DEF_CHOICE_2( cShowPredPost,
    "����������",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRandPredPost, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_4( pConsole,                                                                                                                                             //--- ������� - ������� ---
    "�������",
    "��������� ������������ � ����������� ���������� �������",
    &cShow,
    &gNumStrings,
    &cShowGate,
    &cShowPredPost,
    PageName::Debug_Console, &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageConsole::self = static_cast<const Page *>(&pConsole);
