#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder_Storage.h"
#include "Recorder/Recorder_Display.h"



DEF_CHOICE_3( cSource,
    "��������",
    "������ ����� ������ ��� ���������",
    "���",
    "����� ��",
    "����� ��",
    set.rec.storagePlay, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



DEF_CHOICE_2( cCursor,
    "������",
    "",
    "1",
    "2",
    set.rec.currentCursor, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Next()
{
    Recorder::Display::MoveLeft();
}

DEF_BUTTON( bNext,                                                                                                                     //--- ������� - ����������� - �������� - ��������� ---
    "�����",
    "",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Next
)


static void OnPress_Prev()
{
    Recorder::Display::MoveRight();
}

DEF_BUTTON( bPrev,                                                                                                                    //--- ������� - ����������� - �������� - ���������� ---
    "������",
    "",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Prev
)


static bool IsActive_PageShow()
{
    return !Recorder::IsRunning();
}

static bool HandlerKey_PageShow(const KeyEvent &event)
{
    if (event.type == TypePress::Press || event.type == TypePress::Repeat)
    {
        if (event.key == Key::Left)
        {
            Recorder::Display::MoveCursorLeft();
            return true;
        }
        else if (event.key == Key::Right)
        {
            Recorder::Display::MoveCursorRight();
            return true;
        }
        else
        {
            // ����� ������ �� ����
        }
    }

    return false;
}

DEF_PAGE_2( pShow, // -V641 // -V1027                                                                                                              //--- ������� - ����������� - �������� ---
    "��������",
    "�������� ���������� ������",
    &cSource,
    PageRecorder::PageShow::PageChoice::self,
    PageName::Recorder_Show, &PageRecorder::self, IsActive_PageShow, Page::OpenClose, Page::BeforeDraw, HandlerKey_PageShow
)

const Page * const PageRecorder::PageShow::self = (const Page *)&pShow;
