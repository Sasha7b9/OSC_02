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
#include <cstdio>

#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;
using namespace Osci::Settings;

extern const Page pageService;

const Page * const PageService::self = (const Page *)&pageService;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    &PageService::self, EmptyFuncBtV, OnPress_ResetSettings, EmptyFuncVII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON( bAutoSearch,                                                                                                                                     //--- ������ - ����� ������� ---
    "����� ����",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    &PageService::self, 0, OnPress_AutoSearch, 0
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
    &PageService::self, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_7( pageService, // -V641 // -V1027                                                                                                                                    //--- ������ ---
    "������",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    &bResetSettings,                        ///< ������ - ����� ��������
    &bAutoSearch,                           ///< ������ - ����� �������
    PageService::PageCalibrate::self,    ///< ������ - ����������
    &cSoundVolume,                          ///< ������ - ����
    PageRTC::self,          ///< ������ - �����
    PageService::PageInformation::self,  ///< ������ - ����������
    PageDebug::self,        ///< ������ - �������
    Page::Name::Service, nullptr, 0, 0, 0, 0
)
