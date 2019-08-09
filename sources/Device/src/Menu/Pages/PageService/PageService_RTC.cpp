#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


extern const PageBase pageRTC;
extern const PageBase pageRTC_Set;
extern const PageBase pageRTC_Correction;

const PageBase * const PageService::PageRTC::pointer = &pageRTC;
const PageBase * const PageService::PageRTC::PageCorrect::pointer = &pageRTC_Correction;
const PageBase * const PageService::PageRTC::PageSet::pointer = &pageRTC_Set;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageRTC, // -V641 // -V1027                                                                                                                              //--- ������ - ����� ---
    "�����",
    "��������� � ��������� �������",
    PageService::PageRTC::PageSet::pointer,         ///< ������ - ����� - �����
    PageService::PageRTC::PageCorrect::pointer,     ///< C����� - ����� - ���������
    Page::Name::Service_RTC, PageService::pointer, 0, 0, 0, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
DEF_TIME(tRTC_Time,                                                                                                                                         //--- ������ - ����� - ����� ---
    "�����"
    ,
    "��������� �������� �������.\n������� ������:\n"
    "������ �� ������� ���� \"�����\". ��������� ���� ��������� �������� �������. ��������� ��������� ������ �� �������� ����������, �������������� "
    "�������� ���������� \"�����\", �������� ����, ������, �������, ���, �����, ��� �����. ���������� ������� ������������ �������� ��������. "
    "��������� ����� ��������� ���������� ����������� ��������. ����� �������� ����� \"���������\", ������ � ���������� ����� 0.5 ��� ������ �� "
    "������ ����������. ���� ��������� �������� ������� ��������� � ����������� ������ �������� �������. ������� ���������� ��������� ������ �� "
    "����� ������ �������� ������� � �������� ���� ��������� �������� ���    ���� ��� ���������� ������ �������� �������"
    ,
    pageRTC_Set, 0, dServicetime, hours, minutes, secondes, month, day, year
)

DEF_PAGE_1( pageRTC_Set, //-V641 //-V1027
    "���������",
    "��������� �������� �������",
    &tRTC_Time,
    Page::Name::Service_RTC_Set, PageService::PageRTC::pointer, 0, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Time_Correction()
{
}

DEF_GOVERNOR(tRTC_Correction,                                                                                                                           //--- ������ - ����� - ��������� ---
    "���������",
    "��������� ��������������� ������������ ��� ����������� ���� �������",
    NRST_CORRECTION_TIME, -63, 63, pageRTC_Correction, 0, OnChanged_Time_Correction, 0
)

DEF_PAGE_1( pageRTC_Correction, //-V641 //-V1027
    "���������",
    "��������� �������� ���� �����",
    &tRTC_Correction,
    Page::Name::Service_RTC_Correct, PageService::PageRTC::pointer, 0, 0, 0, 0
)
