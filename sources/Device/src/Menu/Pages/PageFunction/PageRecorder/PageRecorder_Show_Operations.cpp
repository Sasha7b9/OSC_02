#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"


extern const PageBase pageOperations;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Rename()
{

}

DEF_BUTTON( bRename,                                                                                                    //--- ������� - ����������� - �������� - �������� - ������������� ---
    "�������������",
    "",
    pageOperations, Button::FuncActive, OnPress_Rename, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Copy()
{

}

DEF_BUTTON( bCopy,                                                                                                         //--- ������� - ����������� - �������� - �������� - ���������� ---
    "����������",
    "",
    pageOperations, Button::FuncActive, OnPress_Copy, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Move()
{

}

DEF_BUTTON( bMove,                                                                                                        //--- ������� - ����������� - �������� - �������� - ����������� ---
    "�����������",
    "",
    pageOperations, Button::FuncActive, OnPress_Move, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Delete()
{

}

DEF_BUTTON( bDelete,                                                                                                          //--- ������� - ����������� - �������� - �������� - ������� ---
    "�������",
    "",
    pageOperations, Button::FuncActive, OnPress_Delete, Button::EmptyDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageOperations, // -V641 // -V1027                                                                                          //--- ������� - ����������� - �������� - �������� ---
    "��������",
    "",
    &bRename,   ///< ������� - ����������� - �������� - �������� - �������������
    &bCopy,     ///< ������� - ����������� - �������� - �������� - ����������
    &bMove,     ///< ������� - ����������� - �������� - �������� - �����������
    &bDelete,   ///< ������� - ����������� - �������� - �������� - �������
    Page::Name::Function_Recorder_Show_Operations, PageFunction::PageRecorder::PageShow::pointer, Page::FuncActive, Page::FuncPress, Page::FuncDraw, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::PageOperations::pointer = &pageOperations;
