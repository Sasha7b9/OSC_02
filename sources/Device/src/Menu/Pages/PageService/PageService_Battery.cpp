#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"
#endif

extern const PageBase pageBattery;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageService::PageBattery::pointer = &pageBattery;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                  //--- ������ - ������� - ���������� ---  
    "����������", "Show",
    "��������/��������� ����� ����������� � ���������� ������� �� �����",
    "Enables / disables the display of battery voltage information on the screen",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    BAT_SHOW_ON_DISPLAY, pageBattery, FuncActive, Choice::EmptyChange, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_1( pageBattery, // -V641 // -V1027                                                                                                                        //--- ������ - ������� ---
    "�������", "BATTERY",
    "������� ������ � ��������������",
    "Battery functions",
    &cShow,                     ///< ������ - ������� - ����������
    Page::Name::Service_Battery, PageService::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
