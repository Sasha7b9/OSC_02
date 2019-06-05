#include "defines.h"
#include "Display/Colors.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageConsole;
extern const PageBase pageRegisters;

const PageBase *PageService::PageDebug::PageConsole::pointer = &pageConsole;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                 //--- ������� - ������� - ���������� ---
    "����������",
    "",
    "���",
    "��",
    set.dbg_showConsole, pageConsole, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gNumStrings,                                                                                                                          //--- ������� - ������� - ����� ����� ---
    "����� �����",
    "",
    CONSOLE_NUM_STRINGS, 0, 33, pageConsole, FuncActive, Console::OnChangedMaxStringsInConsole, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cSizeFont,                                                                                                                          //--- ������� - ������� - ������ ������ ---
    "������ ������",
    "",
    "5",
    "8",
    set.dbg_sizeFont, pageConsole, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cModeStop,                                                                                                                          //--- ������� - ������� - ���. �������� ---
    "���. ��������",
    "������������� ����������� ������������ ������ � ������� ���� ������� �� ������ ����/����",
    DISABLE_RU,
    ENABLE_RU,
    MODE_PAUSE_CONSOLE, pageConsole, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_ShowAll,                                                                                                     //--- ������� - ������� - �������� - ���������� ��� ---
    "���������� ���",
    "���������� ��� ��������, ���������� � ��������",
    "���",
    "��",
    DBG_SHOW_ALL, pageRegisters, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Console_Registers()
{
    return DBG_SHOW_ALL;
}

DEF_CHOICE_2( cRegisters_RD_FL,                                                                                                                //--- ������� - ������� - �������� - RD_FL ---
    "RD_FL",
    "",
    DISABLE_RU,
    ENABLE_RU,
    DBG_SHOW_FLAG, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_RShiftA,                                                                                                           //--- ������� - ������� - �������� - U ��. 1� ---
    "U ��. 1�",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showRShift[Chan::A], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_RShiftB,                                                                                                           //--- ������� - ������� - �������� - U ��. 2� ---
    "U ��. 2�",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showRShift[Chan::B], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_TrigLev,                                                                                                           //--- ������� - ������� - �������� - U �����. ---
    "U �����.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showTrigLev, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_RangeA,                                                                                                         //--- ������� - ������� - �������� - �����/��� 1 ---
    "�����/��� 1",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showRange[Chan::A], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_RangeB,                                                                                                         //--- ������� - ������� - �������� - �����/��� 2 ---
    "�����/��� 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showRange[Chan::B], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_TrigParam,                                                                                                    //--- ������� - ������� - �������� - �����. �����. ---
    "�����. �����.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showTrigParam, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_ChanParamA,                                                                                                   //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 1",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showChanParam[Chan::A], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_ChanParamB,                                                                                                   //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showChanParam[Chan::B], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_TBase,                                                                                                            //--- ������� - ������� - �������� - �����/��� ---
    "�����/���",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showTBase, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_TShift,                                                                                                               //--- ������� - ������� - �������� - � ��. ---
    "� ��.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg_showTShift, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_12( pageRegisters, // -V641                                                                                                                   //--- ������� - ������� - �������� ---
    "��������",
    "",
    &cRegisters_ShowAll,        ///< ������� - ������� - �������� - ���������� ���
    &cRegisters_RD_FL,          ///< ������� - ������� - �������� - RD_FL
    &cRegisters_RShiftA,        ///< ������� - ������� - �������� - U ��. 1�
    &cRegisters_RShiftB,        ///< ������� - ������� - �������� - U ��. 2�
    &cRegisters_TrigLev,        ///< ������� - ������� - �������� - U �����
    &cRegisters_RangeA,         ///< ������� - ������� - �������� - �����/��� 1
    &cRegisters_RangeB,         ///< ������� - ������� - �������� - �����/��� 2
    &cRegisters_TrigParam,      ///< ������� - ������� - �������� - �����. �����.
    &cRegisters_ChanParamA,     ///< ������� - ������� - �������� - �����. ���. 1
    &cRegisters_ChanParamB,     ///< ������� - ������� - �������� - �����. ���. 2
    &cRegisters_TBase,          ///< ������� - ������� - �������� - �����/���
    &cRegisters_TShift,         ///< ������� - ������� - �������� - � ��.
    Page::Name::Debug_Console_Registers, &pageConsole, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Console_SizeSettings(int x, int y)
{
    String("����.�����. %d", sizeof(Settings)).Draw(x + 6, y + 13, Color::BACK);
}

DEF_BUTTON( bSizeSettings,                                                                                                                      //--- ������� - ������� - ������ �������� ---
    "",
    "���������� ������� ������ ��������� ��� ���������� ��������",
    pageConsole, FuncActive, FuncPress, Draw_Console_SizeSettings
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageConsole, // -V641 // -V1027                                                                                                                       //--- ������� - ������� ---
    "�������",
    "��������� ������������ � ����������� ���������� �������",
    &cShow,             ///< ������� - ������� - ����������
    &gNumStrings,       ///< ������� - ������� - ����� �����
//    &cSizeFont,         ///< ������� - ������� - ������ ������
//    &cModeStop,         ///< ������� - ������� - ���. ��������
//    &pageRegisters,     ///< ������� - ������� - ��������
//    &bSizeSettings,     ///< ������� - ������� - ������ ��������
    Page::Name::Debug_Console, PageService::PageDebug::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

