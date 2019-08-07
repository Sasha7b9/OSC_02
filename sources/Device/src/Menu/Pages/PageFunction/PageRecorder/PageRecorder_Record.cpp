#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecord;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cDestination,                                                                                                            //--- ������� - ����������� - ������ - ��������� � ---
    "��������� �",
    "���� ��������� ������",
    "���",
    "USB-Flash",
    "USB-PC",
    RECORDER_STORAGE_RECORD, pageRecord, 0, 0, Button::EmptyDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageRecord, // -V641 // -V1027                                                                                                           //--- ������� - ����������� - ������ ---
    "������",
    "����� ����� �������� ������",
    &cDestination,        ///< ������� - ����������� - ������ - ��������� �
    Page::Name::Function_Recorder_Record, PageFunction::PageRecorder::pointer, 0, Page::FuncPress, 0, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageRecord::pointer = &pageRecord;
