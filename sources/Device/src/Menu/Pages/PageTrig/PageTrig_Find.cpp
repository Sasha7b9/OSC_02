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
    TrigLevel().Find();
}

DEF_BUTTON( bSearch,                                                                                                                                          //--- ����� - ����� - ����� ---
    "�����",
    "���������� ����� ������ �������������.",
    &PageTrig::PageFind::self, Item::Active, OnPress_Search
)



DEF_PAGE_2( pFind,                                                                                                                                                    //--- ����� - ����� ---
    "�����",
    "���������� �������������� ������� ������ �������������.",
    &cMode,
    &bSearch,
    PageName::Trig_Search,
    &PageTrig::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTrig::PageFind::self = static_cast<const Page *>(&pFind);
