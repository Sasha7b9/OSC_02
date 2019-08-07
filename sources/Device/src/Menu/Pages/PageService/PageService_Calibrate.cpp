#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"

#include "Hardware/Timer.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageCalibrate;



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
    CALIBRATOR_MODE, pageCalibrate, 0, OnChanged_Calibrator_Calibrator, 0
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
    pageCalibrate, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate, Button::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageCalibrate, // -V641 // -V1027                                                                                                                   //--- ������ - ���������� ---
    "����������",
    "����������� ������������ � ���������� ������������",
    &cCalibrator_Calibrator,     /// ������ - ���������� - ����������
    &bCalibrator_Calibrate,      /// ������ - ���������� - �����������
    Page::Name::Service_Calibrator, PageService::pointer, 0, Page::FuncPress, 0, FuncRegSetPage
)


const PageBase * const PageService::PageCalibrate::pointer = &pageCalibrate;

