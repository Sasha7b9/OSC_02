#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                 //--- ������� - ����������� - ������ ---
    "����������",
    "��������� �������� ������ ������������ ��� ����������� ���� �����������",
    "� ������",
    "������",
    RECORDER_VIEW_ALLWAYS, pageRecorder, Choice::FuncActive, Choice::FuncChange, Choice::FuncDraw
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
    PageFunction::PageRecorder::PageSource::pointer,    ///< ������� - ����������� - ��������
    //PageFunction::PageRecorder::PageRecord::pointer,    ///< ������� - ����������� - ������
    PageFunction::PageRecorder::PageShow::pointer,      ///< ������� - ����������� - ��������
    Page::Name::Function_Recorder, PageFunction::pointer, Page::FuncActive, OnOpenClose_Recorder, Page::FuncDraw, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::pointer = &pageRecorder;
