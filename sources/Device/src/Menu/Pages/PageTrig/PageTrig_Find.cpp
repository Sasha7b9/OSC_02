#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cMode,                                                                                                                                          //--- ����� - ����� - ����� ---
    "����",
    "����� ������ ��������������� ������ �������������:\n"
    "1. \"������\" - ����� ������������ �� ������� ������ \"�����\" ��� �� ��������� � ������� 0.5� ������ �����, ���� ����������� "
    "\"������\x99��� ���� �����\x99�����������\".\n"
    "2. \"��������������\" - ����� ������������ �������������.",
    "������",
    "��������������",
    set.trig.modeFind, &PageTrig::PageFind::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Search()
{
    Trig::Level::Find();
}

DEF_BUTTON( bSearch,                                                                                                                                          //--- ����� - ����� - ����� ---
    "�����",
    "���������� ����� ������ �������������.",
    &PageTrig::PageFind::self, Item::Active, OnPress_Search
)



DEF_PAGE_2( pFind, // -V641 // -V1027                                                                                                                                 //--- ����� - ����� ---
    "�����",
    "���������� �������������� ������� ������ �������������.",
    &cMode,                         ///< ����� - ����� - �����
    &bSearch,                       ///< ����� - ����� - �����
    PageName::Trig_Search,
    &PageTrig::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageTrig::PageFind::self = (const Page *)&pFind;
