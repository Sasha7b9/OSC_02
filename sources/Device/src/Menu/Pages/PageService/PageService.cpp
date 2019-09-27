#include "defines.h"
#include "device.h"
#include "Display/Display_Primitives.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/FPGA_Settings.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageService.h"
#include "Keyboard/Keyboard.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include <cstdio>


using namespace Display::Primitives;
using namespace Osci::Settings;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ResetSettings()
{
    Settings::Load(true);
    if (Device::State::InModeOsci())
    {
        FPGA::OnPressStart();
    }
}

DEF_BUTTON( bResetSettings,                                                                                                                                 //--- ������ - ����� �������� ---
    "����� �����",
    "����� �������� �� ��������� �� ���������",
    &PageService::self, Item::Active, OnPress_ResetSettings
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON( bAutoSearch,                                                                                                                                     //--- ������ - ����� ������� ---
    "����� ����",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    &PageService::self, Item::Active, OnPress_AutoSearch
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4( cSoundVolume,
    "���������",
    "��������� ������ ���������",
    "����",
    "���",
    "�����",
    "����",
    SOUND_VOLUME,
    &PageService::self, Item::Active, Choice::Changed, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_7( pService, // -V641 // -V1027                                                                                                                                     //--- ������ ---
    "������",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    &bResetSettings,                        ///< ������ - ����� ��������
    &bAutoSearch,                           ///< ������ - ����� �������
    PageService::PageCalibrate::self,       ///< ������ - ����������
    &cSoundVolume,                          ///< ������ - ����
    PageRTC::self,                          ///< ������ - �����
    PageService::PageInformation::self,     ///< ������ - ����������
    PageDebug::self,                        ///< ������ - �������
    PageName::Service, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageService::self = (const Page *)&pService;
