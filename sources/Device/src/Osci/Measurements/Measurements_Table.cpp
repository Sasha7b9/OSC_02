#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Osci/Measurements/Measurements_Table.h"
#include "Settings/Settings.h"

#include "Utils/Math.h"
#include "Display/Font/Font.h"
#include "Osci/Measurements/Measurements.h"


using namespace Osci;
using namespace Osci::Measurements;
using namespace Osci::Measurements::Settings;

using Display::Primitives::Char;
using Display::Primitives::Rectangle;
using Display::Primitives::Region;
using Display::Primitives::Text;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������� ���������� y ��������� ������� �������������� ���������
static int top = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int GetTopTable();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Table::Draw()
{
    if (!SHOW_MEASURES)
    {
        return;
    }

    top = Grid::Bottom();

    CalculateMeasures();

    int x0 = Grid::Left() - GetDeltaGridLeft();
    int dX = DX();
    int dY = DY();
    int y0 = GetTopTable();

    for (int str = 0; str < NumRows(); str++)
    {
        for (int elem = 0; elem < NumCols(); elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            Measure measure(str, elem);

            bool active = measure.IsActive() && Menu::GetNameOpenedPage() == Page::Name::Measures_Auto_Tune;

            Measure::Type::E type = measure.GetType();
            if (type != Measure::Type::None)
            {
                Region(dX, dY).Fill(x, y, Color::BACK);
                Rectangle(dX, dY).Draw(x, y, Color::FILL);
                top = Math::Min(top, y);
            }
            if (active)
            {
                Region(dX - 4, dY - 4).Fill(x + 2, y + 2, Color::FILL);
            }
            if (type != Measure::Type::None)
            {
                Color color = active ? Color::BACK : Color::FILL;

#define SIZE_BUFFER 20
                char buffer[SIZE_BUFFER];

                measure.Name().Draw(x + 4, y + 2, color);

                if (type == MEAS_MARKED)
                {
                    Region(dX - 2, 9).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
                    measure.Name().Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
                }
                if (VIEW_MEASURES_BOTH)
                {
                    GetStringMeasure(type, Chan::A, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::A));
                    GetStringMeasure(type, Chan::B, buffer, SIZE_BUFFER).Draw(x + 2, y + (SET_ENABLED_A ? 20 : 11), Color::Channel(Chan::B));
                }
                else if (VIEW_MEASURES_A)
                {
                    GetStringMeasure(type, Chan::A, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::A));
                }
                else if (VIEW_MEASURES_B)
                {
                    GetStringMeasure(type, Chan::B, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::B));
                }
                else
                {
                    // ������ ��������� ���
                }
            }
        }
    }

    if (Menu::GetNameOpenedPage() == Page::Name::Measures_Auto_Tune)
    {
        PageChoice::Draw();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int GetTopTable()
{
    if (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2)
    {
        return Grid::Bottom() - Osci::Measurements::Table::DY() * 6;
    }

    int y = Grid::Bottom() - Table::NumRows() * Osci::Measurements::Table::DY();

    if (Menu::IsShown())
    {
        y -= 3;
    }

    return y;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Table::NumCols()
{
    const int cols[] = { 1, 2, 5, 5, 5, 1, 2 };
    return cols[NUM_MEASURES];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Table::NumRows()
{
    int rows[] = { 1, 1, 1, 2, 3, 6, 6 };
    return rows[NUM_MEASURES];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Table::GetDeltaGridLeft()
{
    if (SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_6_1)
        {
            return DX();
        }
        else if (NUM_MEASURES_IS_6_2)
        {
            return DX() * 2;
        }
        else
        {
            // ������ ��������� ���
        }
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Table::DY()
{
    if (VIEW_MEASURES_BOTH)
    {
        return 30;
    }
    return 21;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Table::DX()
{
    return Grid::Width() / 5;
}
