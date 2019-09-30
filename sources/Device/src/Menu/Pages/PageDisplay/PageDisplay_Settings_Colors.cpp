#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"



static void OnPress_ResetColors()
{
}

DEF_BUTTON( bReset,                                                                                                                          //--- ������� - ��������� - ����� - �������� ---
    "��������",
    "����� ���� ������ �� �������� �� ���������",
    &PageDisplay::PageSettings::PageColors::self, Item::Active, OnPress_ResetColors
)


ColorType PageDisplay::PageSettings::PageColors::colorTypeA = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::CHAN[Chan::A])

DEF_GOVERNOR_COLOR( gcChannelA,                                                                                                               //--- ������� - ��������� - ����� - ����� 1 ---
    "����� 1",
    "����� ����� ������ 1",
    PageDisplay::PageSettings::PageColors::colorTypeA, &PageDisplay::PageSettings::PageColors::self
)


ColorType PageDisplay::PageSettings::PageColors::colorTypeB = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::CHAN[Chan::B])

DEF_GOVERNOR_COLOR( gcChannelB,                                                                                                               //--- ������� - ��������� - ����� - ����� 2 ---
    "����� 2",
    "����� ����� ������ 1",
    PageDisplay::PageSettings::PageColors::colorTypeB, &PageDisplay::PageSettings::PageColors::self
)


ColorType PageDisplay::PageSettings::PageColors::colorTypeGrid = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID)

DEF_GOVERNOR_COLOR( gcGrid,                                                                                                                     //--- ������� - ��������� - ����� - ����� ---
    "�����",
    "������������� ���� �����",
    PageDisplay::PageSettings::PageColors::colorTypeGrid, &PageDisplay::PageSettings::PageColors::self
)


DEF_PAGE_4( pColors, // -V641 // -V1027                                                                                                                 //--- ������� - ��������� - ����� ---
    "�����",
    "����� ������ �������",
    &bReset,              ///< ������� - ��������� - ����� - ��������
    &gcChannelA,          ///< ������� - ��������� - ����� - ����� 1
    &gcChannelB,          ///< ������� - ��������� - ����� - ����� 2
    &gcGrid,              ///< ������� - ��������� - ����� - �����
    PageName::Display_Settings_Colors, &PageDisplay::PageSettings::self, Item::Active, Page::Changed, Page::BeforeDraw, Page::Handler
)

const Page * const PageDisplay::PageSettings::PageColors::self = (const Page *)&pColors;
