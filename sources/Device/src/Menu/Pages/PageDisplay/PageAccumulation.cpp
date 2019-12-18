#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"


ModeAccumulation::E &ModeAccumulation::Ref()
{
    return set.disp.modeAccumulation;
}


DEF_CHOICE_2( cEnable,                                                                                                                                 //--- ������� - ���������� - ����� ---
    "�����",
    "",
    DISABLE_RU,
    ENABLE_RU,
    ModeAccumulation::Ref(), &PageDisplay::Accumulation::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



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
    ENumAccum::Ref(), &PageDisplay::Accumulation::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



static bool IsActive_Accum_Clear()
{
    return true;
}

void PageDisplay::Accumulation::OnPress_Accumulation_Clear()
{
    DisplayOsci::SetFlagRedraw();
}

DEF_BUTTON( bClear,                                                                                                                                 //--- ������� - ���������� - �������� ---
    "��������",
    "������� ����� �� ����������� ��������.",
    &PageDisplay::Accumulation::self, IsActive_Accum_Clear, PageDisplay::Accumulation::OnPress_Accumulation_Clear
)



static bool IsActive_Accum()
{
    return !Osci::InModeRandomizer();
}

DEF_PAGE_3( pAccum,                                                                                                                           //--- ������� - ���������� ---
    "������",
    "��������� ������ ����������� ��������� �������� �� ������",
    &cEnable,
    &cNumber,
    &bClear,
    PageName::Display_Accumulation, &PageDisplay::self, IsActive_Accum, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::Accumulation::self = static_cast<const Page *>(&pAccum);
