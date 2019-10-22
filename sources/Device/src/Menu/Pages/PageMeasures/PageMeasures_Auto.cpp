#include "defines.h"
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageMeasures.h"



DEF_CHOICE_2( cIsShow,                                                                                                                             //--- ��������� - ������� - ���������� ---
    "����������",
    "�������� ��� �� �������� ��������� �� �����",
    "���",
    "��",
    set.meas.show, &PageAutoMeasures::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_NumberChannels()
{
    return set.meas.show;
}

DEF_CHOICE_7( cNumber,                                                                                                                             //--- ��������� - ������� - ���������� ---
    "����������"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
    ,
    "1",
    "2",
    "1x5",
    "2x5",
    "3x5",
    "6x1",
    "6x2",
    set.meas.number, &PageAutoMeasures::self, IsActive_NumberChannels, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_3( cChannels,                                                                                                                               //--- ��������� - ������� - ������ ---
    "������",
    "�� ����� ������� �������� ���������",
    "1",
    "2",
    "1 � 2",
    set.meas.source, &PageAutoMeasures::self, IsActive_NumberChannels, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_4( pAuto,                                                                                                                                                 //--- ��������� - ���� ---
    "�������",
    "������ � ���������� �������������� ���������",
    &cIsShow,
    &cNumber,
    &cChannels,
    PageAutoMeasures::PageTune::self,
    PageName::AutoMeasures, &PageMeasures::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageAutoMeasures::self = static_cast<const Page *>(&pAuto);
