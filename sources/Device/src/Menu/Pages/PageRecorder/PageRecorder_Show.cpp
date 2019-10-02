#include "defines.h"
#include "Display/Symbols.h"
#include "Display/Display_Primitives.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Display.h"
#include "Recorder/Recorder_Storage.h"
#include "Settings/Settings.h"


using namespace Display::Primitives;


DEF_CHOICE_3( cSource,
    "��������",
    "������ ����� ������ ��� ���������",
    "���",
    "����� ��",
    "����� ��",
    set.rec.storagePlay, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



//DEF_CHOICE_2( cCursor,
//    "������",
//    "",
//    "1",
//    "2",
//    set.rec.currentCursor, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
//)


static void OnPress_Next()
{
}

static void Draw_Next(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_RIGHT).Draw4SymbolsInRect(x + 6, y + 2);
}

DEF_GRAPH_BUTTON( bRight,                                                                                                                 //--- ������� - ����������� - �������� - ������ ---
    "������",
    "����������� ���� ��������� ������",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Next, Draw_Next
)


static void OnPress_Prev()
{
}

static void Draw_Prev(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_LEFT).Draw4SymbolsInRect(x + 6, y + 2);
}

DEF_GRAPH_BUTTON( bLeft,                                                                                                                   //--- ������� - ����������� - �������� - ����� ---
    "�����",
    "",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Prev, Draw_Prev
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

DEF_PAGE_5( pShow, // -V641 // -V1027                                                                                                              //--- ������� - ����������� - �������� ---
    "��������",
    "�������� ���������� ������",
    &cSource,
    PageRecorder::PageShow::PageChoice::self,
    &bLeft,
    &bRight,
    PageRecorder::PageShow::PageCursors::self,
    PageName::Recorder_Show, &PageRecorder::self, IsActive_PageShow, Page::OpenClose, Page::BeforeDraw, HandlerKey_PageShow
)

const Page * const PageRecorder::PageShow::self = (const Page *)&pShow;
