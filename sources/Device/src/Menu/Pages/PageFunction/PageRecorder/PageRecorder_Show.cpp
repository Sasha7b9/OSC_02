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

DEF_BUTTON( bNext,                                                                                                                     //--- ������� - ����������� - �������� - ��������� ---
    "���������", "Next",
    "",
    "",
    pageShow, FuncActive, OnPress_Next, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Prev()
{

}

DEF_BUTTON( bPrev,                                                                                                                    //--- ������� - ����������� - �������� - ���������� ---
    "����������", "Previous",
    "",
    "",
    pageShow, FuncActive, OnPress_Prev, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageShow, // -V641 // -V1027                                                                                                           //--- ������� - ����������� - �������� ---
    "��������", "VIEW",
    "�������� ���������� ������",
    "View recorded data",
    &cSource,                                                       ///< ������� - ����������� - �������� - ��������
    &bNext,                                                         ///< ������� - ����������� - �������� - ���������
    &bPrev,                                                         ///< ������� - ����������� - �������� - ����������
    PageFunction::PageRecorder::PageShow::PageOperations::pointer,  ///< ������� - ����������� - �������� - ��������
    PageFunction::PageRecorder::PageShow::PageView::pointer,        ///< ������� - ����������� - �������� - ��������
    Page::Name::Function_Recorder_Show, PageFunction::PageRecorder::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::pointer = &pageShow;
