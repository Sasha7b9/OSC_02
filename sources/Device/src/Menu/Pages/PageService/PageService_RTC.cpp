#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


extern const Page pageRTC;
extern const Page pageRTC_Set;
extern const Page pageRTC_Correction;

const Page * const PageRTC::self = (const Page *)&pageRTC;
const Page * const PageRTC::PageCorrect::self = (const Page *)&pageRTC_Correction;
const Page * const PageRTC::PageSet::self = (const Page *)&pageRTC_Set;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageRTC, // -V641 // -V1027                                                                                                                              //--- ������ - ����� ---
    "�����",
    "��������� � ��������� �������",
    PageRTC::PageSet::self,         ///< ������ - ����� - �����
    PageRTC::PageCorrect::self,     ///< C����� - ����� - ���������
    PageName::Service_RTC,
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
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetRight,
    "������",
    "��������� �������",
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetUp,
    "������",
    "���������",
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetDown,
    "������",
    "���������",
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetPick,
    "�������",
    "����� ������������� ��������",
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_PAGE_5( pageRTC_Set, //-V641 //-V1027
    "���������",
    "��������� �������� �������",
    &bSetLeft,
    &bSetRight,
    &bSetUp,
    &bSetDown,
    &bSetPick,
    PageName::Service_RTC_Set,
    &PageRTC::self, 0, 0, 0, 0
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
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrRight,
    "������",
    "��������� �������",
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrUp,
    "������",
    "���������",
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrDown,
    "������",
    "���������",
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrPick,
    "�����",
    "������������ ������������ �������",
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_PAGE_5( pageRTC_Correction, //-V641 //-V1027
    "���������",
    "��������� �������� ���� �����",
    &bCorrLeft,
    &bCorrRight,
    &bCorrUp,
    &bCorrDown,
    &bCorrPick,
    PageName::Service_RTC_Correct,
    &PageRTC::self, 0, 0, 0, 0
)
