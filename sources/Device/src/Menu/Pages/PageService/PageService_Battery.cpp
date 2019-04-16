#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"

#include "Settings/Settings.h"


extern const PageBase pageBattery;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageService::PageBattery::pointer = &pageBattery;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                  //--- ������ - ������� - ���������� ---  
    "����������",
    "��������/��������� ����� ����������� � ���������� ������� �� �����",
    DISABLE_RU,
    ENABLE_RU,
    BAT_SHOW_ON_DISPLAY, pageBattery, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_1( pageBattery, // -V641 // -V1027                                                                                                                        //--- ������ - ������� ---
    "�������",
    "������� ������ � ��������������",
    &cShow,                     ///< ������ - ������� - ����������
    Page::Name::Service_Battery, PageService::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
