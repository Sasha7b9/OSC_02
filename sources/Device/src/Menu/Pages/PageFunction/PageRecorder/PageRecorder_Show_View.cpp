#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"


extern const PageBase pageView;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cMove,                                                                                                      //--- ������� - ����������� - �������� - �������� - ����������� ---
    "�����������", "Move",
    "",
    "",
    "X", "X",
    "Y", "Y",
    RECORDER_AXIS_MOVE, pageView, FuncActive, Choice::EmptyChange, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cZoom,                                                                                                          //--- ������� - ����������� - �������� - �������� - ������� ---
    "�������", "Zoom",
    "",
    "",
    "X", "X",
    "Y", "Y",
    RECORDER_AXIS_ZOOM, pageView, FuncActive, Choice::EmptyChange, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Less()
{

}

DEF_BUTTON( bLess,                                                                                                             //--- ������� - ����������� - �������� - �������� - ������ ---
    "������", "Less",
    "",
    "",
    pageView, FuncActive, OnPress_Less, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_More()
{

}

DEF_BUTTON( bMore,                                                                                                             //--- ������� - ����������� - �������� - �������� - ������ ---
    "������", "More",
    "",
    "",
    pageView, FuncActive, OnPress_More, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageView, // -V641 // -V1027                                                                                                //--- ������� - ����������� - �������� - �������� ---
    "��������", "VIEW",
    "",
    "",
    &cMove,                                                                 ///< ������� - ����������� - �������� - �������� - �����������
    &cZoom,                                                                 ///< ������� - ����������� - �������� - �������� - �������
    &bLess,                                                                 ///< ������� - ����������� - �������� - �������� - ������
    &bMore,                                                                 ///< ������� - ����������� - �������� - �������� - ������
    PageFunction::PageRecorder::PageShow::PageView::PageCursors::pointer,   ///< ������� - ����������� - �������� - �������� - �������
    Page::Name::Function_Recorder_Show_View, PageFunction::PageRecorder::PageShow::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)


const PageBase *PageFunction::PageRecorder::PageShow::PageView::pointer = &pageView;
