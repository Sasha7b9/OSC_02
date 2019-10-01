#include "defines.h"
#include "Menu/pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"



static void OnChanged_Brightness()
{
}

DEF_GOVERNOR( gBrightness,                                                                                                                            //--- ������� - ��������� - ������� ---
    "�������",
    "��������� ������� �������� �������",
    set.disp.brightness, 0, 100, &PageDisplay::PageSettings::self, Item::Active, OnChanged_Brightness
)


static void OnChanged_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6( cAutoHide,                                                                                                                             //--- ������� - ��������� - �������� ---
    "��������",
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� � ������",
    "�������",
    "����� 5 ���",
    "����� 10 ���",
    "����� 15 ���",
    "����� 30 ���",
    "����� 60 ���",
    set.disp.menuAutoHide, &PageDisplay::PageSettings::self, Item::Active, OnChanged_AutoHide, Choice::AfterDraw
)


DEF_PAGE_3( pSettings, // -V641 // -V1027                                                                                                                       //--- ������� - ��������� ---
    "���������",
    "�������������� ��������� �������",
    PageDisplay::PageSettings::PageColors::self,
    &gBrightness,
    &cAutoHide,
    PageName::Display_Settings, &PageDisplay::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::PageSettings::self = (const Page *)&pSettings;
