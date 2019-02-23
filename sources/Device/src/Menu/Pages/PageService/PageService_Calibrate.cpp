#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageCalibrate;



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Calibrator_Calibrator(bool)
{
    FPGA::Settings::LoadCalibratorMode();
}

DEF_CHOICE_2(cCalibrator_Calibrator, // -V206                                                                                                     //--- ������ - ���������� - ���������� ---
    "����������", "Calibrator",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "�����", "DC",
    "+4V", "+4V",
    //"0V", "0V",
    CALIBRATOR_MODE, pageCalibrate, FuncActive, OnChanged_Calibrator_Calibrator, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Calibrator_Calibrate()
{
    return !(SET_CALIBR_MODE_A == CalibrationMode::Disable && CALIBR_MODE_B == CalibrationMode::Disable);
}

static void OnPress_Calibrator_Calibrate()
{
}

DEF_BUTTON(bCalibrator_Calibrate,                                                                                                                //--- ������ - ���������� - ����������� ---
    "�����������", "Calibrate",
    "������ ��������� ����������",
    "Running the calibration procedure",
    pageCalibrate, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate, Button::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(pageCalibrate, // -V641 // -V1027                                                                                                                    //--- ������ - ���������� ---
    "����������", "CALIBRATE",
    "����������� ������������ � ���������� ������������",
    "Control of the calibrator and calibration of an oscillograph",
    &cCalibrator_Calibrator,     // ������ - ���������� - ����������
    &bCalibrator_Calibrate,      // ������ - ���������� - �����������
    Page::Name::Service_Calibrator, PageService::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)


const PageBase *PageService::PageCalibrate::pointer = &pageCalibrate;

