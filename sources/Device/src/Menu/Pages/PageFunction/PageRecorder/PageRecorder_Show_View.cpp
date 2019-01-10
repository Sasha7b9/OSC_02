#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageView;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cMove,                                                                                                      //--- ������� - ����������� - �������� - �������� - ����������� ---
    "�����������", "Move",
    "",
    "",
    "X", "X",
    "Y", "Y",
    RECORDER_AXIS_MOVE, pageView, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cZoom,
    "�������", "Zoom",
    "",
    "",
    "X", "X",
    "Y", "Y",
    RECORDER_AXIS_ZOOM, pageView, FuncActive, FuncChangedChoice, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageView, // -V641 // -V1027                                                                                                //--- ������� - ����������� - �������� - �������� ---
    "��������", "VIEW",
    "",
    "",
    &cMove, ///< ������� - ����������� - �������� - �������� - �����������
    Page::Name::Function_Recorder_Show_View, PageFunction::PageRecorder::PageShow::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)


const PageBase *PageFunction::PageRecorder::PageShow::PageView::pointer = &pageView;
