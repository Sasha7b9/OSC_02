#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


extern const Page pageRecorder;

const Page * const PageRecorder::self = (const Page *)&pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cDestination,                                                                                                                     //--- ������� - ����������� - ��������� � ---
    "��������� �",
    "���� ��������� ������",
    "���",
    "���",
    "����� ��",
    RECORDER_STORAGE_RECORD, &PageRecorder::self, E_BtV, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}

DEF_PAGE_3( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ������� - ����������� ---
    "�����������",
    "������ � ��������������� �������� ������ � ��������",
    PageRecorder::PageSource::self,     ///< ������� - ����������� - ��������
    &cDestination,                      ///< ������� - ����������� - ��������� �
    PageRecorder::PageShow::self,       ///< ������� - ����������� - ��������
    PageName::Function_Recorder, &PageFunction::self, E_BtV, OnOpenClose_Recorder, E_VV, E_BfKE
)
