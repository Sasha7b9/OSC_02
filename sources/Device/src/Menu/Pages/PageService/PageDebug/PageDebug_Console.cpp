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
    set.dbg.numStrings, 0, 33, &PageDebug::PageConsole::self, 0, Console::OnChanged_MaxStringsInConsole
)


//_DEF_CHOICE_2( cSizeFont,                                                                                                                        //--- ������� - ������� - ������ ������ ---
//    "������ ������",
//    "",
//    "5",
//    "8",
//    set.dbg_sizeFont, &PageDebug::PageConsole::self, 0, 0, 0
//)


//_DEF_CHOICE_2( cModeStop,                                                                                                                        //--- ������� - ������� - ���. �������� ---
//    "���. ��������",
//    "������������� ����������� ������������ ������ � ������� ���� ������� �� ������ ����/����",
//    DISABLE_RU,
//    ENABLE_RU,
//    MODE_PAUSE_CONSOLE, &PageDebug::PageConsole::self, 0, 0, 0
//)


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
    set.dbg.showRShift[Chan::A], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRShiftB,                                                                                                                     //--- ������� - ������� - �������� - U ��. 2� ---
    "U ��. 2�",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRShift[Chan::B], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
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
    set.dbg.showRange[Chan::A], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRangeB,                                                                                                                   //--- ������� - ������� - �������� - �����/��� 2 ---
    "�����/��� 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRange[Chan::B], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
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
    set.dbg.showChanParam[Chan::A], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanParamB,                                                                                                             //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showChanParam[Chan::B], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
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


DEF_PAGE_12( pRegisters, // -V641                                                                                                                      //--- ������� - ������� - �������� ---
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
    PageName::Debug_Console_Registers, &PageDebug::PageConsole::self, Item::Active, Page::Changed, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageRegisters::self = (const Page *)&pRegisters;


//static void Draw_Console_SizeSettings(int x, int y)
//{
//    String("����.�����. %d", sizeof(Settings)).Draw(x + 6, y + 13, Color::BACK);
//}
//
//_DEF_BUTTON( bSizeSettings,                                                                                                                      //--- ������� - ������� - ������ �������� ---
//    "",
//    "���������� ������� ������ ��������� ��� ���������� ��������",
//    &PageDebug::PageConsole::self, 0, 0, Draw_Console_SizeSettings
//)


DEF_PAGE_2( pConsole, // -V641 // -V1027                                                                                                                          //--- ������� - ������� ---
    "�������",
    "��������� ������������ � ����������� ���������� �������",
    &cShow,
    &gNumStrings,
//    &cSizeFont,
//    &cModeStop,
//    &pageRegisters,
//    &bSizeSettings,
    PageName::Debug_Console, &PageDebug::self, Item::Active, Page::Changed, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageConsole::self = (const Page *)&pConsole;
