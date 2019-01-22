#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecord;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cDestination,                                                                                                            //--- ������� - ����������� - ������ - ��������� � ---
    "��������� �", "Save to",
    "���� ��������� ������",
    "Where to save data",
    "���",       "RAM",
    "USB-Flash", "USB-Flash",
    "USB-PC",    "USB-PC",
    RECORDER_STORAGE_RECORD, pageRecord, FuncActive, Choice::EmptyChange, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageRecord, // -V641 // -V1027                                                                                                           //--- ������� - ����������� - ������ ---
    "������", "RECORD",
    "����� ����� �������� ������",
    "Choosing a storage location",
    &cDestination,        ///< ������� - ����������� - ������ - ��������� �
    Page::Name::Function_Recorder_Record, PageFunction::PageRecorder::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageRecord::pointer = &pageRecord;
