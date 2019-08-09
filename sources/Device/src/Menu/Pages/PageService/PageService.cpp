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
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageService.h"
#include "Keyboard/Keyboard.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include <cstdio>

#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;
using namespace Osci::Settings;

extern const PageBase pService;
extern const PageBase ppFunction;
extern const PageBase ppSound;
extern const PageBase ppInformation;

const PageBase *PageService::pointer = &pService;


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
    pService, EmptyFuncBtV, OnPress_ResetSettings, EmptyFuncVII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON( bAutoSearch,                                                                                                                                     //--- ������ - ����� ������� ---
    "����� ����",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    pService, 0, OnPress_AutoSearch, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cSound_Enable,                                                                                                                                   //--- ������ - ���� - ���� ---
    "����",
    "���������/���������� �����",
    DISABLE_RU,
    ENABLE_RU,
    SOUND_ENABLED, ppSound, 0, 0, 0
)

DEF_GOVERNOR( gSound_Volume,                                                                                                                              //--- ������ - ���� - ��������� ---
    "���������",
    "��������� ��������� �����",
    SOUND_VOLUME, 0, 100, ppSound, 0, 0, 0
)

DEF_PAGE_2( ppSound, // -V641 // -V1027                                                                                                                               //--- ������ - ���� ---
    "����",
    "� ���� ���� ����� ��������� ��������� �����",
    &cSound_Enable,             ///< ������ - ���� - ����
    &gSound_Volume,             ///< ������ - ���� - ���������
    Page::Name::Service_Sound, &pService, 0, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_7( pService, // -V641 // -V1027                                                                                                                                    //--- ������ ---
    "������",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    &bResetSettings,                        ///< ������ - ����� ��������
    &bAutoSearch,                           ///< ������ - ����� �������
    PageService::PageCalibrate::pointer,    ///< ������ - ����������
    &ppSound,                               ///< ������ - ����
    PageService::PageRTC::pointer,          ///< ������ - �����
    PageService::PageInformation::pointer,  ///< ������ - ����������
    PageService::PageDebug::pointer,        ///< ������ - �������
    Page::Name::Service, nullptr, 0, 0, 0, 0
)
