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

extern const Page pTune;

const Page * const PageAutoMeasures::PageTune::self = (const Page *)&pTune;



static void Draw_Markers(int x, int y)
{
    Char(SymbolUGO2::BIG_M).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bMarkers,                                                                                                                          //--- ��������� - ��������� - ������ ---
    "������",
    "��������� ���������� ������� ��� ����������� �������� ���������",
    &PageAutoMeasures::PageTune::self, Item::Active, Measure::ShortPressOnSmallButonMarker, Draw_Markers
)


static void Draw_Settings(int x, int y)
{
    Char(SymbolUGO2::SETTINGS).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bSettings,                                                                                                                      //--- ��������� - ��������� - ��������� ---
    "���������",
    "��������� ������� ����������� ���������",
    &PageAutoMeasures::PageTune::self, Item::Active, Measurements::PageChoice::ChangeState, Draw_Settings
)


static bool IsActive_Tune()
{
    return set.meas.show;
}

static bool HandlerKey_Tune(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        Osci::Measurements::PageChoice::OnKeyEvent(event);
        return true;
    }

    return false;
}

DEF_PAGE_2( pTune, // -V641 // -V1027
    "���������",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    &bMarkers,
    &bSettings,
    PageName::AutoMeasures_Tune, &PageAutoMeasures::self, IsActive_Tune, Page::OpenClose, Page::BeforeDraw, HandlerKey_Tune
)
