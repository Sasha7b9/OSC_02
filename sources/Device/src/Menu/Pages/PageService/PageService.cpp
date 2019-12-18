#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


void PageService::OnPress_ResetSettings()
{
    Settings::Load(true);
    if (Device::InModeOsci())
    {
        Osci::Start();
    }
}

DEF_BUTTON( bResetSettings,                                                                                                                                 //--- ������ - ����� �������� ---
    "����� �����",
    "����� �������� �� ��������� �� ���������",
    &PageService::self, Item::Active, PageService::OnPress_ResetSettings
)


static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON( bAutoSearch,                                                                                                                                     //--- ������ - ����� ������� ---
    "����� ����",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    &PageService::self, Item::Active, OnPress_AutoSearch
)


DEF_CHOICE_4( cSoundVolume,
    "���������",
    "��������� ������ ���������",
    "����",
    "���",
    "�����",
    "����",
    set.serv.soundVolume, &PageService::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_7( pService,                                                                                                                                                        //--- ������ ---
    "������",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    &bResetSettings,
    &bAutoSearch,
    PageService::Calibrate::self,
    &cSoundVolume,
    PageRTC::self,
    PageService::Information::self,
    PageDebug::self,
    PageName::Service, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageService::self = static_cast<const Page *>(&pService);
