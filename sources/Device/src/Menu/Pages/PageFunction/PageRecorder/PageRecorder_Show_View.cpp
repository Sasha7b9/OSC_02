#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cMove,                                                                                                      //--- ������� - ����������� - �������� - �������� - ����������� ---
    "�����������",
    "",
    "X",
    "Y",
    set.rec.axisMove, &PageRecorder::PageShow::PageView::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cZoom,                                                                                                          //--- ������� - ����������� - �������� - �������� - ������� ---
    "�������",
    "",
    "X",
    "Y",
    set.rec.axisZoom, &PageRecorder::PageShow::PageView::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Less()
{

}

DEF_BUTTON( bLess,                                                                                                             //--- ������� - ����������� - �������� - �������� - ������ ---
    "������",
    "",
    &PageRecorder::PageShow::PageView::self, Item::Active, OnPress_Less
)


static void OnPress_More()
{

}

DEF_BUTTON( bMore,                                                                                                             //--- ������� - ����������� - �������� - �������� - ������ ---
    "������",
    "",
    &PageRecorder::PageShow::PageView::self, Item::Active, OnPress_More
)


DEF_PAGE_5( pView, // -V641 // -V1027                                                                                                   //--- ������� - ����������� - �������� - �������� ---
    "��������",
    "",
    &cMove,                                                                 ///< ������� - ����������� - �������� - �������� - �����������
    &cZoom,                                                                 ///< ������� - ����������� - �������� - �������� - �������
    &bLess,                                                                 ///< ������� - ����������� - �������� - �������� - ������
    &bMore,                                                                 ///< ������� - ����������� - �������� - �������� - ������
    PageRecorder::PageShow::PageView::PageCursors::self,      ///< ������� - ����������� - �������� - �������� - �������
    PageName::Function_Recorder_Show_View, &PageRecorder::PageShow::self, Item::Active, Page::Changed, Page::BeforeDraw, Page::Handler
)

const Page * const PageRecorder::PageShow::PageView::self = (const Page *)&pView;
