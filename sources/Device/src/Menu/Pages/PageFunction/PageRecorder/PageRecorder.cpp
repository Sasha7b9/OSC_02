#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


extern const PageBase pageRecorder;

const Page * const PageFunction::PageRecorder::self = (const Page * const)&pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                 //--- ������� - ����������� - ������ ---
    "����������",
    "��������� �������� ������ ������������ ��� ����������� ���� �����������",
    "� ������",
    "������",
    RECORDER_VIEW_ALLWAYS, PageFunction::PageRecorder::self, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}


DEF_PAGE_2( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ������� - ����������� ---
    "�����������",
    "������ � ��������������� �������� ������ � ��������",
    //&cShow,                                             ///< ������� - ����������� - ����������
    PageFunction::PageRecorder::PageSource::self,    ///< ������� - ����������� - ��������
    //PageFunction::PageRecorder::PageRecord::self,    ///< ������� - ����������� - ������
    PageFunction::PageRecorder::PageShow::self,      ///< ������� - ����������� - ��������
    Page::Name::Function_Recorder, PageFunction::self, 0, OnOpenClose_Recorder, 0, 0
)
