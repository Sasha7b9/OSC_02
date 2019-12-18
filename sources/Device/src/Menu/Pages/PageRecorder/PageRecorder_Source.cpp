#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"



DEF_CHOICE_2( cChanA,                                                                                                                    //--- ������� - ����������� - �������� - ����� 1 ---
    "����� 1",
    "����� ������ 1 ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    set.rec.enA, &PageRecorder::Source::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanB,                                                                                                                    //--- ������� - ����������� - �������� - ����� 2 ---
    "����� 2",
    "����� ������ 2 ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    set.rec.enB, &PageRecorder::Source::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cSensor,                                                                                                                    //--- ������� - ����������� - �������� - ������ ---
    "������",
    "����� ������� ��� ������ � ���������",
    DISABLE_RU,
    ENABLE_RU,
    set.rec.enSensor, &PageRecorder::Source::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



static bool IsActive_Source()
{
    return !Recorder::IsRunning();
}

DEF_PAGE_3( pSource,                                                                                                                               //--- ������� - ����������� - �������� ---
    "��������",
    "����� ������������ ��������",
    &cChanA,
    &cChanB,
    &cSensor,
    PageName::Recorder_Source, &PageRecorder::self, IsActive_Source, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageRecorder::Source::self = static_cast<const Page *>(&pSource);
