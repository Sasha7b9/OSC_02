#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"


extern const Page pageRecord;

const Page * const PageRecorder::PageRecord::self = (const Page *)&pageRecord;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cDestination,                                                                                                            //--- ������� - ����������� - ������ - ��������� � ---
    "��������� �",
    "���� ��������� ������",
    "���",
    "USB-Flash",
    "USB-PC",
    RECORDER_STORAGE_RECORD, &PageRecorder::PageRecord::self, 0, 0, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageRecord, // -V641 // -V1027                                                                                                           //--- ������� - ����������� - ������ ---
    "������",
    "����� ����� �������� ������",
    &cDestination,        ///< ������� - ����������� - ������ - ��������� �
    PageName::Function_Recorder_Record, &PageRecorder::self, 0, 0, 0, 0
)
