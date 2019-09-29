#include "defines.h"
#include "Cursors.h"
#include "Display/Colors.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FPGA/FPGA_Math.h"
#include "Menu/Menu.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstring>


using namespace Display::Primitives;
using namespace FPGA::Math;
using namespace Osci::Measurements;
using Osci::Measurements::Cursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� ������������ ������
static void DrawVertical(int x, int yTearing);
/// ���������� �������������� ������
static void DrawHorizontal(int y, int xTearing);

static void UpdateCursorsForLook();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Cursors::PosU(Chan::E ch, int numCur)
{
    return set.curs.posCurU[ch][numCur] / (Grid::Bottom() == Grid::FullBottom() ? 1.0F : 2.0F);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cursors::NecessaryDraw()
{
    return (CURsU_ENABLED || CURsT_ENABLED) && (set.curs.showCursors || Menu::GetNameOpenedPage() == PageName::Measures_Cursors_Set);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::Voltage(Chan::E source, int numCur)
{
    float voltage = FPGA::Math::VoltageCursor(PosU(source, numCur), set.ch[source].range, SET_RSHIFT(source));
    if (set.ch[source].divider == 1)
    {
        voltage *= 10.0F;
    }

    return ::Voltage(voltage).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::Time(Chan::E source, int numCur)
{
    float time = FPGA::Math::TimeCursor(Cursors::PosT(source, numCur), SET_TBASE);

    return ::Time(time).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Cursors::PosT(Chan::E ch, int num)
{
    float retValue = 0.0F;
    std::memcpy(&retValue, &set.curs.posCurT[ch][num], sizeof(float));
    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Cursors::SetCursPosT_temp(Chan::E ch, int num, float value)
{
    std::memcpy(&set.curs.posCurT[ch][num], &value, sizeof(float));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Cursors::Draw()
{
    Chan::E source = set.curs.source;

    Color::CHAN[source].SetAsCurrent();

    if (NecessaryDraw())
    {
        bool bothCursors = CURsT_ENABLED && CURsU_ENABLED;  // ������� ����, ��� �������� � ������������ � �������������� ������� - ���� ���������� 
                                                            // �������� � ������ �����������

        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = Grid::Left() + (int)Cursors::PosT(source, 0);
            x1 = Grid::Left() + (int)Cursors::PosT(source, 1);
            y0 = Grid::Top() + (int)set.curs.posCurU[source][0];
            y1 = Grid::Top() + (int)set.curs.posCurU[source][1];

            Rectangle(4, 4).Draw(x0 - 2, y0 - 2);
            Rectangle(4, 4).Draw(x1 - 2, y1 - 2);
        }

        if (CURsT_ENABLED)
        {
            DrawVertical((int)Cursors::PosT(source, 0), y0);
            DrawVertical((int)Cursors::PosT(source, 1), y1);
        }
        if (CURsU_ENABLED)
        {
            DrawHorizontal((int)set.curs.posCurU[source][0], x0);
            DrawHorizontal((int)set.curs.posCurU[source][1], x1);
        }

        UpdateCursorsForLook();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void UpdateCursorsForLook()
{
//    Chan::E source = CURS_SOURCE;

    if ((set.curs.active == CursorsActive::T) && (CURS_LOOK_U(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        //SetCursorU(source, 0, Processing::CalculateCursorU(source, CURsT_POS(source, 0)));
    }
    if ((set.curs.active == CursorsActive::T) && (CURS_LOOK_U(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        //SetCursorU(source, 1, Processing::CalculateCursorU(source, CURsT_POS(source, 1)));
    }
    if ((set.curs.active == CursorsActive::U) && (CURS_LOOK_T(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        //SetCursorT(source, 0, Processing::CalculateCursorT(source, CURsU_POS(source, 0), 0));
    }
    if ((set.curs.active == CursorsActive::U) && (CURS_LOOK_T(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        //SetCursorT(source, 1, Processing::CalculateCursorT(source, CURsU_POS(source, 1), 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::PercentsU(Chan::E source)
{
    /// \todo ��� ����� ����. ��� ������� �������� HardFault. ��������, ��-�� ����������� ������������ Settings. ���� ��������
    // float dPerc = dUperc(source);     
    float dPerc = 100.0F;
    std::memcpy(&dPerc, &dUperc(source), sizeof(float));

    float dValue = std::fabsf(PosU(source, 0) - PosU(source, 1));
    return String("%s%%", Float(dValue / dPerc * 100.0F).ToString(false, 5).CString());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::PercentsT(Chan::E source)
{
    float dPerc = 100.0F;
    std::memcpy(&dPerc, &dTperc(source), sizeof(float));

    float dValue = std::fabsf(Cursors::PosT(source, 0) - Cursors::PosT(source, 1));
    return String("%s%%", Float(dValue / dPerc * 100.0F).ToString(false, 6).CString());
}
