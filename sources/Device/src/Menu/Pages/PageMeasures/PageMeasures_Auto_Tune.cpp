#include "defines.h"
#include "Menu/MenuItems.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Osci/Measurements/Measures.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Utils/Math.h"
#include "Hardware/Beeper.h"
#include "Osci/Measurements/Measurements.h"


using namespace Display::Primitives;
using namespace Osci;
using namespace Osci::Measurements;

extern const Page pageTune;

const Page * const PageMeasuresAuto::PageTune::self = (const Page *)&pageTune;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Draw_Markers(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x60').Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bMarkers,                                                                                                                          //--- ��������� - ��������� - ������ ---
    "������",
    "��������� ���������� ������� ��� ����������� �������� ���������",
    &PageMeasuresAuto::PageTune::self, Item::Active, Measure::ShortPressOnSmallButonMarker, Draw_Markers
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Settings(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);

    Char('\x62').Draw4SymbolsInRect(x + 2, y + 1);

    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSettings,                                                                                                                      //--- ��������� - ��������� - ��������� ---
    "���������",
    "��������� ������� ����������� ���������",
    &PageMeasuresAuto::PageTune::self, Item::Active, Measurements::PageChoice::ChangeState, Draw_Settings
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Tune()
{
    return SHOW_MEASURES;
}

static bool OnArrows_Tune(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        Osci::Measurements::PageChoice::OnKeyEvent(event);
        return true;
    }

    return false;
}

DEF_PAGE_2( pageTune, // -V641 // -V1027
    "���������",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    &bMarkers,
    &bSettings,
    PageName::Measures_Auto_Tune, &PageMeasuresAuto::self, IsActive_Tune, Page::Changed, Page::BeforeDraw, OnArrows_Tune
)
