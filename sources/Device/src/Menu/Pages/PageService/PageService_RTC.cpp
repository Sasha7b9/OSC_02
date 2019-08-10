#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


extern const PageBase pageRTC;
extern const PageBase pageRTC_Set;
extern const PageBase pageRTC_Correction;

const Page * const PageService::PageRTC::self = (const Page *)&pageRTC;
const Page * const PageService::PageRTC::PageCorrect::self = (const Page *)&pageRTC_Correction;
const Page * const PageService::PageRTC::PageSet::self = (const Page *)&pageRTC_Set;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageRTC, // -V641 // -V1027                                                                                                                              //--- ������ - ����� ---
    "�����",
    "��������� � ��������� �������",
    PageService::PageRTC::PageSet::self,         ///< ������ - ����� - �����
    PageService::PageRTC::PageCorrect::self,     ///< C����� - ����� - ���������
    Page::Name::Service_RTC,
    &PageService::self, 0, 0, 0, 0
)


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
//    pageRTC_Set, 0, dServicetime, hours, minutes, secondes, month, day, year
//)

DEF_BUTTON( bSetLeft,
    "�����",
    "���������� �������",
    &PageService::PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetRight,
    "������",
    "��������� �������",
    &PageService::PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetUp,
    "������",
    "���������",
    &PageService::PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetDown,
    "������",
    "���������",
    &PageService::PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetPick,
    "�������",
    "����� ������������� ��������",
    &PageService::PageRTC::PageSet::self, 0, 0, 0
)

DEF_PAGE_5( pageRTC_Set, //-V641 //-V1027
    "���������",
    "��������� �������� �������",
    &bSetLeft,
    &bSetRight,
    &bSetUp,
    &bSetDown,
    &bSetPick,
    Page::Name::Service_RTC_Set,
    &PageService::PageRTC::self, 0, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static void OnChanged_Time_Correction()
//{
//}
//
//DEF_GOVERNOR(tRTC_Correction,                                                                                                                           //--- ������ - ����� - ��������� ---
//    "���������",
//    "��������� ��������������� ������������ ��� ����������� ���� �������",
//    NRST_CORRECTION_TIME, -63, 63, pageRTC_Correction, 0, OnChanged_Time_Correction, 0
//)

DEF_BUTTON( bCorrLeft,
    "�����",
    "���������� �������",
    &PageService::PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrRight,
    "������",
    "��������� �������",
    &PageService::PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrUp,
    "������",
    "���������",
    &PageService::PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrDown,
    "������",
    "���������",
    &PageService::PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrPick,
    "�����",
    "������������ ������������ �������",
    &PageService::PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_PAGE_5( pageRTC_Correction, //-V641 //-V1027
    "���������",
    "��������� �������� ���� �����",
    &bCorrLeft,
    &bCorrRight,
    &bCorrUp,
    &bCorrDown,
    &bCorrPick,
    Page::Name::Service_RTC_Correct,
    &PageService::PageRTC::self, 0, 0, 0, 0
)
