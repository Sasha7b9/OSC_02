#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cChanA,                                                                                                                    //--- ������� - ����������� - �������� - ����� 1 ---
    "����� 1",
    "����� ������ 1 ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    REC_SRC_A, &PageRecorder::PageSource::self, Item::Active, Choice::Changed, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cChanB,                                                                                                                    //--- ������� - ����������� - �������� - ����� 2 ---
    "����� 2",
    "����� ������ 2 ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    REC_SRC_B, &PageRecorder::PageSource::self, Item::Active, Choice::Changed, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cSensor,                                                                                                                    //--- ������� - ����������� - �������� - ������ ---
    "������",
    "����� ������� ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    REC_SRC_SENSOR, &PageRecorder::PageSource::self, Item::Active, Choice::Changed, E_VII
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Source()
{
    return !Recorder::IsRunning();
}

DEF_PAGE_3( pSource, // -V641 // -V1027                                                                                                            //--- ������� - ����������� - �������� ---
    "��������",
    "����� ������������ ��������",
    &cChanA,        ///< ������� - ����������� - �������� - ����� 1
    &cChanB,        ///< ������� - ����������� - �������� - ����� 2
    &cSensor,       ///< ������� - ����������� - �������� - ������
    PageName::Function_Recorder_Source, &PageRecorder::self, IsActive_Source, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageRecorder::PageSource::self = (const Page *)&pSource;
