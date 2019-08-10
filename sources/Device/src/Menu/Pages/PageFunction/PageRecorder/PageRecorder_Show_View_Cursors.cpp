#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"


extern const PageDef pageRecorderCursors;

const Page * const PageRecorder::PageShow::PageView::PageCursors::self = (const Page *)&pageRecorderCursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Center()
{

}

DEF_BUTTON( bCenter,
    "� �����",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, 0, OnPress_Center, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Left()
{

}

DEF_BUTTON( bLeft,
    "����",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, 0, OnPress_Left, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Right()
{

}

DEF_BUTTON( bRight,
    "�����",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, 0, OnPress_Right, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageRecorderCursors, // -V641 // -V1027
    "�������",
    "",
    &bCenter,
    &bLeft,
    &bRight,
    Page::Name::Function_Recorder_Show_View_Cursors, &PageRecorder::PageShow::PageView::self, 0, 0, 0, 0
)
