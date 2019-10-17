#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"


static bool isActive = false;
/// ������� �������
static int8 posCursor = 0;


void PageChoiceMeasures::ChangeState()
{
    isActive = !isActive;
}


void PageChoiceMeasures::OnOpenCloseEvent()
{
    ChangeState();
    if (isActive)
    {
        posCursor = (int8)set.meas.measures[AutoMeasurements::posActive];
    }
}


void PageChoiceMeasures::OnKeyEvent(const KeyEvent &event)
{
    if (event.type != TypePress::Press)
    {
        return;
    }

    Key::E key = event.key;

    int8 delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    if (isActive)
    {
        posCursor += delta;
        Beeper::RegulatorSwitchRotate();

        if (posCursor < 0)
        {
            posCursor = TypeMeasure::Count - 1;
        }
        else if (posCursor == TypeMeasure::Count)
        {
            posCursor = 0;
        }
        else
        {
            // ����� ������ ������ �� �����
        }

        set.meas.measures[AutoMeasurements::posActive] = (TypeMeasure::E)posCursor;
        Color::ResetFlash();
    }
    else
    {
        Measure::ChangeActive(delta);

        Beeper::RegulatorSwitchRotate();
    }
}


void PageChoiceMeasures::Draw()
{
    if (!isActive)
    {
        return;
    }
    int x = ((set.meas.number == MeasuresOnDisplay::_6_1) || (set.meas.number == MeasuresOnDisplay::_6_2)) ? (Grid::Right() - 3 * Grid::Width() / 5) : Grid::Left();
    int y = Grid::Top();
    int dX = Grid::Width() / 5;
    int dY = 22;
    int maxRow = ((set.meas.number == MeasuresOnDisplay::_6_1) || (set.meas.number == MeasuresOnDisplay::_6_2)) ? 8 : 5;
    int maxCol = ((set.meas.number == MeasuresOnDisplay::_6_1) || (set.meas.number == MeasuresOnDisplay::_6_2)) ? 3 : 5;
    TypeMeasure::E meas = TypeMeasure::None;

    for (int row = 0; row < maxRow; row++)
    {
        for (int col = 0; col < maxCol; col++)
        {
            if (meas < TypeMeasure::Count)
            {
                int x0 = x + col * dX;
                int y0 = y + row * dY;
                bool active = (meas == posCursor);
                Rectangle(dX, dY).Draw(x0, y0, Color::WHITE);
                Region(dX - 2, dY - 2).Fill(x0 + 1, y0 + 1, (active ? Color::FLASH_10 : Color::BACK));
                Char((SymbolUGO::E)Measure::GetChar(meas)).Draw10SymbolsInRect(x0 + 2, y0 + 1, active ? Color::FLASH_01 : Color::FILL);
                Font::SetCurrent(TypeFont::_5);
                Text(Measure::GetName(meas)).DrawRelativelyRight(x0 + dX, y0 + 12);
                meas = (TypeMeasure::E)(static_cast<int>(meas) + 1);    // meas++;
            }
        }
    }

    Font::SetCurrent(TypeFont::_8);
}
