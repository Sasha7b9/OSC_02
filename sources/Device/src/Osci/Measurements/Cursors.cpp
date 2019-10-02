#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"
#include <cstring>
#include <cmath>


/// ���������� ������������ ������
static void DrawVertical(int x, int yTearing);
/// ���������� �������������� ������
static void DrawHorizontal(int y, int xTearing);

static void UpdateCursorsForLook();



float CursorsOsci::PosU(Chan::E ch, int numCur)
{
    return set.curs.posCurU[ch][numCur] / (Grid::Bottom() == Grid::FullBottom() ? 1.0F : 2.0F);
}


bool CursorsOsci::NecessaryDraw()
{
    return ((set.curs.cntrlU[set.curs.source] == CursorsControl::Disable) || (set.curs.cntrlT[set.curs.source] == CursorsControl::Disable)) &&
        (set.curs.showCursors || (Menu::OpenedItem() == PageCursorsMeasures::PageSet::self));
}


String CursorsOsci::Voltage(Chan::E source, int numCur)
{
    float voltage = FPGA::Math::VoltageCursor(PosU(source, numCur), set.ch[source].range, SET_RSHIFT(source));
    if (set.ch[source].divider == 1)
    {
        voltage *= 10.0F;
    }

    return ::Voltage(voltage).ToString(true);
}


String CursorsOsci::Time(Chan::E source, int numCur)
{
    float time = FPGA::Math::TimeCursor(CursorsOsci::PosT(source, numCur), set.time.base);

    return ::Time(time).ToString(true);
}


float CursorsOsci::PosT(Chan::E ch, int num)
{
    float retValue = 0.0F;
    std::memcpy(&retValue, &set.curs.posCurT[ch][num], sizeof(float));
    return retValue;
}


void CursorsOsci::SetCursPosT_temp(Chan::E ch, int num, float value)
{
    std::memcpy(&set.curs.posCurT[ch][num], &value, sizeof(float));
}


void CursorsOsci::Draw()
{
    Chan::E source = set.curs.source;

    Color::CHAN[source].SetAsCurrent();

    if (NecessaryDraw())
    {
        bool bothCursors = (set.curs.cntrlT[set.curs.source] == CursorsControl::Disable) &&
                           (set.curs.cntrlU[set.curs.source] == CursorsControl::Disable);  // ������� ����, ��� �������� � ������������ � �������������� ������� - ���� ���������� 
                                                                                           // �������� � ������ �����������

        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = Grid::Left() + (int)CursorsOsci::PosT(source, 0);
            x1 = Grid::Left() + (int)CursorsOsci::PosT(source, 1);
            y0 = Grid::Top() + (int)set.curs.posCurU[source][0];
            y1 = Grid::Top() + (int)set.curs.posCurU[source][1];

            Rectangle(4, 4).Draw(x0 - 2, y0 - 2);
            Rectangle(4, 4).Draw(x1 - 2, y1 - 2);
        }

        if (set.curs.cntrlT[set.curs.source] == CursorsControl::Disable)
        {
            DrawVertical((int)CursorsOsci::PosT(source, 0), y0);
            DrawVertical((int)CursorsOsci::PosT(source, 1), y1);
        }
        if (set.curs.cntrlU[set.curs.source] == CursorsControl::Disable)
        {
            DrawHorizontal((int)set.curs.posCurU[source][0], x0);
            DrawHorizontal((int)set.curs.posCurU[source][1], x1);
        }

        UpdateCursorsForLook();
    }
}


static void DrawVertical(int x, int yTearing)
{
    x += Grid::Left();
    if (yTearing == -1)
    {
        DashedVLine(Grid::ChannelBottom() - Grid::Top() - 3, 1, 1, 0).Draw(x, Grid::Top() + 2);
    }
    else
    {
        DashedVLine(yTearing - Grid::Top() - 4, 1, 1, 0).Draw(x, Grid::Top() + 2);
        DashedVLine(Grid::ChannelBottom() - yTearing - 3, 1, 1, 0).Draw(x, yTearing + 2);
    }
    Rectangle(2, 2).Draw(x - 1, Grid::Top() - 1);
    Rectangle(2, 2).Draw(x - 1, Grid::ChannelBottom() - 1);
}


static void DrawHorizontal(int y, int xTearing)
{
    y += Grid::Top();
    if (xTearing == -1)
    {
        DashedHLine(Grid::Right() - Grid::Left() - 3, 1, 1, 0).Draw(Grid::Left() + 2, y);
    }
    else
    {
        DashedHLine(xTearing - Grid::Left() - 4, 1, 1, 0).Draw(Grid::Left() + 2, y);
        DashedHLine(Grid::Right() - xTearing - 3, 1, 1, 0).Draw(xTearing + 2, y);
    }
    Rectangle(2, 2).Draw(Grid::Left() - 1, y - 1);
    Rectangle(2, 2).Draw(Grid::Right() - 1, y - 1);
}


static void UpdateCursorsForLook()
{
//    Chan::E source = CURS_SOURCE;

    if ((set.curs.active == CursorsActive::T) && ((set.curs.lookMode[Chan::A] == CursorsLookMode::Voltage) || (set.curs.lookMode[Chan::A] == CursorsLookMode::Both)))
    {
        //SetCursorU(source, 0, Processing::CalculateCursorU(source, CURsT_POS(source, 0)));
    }
    if ((set.curs.active == CursorsActive::T) && ((set.curs.lookMode[Chan::B] == CursorsLookMode::Voltage) || (set.curs.lookMode[Chan::B] == CursorsLookMode::Both)))
    {
        //SetCursorU(source, 1, Processing::CalculateCursorU(source, CURsT_POS(source, 1)));
    }
    if ((set.curs.active == CursorsActive::U) && ((set.curs.lookMode[Chan::A] == CursorsLookMode::Time) || (set.curs.lookMode[Chan::A] == CursorsLookMode::Both)))
    {
        //SetCursorT(source, 0, Processing::CalculateCursorT(source, CURsU_POS(source, 0), 0));
    }
    if ((set.curs.active == CursorsActive::U) && ((set.curs.lookMode[Chan::B] == CursorsLookMode::Time) || (set.curs.lookMode[Chan::B] == CursorsLookMode::Both)))
    {
        //SetCursorT(source, 1, Processing::CalculateCursorT(source, CURsU_POS(source, 1), 1));
    }
}


String CursorsOsci::PercentsU(Chan::E source)
{
    /// \todo ��� ����� ����. ��� ������� �������� HardFault. ��������, ��-�� ����������� ������������ Settings. ���� ��������
    // float dPerc = dUperc(source);     
    float dPerc = 100.0F;
    std::memcpy(&dPerc, &set.curs.deltaU100percents[source], sizeof(float));

    float dValue = std::fabsf(PosU(source, 0) - PosU(source, 1));
    return String("%s%%", Float(dValue / dPerc * 100.0F).ToString(false, 5).CString());
}


String CursorsOsci::PercentsT(Chan::E source)
{
    float dPerc = 100.0F;
    std::memcpy(&dPerc, &set.curs.deltaT100percents[source], sizeof(float));

    float dValue = std::fabsf(PosT(source, 0) - PosT(source, 1));
    return String("%s%%", Float(dValue / dPerc * 100.0F).ToString(false, 6).CString());
}
