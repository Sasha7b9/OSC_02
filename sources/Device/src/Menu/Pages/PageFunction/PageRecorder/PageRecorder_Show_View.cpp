#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cMove,                                                                                                      //--- ������� - ����������� - �������� - �������� - ����������� ---
    "�����������",
    "",
    "X",
    "Y",
    REC_AXIS_MOVE, &PageRecorder::PageShow::PageView::self, Item::EmptyActive, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cZoom,                                                                                                          //--- ������� - ����������� - �������� - �������� - ������� ---
    "�������",
    "",
    "X",
    "Y",
    REC_AXIS_ZOOM, &PageRecorder::PageShow::PageView::self, Item::EmptyActive, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Less()
{

}

DEF_BUTTON( bLess,                                                                                                             //--- ������� - ����������� - �������� - �������� - ������ ---
    "������",
    "",
    &PageRecorder::PageShow::PageView::self, Item::EmptyActive, OnPress_Less
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_More()
{

}

DEF_BUTTON( bMore,                                                                                                             //--- ������� - ����������� - �������� - �������� - ������ ---
    "������",
    "",
    &PageRecorder::PageShow::PageView::self, Item::EmptyActive, OnPress_More
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pView, // -V641 // -V1027                                                                                                   //--- ������� - ����������� - �������� - �������� ---
    "��������",
    "",
    &cMove,                                                                 ///< ������� - ����������� - �������� - �������� - �����������
    &cZoom,                                                                 ///< ������� - ����������� - �������� - �������� - �������
    &bLess,                                                                 ///< ������� - ����������� - �������� - �������� - ������
    &bMore,                                                                 ///< ������� - ����������� - �������� - �������� - ������
    PageRecorder::PageShow::PageView::PageCursors::self,      ///< ������� - ����������� - �������� - �������� - �������
    PageName::Function_Recorder_Show_View, &PageRecorder::PageShow::self, Item::EmptyActive, E_VB, Page::EmptyBeforeDraw, E_BfKE
)

const Page * const PageRecorder::PageShow::PageView::self = (const Page *)&pView;
