#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecorderCursors;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Center()
{

}

DEF_BUTTON( bCenter,
    "� �����", "To the center",
    "",
    "",
    pageRecorderCursors, FuncActive, OnPress_Center, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Left()
{

}

DEF_BUTTON( bLeft,
    "����", "Left",
    "",
    "",
    pageRecorderCursors, FuncActive, OnPress_Left, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Right()
{

}

DEF_BUTTON( bRight,
    "�����", "Right",
    "",
    "",
    pageRecorderCursors, FuncActive, OnPress_Right, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageRecorderCursors, // -V641 // -V1027
    "�������", "CURSORS",
    "",
    "",
    &bCenter,
    &bLeft,
    &bRight,
    Page::Name::Function_Recorder_Show_View_Cursors, PageFunction::PageRecorder::PageShow::PageView::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::PageView::PageCursors::pointer = &pageRecorderCursors;
