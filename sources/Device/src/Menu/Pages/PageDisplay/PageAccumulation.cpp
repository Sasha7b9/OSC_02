#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Osci/Display/Osci_Display.h"
#include "Settings/Settings.h"


using namespace Osci::Settings;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cEnable,                                                                                                                                 //--- ������� - ���������� - ����� ---
    "�����",
    "",
    DISABLE_RU,
    ENABLE_RU,
    ACCUMULATION, &PageDisplay::PageAccumulation::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_9( cNumber,                                                                                                                            //--- ������� - ���������� - ���������� ---
    "����������"
    ,
    "����� ������������ ���������� ��������� �������� �� ������. ���� � ��������� \"�����\" ������� \"�������������\", ����� ��������� ������ �������� ������ \"��������\"."
    "\"�������������\" - ������ ��������� ������� �� ������� �� ��� ���, ���� �� ����� ������ ������ \"��������\".",
    DISABLE_RU,
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "�������������",
    ENUM_ACCUM, &PageDisplay::PageAccumulation::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Accum_Clear()
{
    return true;
}

void PageDisplay::PageAccumulation::OnPress_Accumulation_Clear()
{
    Osci::Display::SetFlagRedraw();
}

DEF_BUTTON( bClear,                                                                                                                                 //--- ������� - ���������� - �������� ---
    "��������",
    "������� ����� �� ����������� ��������.",
    &PageDisplay::PageAccumulation::self, IsActive_Accum_Clear, PageDisplay::PageAccumulation::OnPress_Accumulation_Clear
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Accum()
{
    return !Osci::InModeRandomizer();
}

DEF_PAGE_3( pAccum, // -V641 // -V1027                                                                                                                         //--- ������� - ���������� ---
    "������",
    "��������� ������ ����������� ��������� �������� �� ������",
    &cEnable,    ///< ������� - ���������� - ����������
    &cNumber,    ///< ������� - ���������� - �����
    &bClear,     ///< ������� - ���������� - ��������
    PageName::Display_Accumulation, &PageDisplay::self, IsActive_Accum, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDisplay::PageAccumulation::self = (const Page *)&pAccum;
