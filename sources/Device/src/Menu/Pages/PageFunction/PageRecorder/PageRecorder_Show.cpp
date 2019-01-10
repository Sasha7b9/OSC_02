#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageShow;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cSource,                                                                                                                  //--- ������� - ����������� - �������� - �������� ---
    "��������", "Source",
    "����� ������������ ������",
    "Location of data",
    "���", "RAM",
    "USB-Flash", "USB-Flash",
    "USB-PC",    "USB-PC",
    RECORDER_STORAGE_PLAY, pageShow, FuncActive, FuncChangedChoice, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{

}

DEF_BUTTON( bNext,
    "���������", "Next",
    "����������", "Previous",
    pageShow, FuncActive, OnPress_Next, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageShow, // -V641 // -V1027                                                                                                           //--- ������� - ����������� - �������� ---
    "��������", "VIEW",
    "�������� ���������� ������",
    "View recorded data",
    &cSource,           ///< ������� - ����������� - �������� - ��������
    &bNext,             ///< ������� - ����������� _ �������� - ���������
    Page::Name::Function_Recorder_Play, PageFunction::PageRecorder::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::pointer = &pageShow;
