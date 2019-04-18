#include "defines.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageTime.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Settings/Settings.h"

#include "Osci/Osci.h"


using namespace Osci::Settings;


extern const PageBase pTime;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Sample()
{
    return Osci::InModeRandomizer() && !START_MODE_IS_SINGLE;
}

DEF_CHOICE_2( cSample,                                                                                                                                          //--- ��������� - ������� ---
    "�������"
    ,
    "\"��������\" - \n"
    "\"�������������\" -"
    ,
    "�������� �����",
    "�������������",
    SAMPLE_TYPE, pTime, IsActive_Sample, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_PeakDet()
{
    return (SET_TBASE >= TBase::MIN_PEAK_DET);
}

void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        Osci::OnChangedPoints();
    }
    else
    {
        Display::ShowWarning(Warning::TooSmallSweepForPeakDet);
    }
}

DEF_CHOICE_2( cPeakDet,                                                                                                                                         //--- ��������� - ��� ��� ---
    "��� ���",
    "��������/��������� ������� ��������."
    ,
    DISABLE_RU,
    ENABLE_RU,
    SET_PEAKDET, pTime, IsActive_PeakDet, PageTime::OnChanged_PeakDet, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageTime::OnChanged_TPos(bool active)
{
    PageMemory::OnChanged_Points(active);
    TShift::Set(SET_TSHIFT);
}

DEF_CHOICE_3( cTPos,                                                                                                                                                 //--- ��������� - �� ---
    "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����."
    ,
    "����",
    "�����",
    "�����",
    TPOS, pTime, FuncActive, PageTime::OnChanged_TPos, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cDivRole,                                                                                       //--- ��������� - �-��� ��/��� ---
    "�-��� ��/���"
    ,
    "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
    "1. \"�����\" - ��������� �������� �� �������.\n"
    "2. \"������\" - ����������� �� ������."
    ,
    "�����",
    "������",
    TIME_DIV_XPOS, pTime, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cShiftXtype,                                                                                                                                     //--- ��������� - �������� ---
    "��������",
    "����� ����� ��������� �������� �� �����������\n1. \"�����\" - ����������� ���������� �������� � ��������.\n2. \"�������\" - ����������� ��������� ������� �� ������."
    ,
    "�����",
    "�������",
    LINKING_TSHIFT, pTime, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTime::pointer = &pTime;


DEF_PAGE_3( pTime, // -V641 // -V1027                                                                                                                                     //--- ��������� ---
    "����",
    "�������� ��������� ��������",
    &cSample,           ///< ��������� - �������
    &cPeakDet,          ///< ��������� - ��� ���
    &cTPos,             ///< ��������� - ��
    //&cDivRole,          ///< ��������� - �-��� ��/���
    //&cShiftXtype,       ///< ��������� - ��������
    Page::Name::Time, nullptr, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
