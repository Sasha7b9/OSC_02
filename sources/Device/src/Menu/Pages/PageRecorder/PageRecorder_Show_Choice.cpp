#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Display/Font/Font.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"


static void OnPress_Up()
{
}

static void Draw_Up(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_UP).Draw4SymbolsInRect(x + 2, y + 4);
}

static bool IsActive_Up()
{
    return true;
}

DEF_GRAPH_BUTTON( bUp,
    "����������",
    "������� � ���������� ������",
    &PageRecorder::PageShow::PageChoice::self, IsActive_Up, OnPress_Up, Draw_Up
)


static void OnPress_Down()
{

}

static void Draw_Down(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_DOWN).Draw4SymbolsInRect(x + 2, y + 4);
}

static bool IsActive_Down()
{
    return true;
}

DEF_GRAPH_BUTTON( bDown,
    "���������",
    "������� � ��������� ������",
    &PageRecorder::PageShow::PageChoice::self, IsActive_Down, OnPress_Down, Draw_Down
)


DEF_PAGE_2( pChoice,
    "�����",
    "����� ������� ��� ������",
    &bUp,
    &bDown,
    PageName::Recorder_Show_Choice,
    &PageRecorder::PageShow::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


const Page *const PageRecorder::PageShow::PageChoice::self = (const Page *)&pChoice;
