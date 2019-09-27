#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Calibrator(bool)
{
    FPGA::Settings::LoadCalibratorMode();
}

DEF_CHOICE_2( cCalibrator, // -V206                                                                                                                //--- ������ - ���������� - ���������� ---
    "����������",
    "����� ������ �����������",
    "�����",
    "����",
    CALIBRATOR_MODE, &PageService::PageCalibrate::self, Item::EmptyActive, OnChanged_Calibrator, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Calibrate()
{
    //return !(SET_CALIBR_MODE_A == CalibrationMode::Disable && CALIBR_MODE_B == CalibrationMode::Disable);
    return true;
}

static void OnPress_Calibrate()
{
    Display::FuncOnWaitStart("���������� ������ 1", false);

    Timer::PauseOnTime(5000);

    Display::FuncOnWaitStop();

    Display::FuncOnWaitStart("���������� ������ 2", false);

    Timer::PauseOnTime(5000);

    Display::FuncOnWaitStop();
}

DEF_BUTTON( bCalibrate,                                                                                                                           //--- ������ - ���������� - ����������� ---
    "�����������",
    "������ ��������� ����������",
    &PageService::PageCalibrate::self, IsActive_Calibrate, OnPress_Calibrate
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pCalibrate, // -V641 // -V1027                                                                                                                      //--- ������ - ���������� ---
    "����������",
    "����������� ������������ � ���������� ������������",
    &cCalibrator,     /// ������ - ���������� - ����������
    &bCalibrate,      /// ������ - ���������� - �����������
    PageName::Service_Calibrator, &PageService::self, Item::EmptyActive, E_VB, Page::EmptyBeforeDraw, E_BfKE
)

const Page * const PageService::PageCalibrate::self = (const Page *)&pCalibrate;
