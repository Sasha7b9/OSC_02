#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pRTC, // -V641 // -V1027                                                                                                                                 //--- ������ - ����� ---
    "�����",
    "��������� � ��������� �������",
    PageRTC::PageSet::self,         ///< ������ - ����� - �����
    PageRTC::PageCorrect::self,     ///< C����� - ����� - ���������
    PageName::Service_RTC,
    &PageService::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::self = (const Page *)&pRTC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static int8 dServicetime = 0;
//static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
//DEF_TIME(tRTC_Time,                                                                                                                                         //--- ������ - ����� - ����� ---
//    "�����"
//    ,
//    "��������� �������� �������.\n������� ������:\n"
//    "������ �� ������� ���� \"�����\". ��������� ���� ��������� �������� �������. ��������� ��������� ������ �� �������� ����������, �������������� "
//    "�������� ���������� \"�����\", �������� ����, ������, �������, ���, �����, ��� �����. ���������� ������� ������������ �������� ��������. "
//    "��������� ����� ��������� ���������� ����������� ��������. ����� �������� ����� \"���������\", ������ � ���������� ����� 0.5 ��� ������ �� "
//    "������ ����������. ���� ��������� �������� ������� ��������� � ����������� ������ �������� �������. ������� ���������� ��������� ������ �� "
//    "����� ������ �������� ������� � �������� ���� ��������� �������� ���    ���� ��� ���������� ������ �������� �������"
//    ,
//    pSet, 0, dServicetime, hours, minutes, secondes, month, day, year
//)

DEF_BUTTON( bSet_Left,
    "�����",
    "���������� �������",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_BUTTON( bSet_Right,
    "������",
    "��������� �������",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_BUTTON( bSet_Up,
    "������",
    "���������",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_BUTTON( bSet_Down,
    "������",
    "���������",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_BUTTON( bSet_Pick,
    "�������",
    "����� ������������� ��������",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_PAGE_5( pSet, //-V641 //-V1027
    "���������",
    "��������� �������� �������",
    &bSet_Left,
    &bSet_Right,
    &bSet_Up,
    &bSet_Down,
    &bSet_Pick,
    PageName::Service_RTC_Set,
    &PageRTC::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::PageSet::self = (const Page *)&pSet;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static void OnChanged_Time_Correction()
//{
//}
//
//_DEF_GOVERNOR(tRTC_Correction,                                                                                                                           //--- ������ - ����� - ��������� ---
//    "���������",
//    "��������� ��������������� ������������ ��� ����������� ���� �������",
//    NRST_CORRECTION_TIME, -63, 63, pCorrection, 0, OnChanged_Time_Correction, 0
//)

DEF_BUTTON( bCorrection_Left,
    "�����",
    "���������� �������",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

DEF_BUTTON( bCorrection_Right,
    "������",
    "��������� �������",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

DEF_BUTTON( bCorrection_Up,
    "������",
    "���������",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

DEF_BUTTON( bCorrection_Down,
    "������",
    "���������",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

DEF_BUTTON( bCorrection_Pick,
    "�����",
    "������������ ������������ �������",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pCorrection, //-V641 //-V1027
    "���������",
    "��������� �������� ���� �����",
    &bCorrection_Left,
    &bCorrection_Right,
    &bCorrection_Up,
    &bCorrection_Down,
    &bCorrection_Pick,
    PageName::Service_RTC_Correct,
    &PageRTC::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::PageCorrect::self = (const Page *)&pCorrection;
