#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


// ������� ���������� y ��������� ������� �������������� ���������
static int top = 0;



static int GetTopTable();



void TableMeasures::Draw()
{
    if (!set.meas.show)
    {
        return;
    }

    DFont::Set(DTypeFont::_8);

    top = Grid::Bottom();

    AutoMeasurements::CalculateMeasures();

    for (int str = 0; str < NumRows(); str++)
    {
        for (int elem = 0; elem < NumCols(); elem++)
        {
            int x = Grid::Left() - GetDeltaGridLeft() + DX() * elem;
            int y = GetTopTable() + str * DY();

            Cell(str, elem).Draw(x, y);
        }
    }

    if(Menu::OpenedItem() == PageAutoMeasures::Tune::self)
    {
        PageChoiceMeasures::Draw();
    }
}


void TableMeasures::Cell::Draw(int x, int y)
{
    Measure measure(row, col);

    bool active = measure.IsActive() && (Menu::OpenedItem() == PageAutoMeasures::Tune::self);

    TypeMeasure::E type = measure.GetType();

    if (type != TypeMeasure::None)
    {
        Region(DX(), DY()).Fill(x, y, Color::BACK);
        Rectangle(DX(), DY()).Draw(x, y, Color::FILL);
        top = Math::Min(top, y);
    }

    if (active)
    {
        Region(DX() - 4, DY() - 4).Fill(x + 2, y + 2, Color::FILL);
    }

    if (type != TypeMeasure::None)
    {
        Color color = active ? Color::BACK : Color::FILL;

        measure.Name().Draw(x + 4, y + 2, color);

        if (type == set.meas.marked)
        {
            Region(DX() - 2, 9).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
            measure.Name().Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
        }

        DrawStringMeasure(x, y);   
    }
}


void TableMeasures::Cell::DrawStringMeasure(int x, int y)
{
    Measure measure(row, col);

    static const int SIZE_BUFFER = 20;

    char buffer[SIZE_BUFFER];

    String measureA = measure.GetStringMeasure(ChanA, buffer, SIZE_BUFFER);
    String measureB = measure.GetStringMeasure(ChanB, buffer, SIZE_BUFFER);

    Color colA = Color::CHAN[ChanA];
    Color colB = Color::CHAN[ChanB];

    if (MeasuresSource::IsBoth())
    {
        measureA.Draw(x + 2, y + 11, colA);
        measureB.Draw(x + 2, y + (setA.enabled ? 20 : 11), colB); //-V547
    }
    else if (MeasuresSource::IsA())
    {
        measureA.Draw(x + 2, y + 11, colA);
    }
    else if (MeasuresSource::IsB())
    {
        measureB.Draw(x + 2, y + 11, colB);
    }
    else
    {
        // ������ ��������� ���
    }
}


static int GetTopTable()
{
    if ((MeasuresOnDisplay() == MeasuresOnDisplay::_6_1) || (MeasuresOnDisplay() == MeasuresOnDisplay::_6_2))
    {
        return Grid::Bottom() - TableMeasures::DY() * 6;
    }

    int y = Grid::Bottom() - TableMeasures::NumRows() * TableMeasures::DY();

    if (Menu::IsShown())
    {
        y -= 3;
    }

    return y;
}


int TableMeasures::NumCols()
{
    static const int cols[] = { 1, 2, 5, 5, 5, 1, 2 };
    return cols[MeasuresOnDisplay()];
}


int TableMeasures::NumRows()
{
    static const int rows[] = { 1, 1, 1, 2, 3, 6, 6 };
    return rows[MeasuresOnDisplay()];
}


int TableMeasures::GetDeltaGridLeft()
{
    if (set.meas.show && MeasuresModeViewSignals::IsCompress())
    {
        if (MeasuresOnDisplay() == MeasuresOnDisplay::_6_1)
        {
            return DX();
        }
        else if (MeasuresOnDisplay() == MeasuresOnDisplay::_6_2)
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


int TableMeasures::DY()
{
    if (MeasuresSource::IsBoth())
    {
        return 30;
    }
    return 21;
}


int TableMeasures::DX()
{
    return Grid::Width() / 5;
}
