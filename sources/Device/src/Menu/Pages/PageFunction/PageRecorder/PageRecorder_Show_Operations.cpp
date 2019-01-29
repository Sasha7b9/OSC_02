#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"


extern const PageBase pageOperations;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Rename()
{

}

DEF_BUTTON( bRename,                                                                                                    //--- ������� - ����������� - �������� - �������� - ������������� ---
    "�������������", "Rename",
    "",
    "",
    pageOperations, FuncActive, OnPress_Rename, Button::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Copy()
{

}

DEF_BUTTON( bCopy,                                                                                                         //--- ������� - ����������� - �������� - �������� - ���������� ---
    "����������", "Copy",
    "",
    "",
    pageOperations, FuncActive, OnPress_Copy, Button::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Move()
{

}

DEF_BUTTON( bMove,                                                                                                        //--- ������� - ����������� - �������� - �������� - ����������� ---
    "�����������", "Move",
    "",
    "",
    pageOperations, FuncActive, OnPress_Move, Button::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Delete()
{

}

DEF_BUTTON( bDelete,                                                                                                          //--- ������� - ����������� - �������� - �������� - ������� ---
    "�������", "Delete",
    "",
    "",
    pageOperations, FuncActive, OnPress_Delete, Button::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageOperations, // -V641 // -V1027                                                                                          //--- ������� - ����������� - �������� - �������� ---
    "��������", "OPERATIONS",
    "",
    "",
    &bRename,   ///< ������� - ����������� - �������� - �������� - �������������
    &bCopy,     ///< ������� - ����������� - �������� - �������� - ����������
    &bMove,     ///< ������� - ����������� - �������� - �������� - �����������
    &bDelete,   ///< ������� - ����������� - �������� - �������� - �������
    Page::Name::Function_Recorder_Show_Operations, PageFunction::PageRecorder::PageShow::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::PageOperations::pointer = &pageOperations;
