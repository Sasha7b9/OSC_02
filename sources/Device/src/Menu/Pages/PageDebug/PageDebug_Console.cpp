#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display/Colors.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDebug.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageConsole;
extern const PageBase pppConsole_Registers;

const PageBase *PageDebug::PageConsole::pointer = &pageConsole;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_GOVERNOR(gConsole_NumStrings,                                                                                                                   //--- ������� - ������� - ����� ����� ---
    "����� �����", "Number strings",
    "",
    "",
    CONSOLE_NUM_STRINGS, 0, 33, pageConsole, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_SizeFont,                                                                                                                   //--- ������� - ������� - ������ ������ ---
    "������ ������", "Size font",
    "",
    "",
    "5", "5",
    "8", "8",
    set.dbg_sizeFont, pageConsole, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_ModeStop,                                                                                                                   //--- ������� - ������� - ���. �������� ---
    "���. ��������", "Mode stop",
    "������������� ����������� ������������ ������ � ������� ���� ������� �� ������ ����/����",
    "It provides the ability to pause the output to the console by pressing the ����/���� button",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    MODE_PAUSE_CONSOLE, pageConsole, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_ShowAll,                                                                                              //--- ������� - ������� - �������� - ���������� ��� ---
    "���������� ���", "Show all",
    "���������� ��� ��������, ���������� � ��������",
    "To show all values transferred in registers",
    "���", "No",
    "��", "Yes",
    DBG_SHOW_ALL, pppConsole_Registers, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Console_Registers()
{
    return DBG_SHOW_ALL;
}

DEF_CHOICE_2(cConsole_Registers_RD_FL,                                                                                                         //--- ������� - ������� - �������� - RD_FL ---
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    DBG_SHOW_FLAG, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_RShiftA,                                                                                                    //--- ������� - ������� - �������� - U ��. 1� ---
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRShift[Chan::A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_RShiftB,                                                                                                    //--- ������� - ������� - �������� - U ��. 2� ---
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRShift[Chan::B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_TrigLev,                                                                                                    //--- ������� - ������� - �������� - U �����. ---
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTrigLev, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_RangeA,                                                                                                  //--- ������� - ������� - �������� - �����/��� 1 ---
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRange[Chan::A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_RangeB,                                                                                                  //--- ������� - ������� - �������� - �����/��� 2 ---
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRange[Chan::B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_TrigParam,                                                                                             //--- ������� - ������� - �������� - �����. �����. ---
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTrigParam, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_ChanParamA,                                                                                            //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showChanParam[Chan::A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_ChanParamB,                                                                                            //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showChanParam[Chan::B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_TBase,                                                                                                     //--- ������� - ������� - �������� - �����/��� ---
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTBase, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_TShift,                                                                                                        //--- ������� - ������� - �������� - � ��. ---
    "� ��.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTShift, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_12(pppConsole_Registers,                                                                                                                      //--- ������� - ������� - �������� ---
    "��������", "REGISTERS",
    "",
    "",
    &cConsole_Registers_ShowAll,     // ������� - ������� - �������� - ���������� ���
    &cConsole_Registers_RD_FL,       // ������� - ������� - �������� - RD_FL
    &cConsole_Registers_RShiftA,     // ������� - ������� - �������� - U ��. 1�
    &cConsole_Registers_RShiftB,     // ������� - ������� - �������� - U ��. 2�
    &cConsole_Registers_TrigLev,     // ������� - ������� - �������� - U �����
    &cConsole_Registers_RangeA,      // ������� - ������� - �������� - �����/��� 1
    &cConsole_Registers_RangeB,      // ������� - ������� - �������� - �����/��� 2
    &cConsole_Registers_TrigParam,   // ������� - ������� - �������� - �����. �����.
    &cConsole_Registers_ChanParamA,  // ������� - ������� - �������� - �����. ���. 1
    &cConsole_Registers_ChanParamB,  // ������� - ������� - �������� - �����. ���. 2
    &cConsole_Registers_TBase,       // ������� - ������� - �������� - �����/���
    &cConsole_Registers_TShift,      // ������� - ������� - �������� - � ��.
    Page::Name::Debug_Console_Registers, &pageConsole, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Console_SizeSettings(int x, int y)
{
    char buffer[30];
    sprintf(buffer, "����.�����. %d", sizeof(Settings));
    Painter::DrawText(x + 6, y + 13, buffer, Color::BACK);
}

DEF_BUTTON(bConsole_SizeSettings,                                                                                                               //--- ������� - ������� - ������ �������� ---
    "", "",
    "���������� ������� ������ ��������� ��� ���������� ��������",
    "Displays the current size of the structure to save settings",
    pageConsole, FuncActive, FuncPress, Draw_Console_SizeSettings
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(pageConsole,                                                                                                                                             //--- ������� - ������� ---
    "�������", "CONSOLE",
    "",
    "",
    &gConsole_NumStrings,       ///< ������� - ������� - ����� �����
    &cConsole_SizeFont,         ///< ������� - ������� - ������ ������
    &cConsole_ModeStop,         ///< ������� - ������� - ���. ��������
    &pppConsole_Registers,      ///< ������� - ������� - ��������
    &bConsole_SizeSettings,     ///< ������� - ������� - ������ ��������
    Page::Name::Debug_Console, PageDebug::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

