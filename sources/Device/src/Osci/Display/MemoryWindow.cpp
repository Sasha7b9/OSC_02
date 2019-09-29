#include "defines.h"
#include "MemoryWindow.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Hardware/Battery.h"
#include "Osci/Display/HiPart.h"
#include "Display/Display.h"
#include "Osci/Display/PainterData.h"
#include "Osci/Osci_Settings.h"
#include "Osci/Measurements/Measurements.h"


using namespace Display::Primitives;
using namespace FPGA;
using namespace Osci::Measurements;

using Osci::Display::MemoryWindow::Width;
using Osci::Display::MemoryWindow::Height;
using Osci::Display::MemoryWindow::X;
using Osci::Display::MemoryWindow::Y;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, uint length);
/// ���������� ������� ������
static void DrawScreenArea();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::MemoryWindow::Draw()
{
    if (Cursors::NecessaryDraw())
    {
        return;
    }

    DrawScreenArea();

    Chan::E chans[2] = { (set.disp.lastAffectedChannel == Chan::A) ? Chan::B : Chan::A, (set.disp.lastAffectedChannel == Chan::A) ? Chan::A : Chan::B };

    for (int i = 0; i < 2; i++)
    {
        Chan::E chan = chans[i];
        if (set.ch[chan].enabled)
        {
            Color::CHAN[chan].SetAsCurrent();
            DrawDataInRect(X(), Y(), Width(), Height(), OUT(chan), FPGA_NUM_POINTS);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, uint length)
{
    if (set.time.peakDet == PeakDetMode::Enabled)
    {
        length *= 2;
    }

    int numIntervals = width + 1;                               // ���������� ����������, � ������� ����� �������� ��� ������ - ����������, ���������� ������������ �����
    float pointsInInterval = (float)length / numIntervals;      // ���������� �����, �������� � ����� ���������.

    float stepY = (float)height / (float)(VALUE::MAX - VALUE::MIN);

    int y0 = y + height;

    for (int i = 0; i < numIntervals - 1; i++)
    {
        int start = (int)(i * pointsInInterval + 0.5F);          // ��������� ����� � ���������
        int end = (int)((i + 1) * pointsInInterval + 0.5F) - 1;  // �������� ����� � ���������

        int min = 255;
        int max = 0;

        for (int j = start; j <= end; j++)
        {
            if (data[j] < min)
            {
                min = data[j];
            }
            if (data[j] > max)
            {
                max = data[j];
            }
        }

        int deltaMIN = (int)(min * stepY + 0.5F);
        int deltaMAX = (int)(max * stepY + 0.5F);

        VLine(deltaMIN - deltaMAX).Draw(x + i, y0 - deltaMIN);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawScreenArea()
{
    float relBegin = (float)SHIFT_IN_MEMORY / FPGA_NUM_POINTS;          // ������������� ������ �������� ������� ������������ ���� ������
    float relWidth = (float)(Grid::Width() + 1) / FPGA_NUM_POINTS;      // ������������� (������������ ���� ������� ����) ������ ������� �������

    int begin = (int)(relBegin * Width() + 0.5F);   // �������� ������ �������� ������� ������������ ������ ���� ������
    int width = (int)(relWidth * Width() + 0.5F);   // �������� ������ ������� �������

    Region(width, Height()).Fill(X() + begin, Y(), Color::GRAY_20);

    Rectangle(width, Height()).Draw(X() + begin, Y(), Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Display::MemoryWindow::Width()
{
    return 270;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Display::MemoryWindow::Height()
{
    return 17;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Display::MemoryWindow::X()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Display::MemoryWindow::Y()
{
    return ::Display::HEIGHT - 19;
}
