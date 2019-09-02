#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include "Display/Font/Font.h"
#include "Display/Symbols.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Hardware/Clock.h"
#include "Utils/Values.h"
#include "Utils/Math.h"


using namespace Display::Primitives;

/// ����� ������������ ������
static int curField = 0;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pRTC, // -V641 // -V1027                                                                                                                                 //--- ������ - ����� ---
    "�����",
    "��������� � ��������� �������",
    PageRTC::PageSet::self,         ///< ������ - ����� - �����
    PageRTC::PageCorrect::self,     ///< C����� - ����� - ���������
    PageName::Service_RTC,
    &PageService::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::self = (const Page *)&pRTC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_SetLeft()
{
    Math::CircleDecrease(&curField, 0, 5);
}

static void Draw_Left(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Left,
    "�����",
    "���������� �������",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetLeft, Draw_Left
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetRight()
{
    Math::CircleIncrease(&curField, 0, 5);
}

static void Draw_Right(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Right,
    "������",
    "��������� �������",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetRight, Draw_Right
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetUp()
{
}

static void Draw_Up(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_UP).Draw4SymbolsInRect(x + 2, y + 4);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Up,
    "������",
    "���������",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetUp, Draw_Up
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetDown()
{
}

static void Draw_Down(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_DOWN).Draw4SymbolsInRect(x + 2, y + 4);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Down,
    "������",
    "���������",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetDown, Draw_Down
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetPick()
{
}

static void Draw_Pick(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_SELECT).Draw4SymbolsInRect(x + 2 , y);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON(bSet_Pick,
    "�������",
    "����� ������������� ��������",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetPick, Draw_Pick
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDigit(int x, int y, int digit)
{
    Integer(digit).ToString(false).Draw(x, y);
}

static void DrawField(PackedTime &time, int numField)
{
    int x0 = 50;
    int y0 = 20;

    int posX = (numField % 3);

    int x = x0 + posX * 90;
    int y = numField < 3 ? y0 : y0 + 100;

    uint fields[6] = { time.hours, time.minutes, time.seconds, time.day, time.month, time.year };

    Color::FILL.SetAsCurrent();

    if (numField == curField)
    {
        Region(69, 67).Fill(x - 2, y - 2, Color::FLASH_01);
        Color::FLASH_10.SetAsCurrent();
    }

    Integer value((int)fields[numField]);

    DrawDigit(x, y, value[1]);
    DrawDigit(x + 35, y, value[0]);

    const char separator[2] = { (numField < 3) ? ':' : '.', '\0' };

    if (posX < 2)
    {
        Text(separator).Draw(x + 74, (numField < 3) ? y - 14 : y, Color::FILL);
    }
}

static void DrawTime(PackedTime &time)
{
    Font::SetCurrent(Font::Type::_Big64);
    int spacing = Font::GetSpacing();
    Font::SetSpacing(5);

    for (int i = 0; i < 6; i++)
    {
        DrawField(time, i);
    }

    Font::SetCurrent(Font::Type::_8);
    Font::SetSpacing(spacing);
}

static void BeforeDraw_Set()
{
    Painter::BeginScene(Color::BACK);

    PackedTime time = Hardware::Clock::GetTime();

    DrawTime(time);
}

static void OnOpenClose_Set(bool)
{
    Color::ResetFlash();
}

DEF_PAGE_5( pSet, //-V641 //-V1027
    "���������",
    "��������� �������� �������",
    &bSet_Left,
    &bSet_Right,
    &bSet_Up,
    &bSet_Down,
    &bSet_Pick,
    PageName::Service_RTC_Set,
    &PageRTC::self, E_BtV, OnOpenClose_Set, BeforeDraw_Set, E_BfKE
)

const Page * const PageRTC::PageSet::self = (const Page *)&pSet;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_CorrectionLeft()
{
}

DEF_GRAPH_BUTTON( bCorrection_Left,
    "�����",
    "���������� �������",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionLeft, Draw_Left
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CorrectionRight()
{
}

DEF_GRAPH_BUTTON( bCorrection_Right,
    "������",
    "��������� �������",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionRight, Draw_Right
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CorrectionUp()
{
}

DEF_GRAPH_BUTTON( bCorrection_Up,
    "������",
    "���������",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionUp, Draw_Up
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CorrectionDown()
{
}

DEF_GRAPH_BUTTON( bCorrection_Down,
    "������",
    "���������",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionDown, Draw_Down
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CorrectionPick()
{
}

DEF_GRAPH_BUTTON( bCorrection_Pick,
    "�����",
    "������������ ������������ �������",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionPick, Draw_Pick
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pCorrection, //-V641 //-V1027
    "���������",
    "��������� �������� ���� �����",
    &bCorrection_Left,
    &bCorrection_Right,
    &bCorrection_Up,
    &bCorrection_Down,
    &bCorrection_Pick,
    PageName::Service_RTC_Correct,
    &PageRTC::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::PageCorrect::self = (const Page *)&pCorrection;
