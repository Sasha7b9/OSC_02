#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cShow,                                                                                                                                  //--- ������ - ������� - ���������� ---  
    "����������",
    "��������/��������� ����� ����������� � ���������� ������� �� �����",
    DISABLE_RU,
    ENABLE_RU,
    set.serv.showInfoVoltage, &PageService::PageBattery::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_1( pBattery, // -V641 // -V1027                                                                                                                           //--- ������ - ������� ---
    "�������",
    "������� ������ � ��������������",
    &cShow,                     ///< ������ - ������� - ����������
    PageName::Service_Battery, &PageService::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageService::PageBattery::self = (const Page *)&pBattery;
