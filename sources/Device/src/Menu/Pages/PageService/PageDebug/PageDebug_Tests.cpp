#include "defines.h"
#include "device.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Settings/Settings.h"


static void OnPress_Run()
{
    set.dbg.runTest = true;
    set.Save();
    Device::Reset();
}


DEF_BUTTON( bRun,                                                                                                                                       //--- ������� - ����� - ��������� ---
    "���������",
    "���������� ��������� ������",
    &PageDebug::PageTests::self, Button::Active, OnPress_Run
)


DEF_PAGE_1( pTests,                                                                                                                                                 //--- ������� - ����� ---
    "�����",
    "���������� ������",
    &bRun,
    PageName::Debug_Tests,
    &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


const Page *const PageDebug::PageTests::self = static_cast<const Page *>(&pTests);
