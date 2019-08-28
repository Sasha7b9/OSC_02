#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"


extern const Page pageCalibrate;

const Page * const PageService::PageCalibrate::self = (const Page *)&pageCalibrate;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Calibrator_Calibrator(bool)
{
    FPGA::Settings::LoadCalibratorMode();
}

DEF_CHOICE_2( cCalibrator_Calibrator, // -V206                                                                                                     //--- ������ - ���������� - ���������� ---
    "����������",
    "����� ������ �����������",
    "�����",
    "����",
    CALIBRATOR_MODE, &PageService::PageCalibrate::self, 0, OnChanged_Calibrator_Calibrator, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Calibrator_Calibrate()
{
    //return !(SET_CALIBR_MODE_A == CalibrationMode::Disable && CALIBR_MODE_B == CalibrationMode::Disable);
    return true;
}

static void OnPress_Calibrator_Calibrate()
{
    Display::FuncOnWaitStart("���������� ������ 1", false);

    Timer::PauseOnTime(5000);

    Display::FuncOnWaitStop();

    Display::FuncOnWaitStart("���������� ������ 2", false);

    Timer::PauseOnTime(5000);

    Display::FuncOnWaitStop();
}

DEF_BUTTON( bCalibrator_Calibrate,                                                                                                                //--- ������ - ���������� - ����������� ---
    "�����������",
    "������ ��������� ����������",
    &PageService::PageCalibrate::self, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageCalibrate, // -V641 // -V1027                                                                                                                   //--- ������ - ���������� ---
    "����������",
    "����������� ������������ � ���������� ������������",
    &cCalibrator_Calibrator,     /// ������ - ���������� - ����������
    &bCalibrator_Calibrate,      /// ������ - ���������� - �����������
    PageName::Service_Calibrator, &PageService::self, E_BtV, E_VB, E_VV, E_BfKE
)
