#include "defines.h"
#include "Recorder/Recorder_Display.h"
#include "Recorder/Recorder_Settings.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Menu/Menu.h"

#include "Recorder/Recorder_Storage.h"
#include "Hardware/Timer.h"
#include "FPGA/FPGA.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Recorder
{
namespace Display
{
    /// ���������� ������������� ���������
    void DrawSettings(int x, int y);
    /// ���������� ������
    static void DrawData();
};
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Display::Update()
{
    Painter::BeginScene(Color::BLACK);

    Grid::Draw();

    DrawData();

    DrawSettings(269, 0);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawSettings(int x, int y)
{
    Region(50, 30).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(RECORDER_SCALE_X.ToString()).Draw(x + 2, y + 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Y(int value)
{
    int delta = FPGA::VALUE::AVE - value;

    float scale = 120.0F / 125.0F;

    int y = 120 + (int)(delta * scale);

    if (y < 0)
    {
        y = 0;
    }
    if (y > 239)
    {
        y = 239;
    }

    return y;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawData()
{
    Storage::Frame *frame = Storage::CurrentFrame();

    uint numPoints = frame->NumPoints();

    if (numPoints == 0)
    {
        return;
    }

    Color::SetCurrent(Color::FILL);

    int x = 0;

    Storage::Point point = frame->GetPoint((numPoints < 320) ? (0) : (numPoints - 320), numPoints);

    do
    {
        if (!point.IsEmpty())
        {
            int min = Y(point.Min());
            int max = Y(point.Max());
            
            VLine(max - min).Draw(x, min);
        }

        point = frame->NextPoint(numPoints);
        x++;
    } while (x < 320);
}
