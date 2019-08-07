#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageSource;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cChanA,                                                                                                                    //--- ������� - ����������� - �������� - ����� 1 ---
    "����� 1",
    "����� ������ 1 ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    RECORDER_SOURCE_A, pageSource, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cChanB,                                                                                                                    //--- ������� - ����������� - �������� - ����� 2 ---
    "����� 2",
    "����� ������ 2 ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    RECORDER_SOURCE_B, pageSource, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cSensor,                                                                                                                    //--- ������� - ����������� - �������� - ������ ---
    "������",
    "����� ������� ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    RECORDER_SOURCE_SENSOR, pageSource, 0, 0, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageSource, // -V641 // -V1027                                                                                                         //--- ������� - ����������� - �������� ---
    "��������",
    "����� ������������ ��������",
    &cChanA,        ///< ������� - ����������� - �������� - ����� 1
    &cChanB,        ///< ������� - ����������� - �������� - ����� 2
    //&cSensor,       ///< ������� - ����������� - �������� - ������
    Page::Name::Function_Recorder_Source, PageFunction::PageRecorder::pointer, 0, 0, 0, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageSource::pointer = &pageSource;
