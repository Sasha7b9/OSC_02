#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                 //--- ������� - ����������� - ������ ---
    "����������", "Show",
    "��������� �������� ������ ������������ ��� ����������� ���� �����������",
    "Allows you to display data from the recorder when the recorder menu is off",
    "� ������", "In mode",
    "������",   "In always",
    RECORDER_VIEW_ALLWAYS, pageRecorder, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}


DEF_PAGE_2( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ������� - ����������� ---
    "�����������", "RECORDER",
    "������ � ��������������� �������� ������ � ��������",
    "Recording and playback of input signals and sensors",
    //&cShow,                                             ///< ������� - ����������� - ����������
    PageFunction::PageRecorder::PageSource::pointer,    ///< ������� - ����������� - ��������
    //PageFunction::PageRecorder::PageRecord::pointer,    ///< ������� - ����������� - ������
    PageFunction::PageRecorder::PageShow::pointer,      ///< ������� - ����������� - ��������
    Page::Name::Function_Recorder, PageFunction::pointer, FuncActive, OnOpenClose_Recorder, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::pointer = &pageRecorder;
