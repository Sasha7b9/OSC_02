#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "MemoryWindow.h"
#include "Data/DataStorage.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::Rectangle;
using Display::Region;
using Display::VLine;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MemoryWindow::Draw()
{
    if (Cursors::NecessaryDraw())
    {
        return;
    }

    DrawScreenArea();

    Chan::E chans[2] = {LAST_AFFECTED_CH_IS_A ? Chan::B : Chan::A, LAST_AFFECTED_CH_IS_A ? Chan::A : Chan::B};

    for (int i = 0; i < 2; i++)
    {
        Chan::E chan = chans[i];
        if (SET_ENABLED(chan))
        {
            Painter::SetColor(Color::Channel(chan));
            DrawDataInRect(X(), Y(), Width(), Height(), OUT(chan), (uint)FPGA::NumPoints());
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryWindow::DrawDataInRect(int x, int y, int width, int height, const uint8 *data, uint length)
{
    int numIntervals = width + 1;            // ���������� ����������, � ������� ����� �������� ��� ������ - ����������, ���������� ������������ �����
    float pointsInInterval = (float)length / numIntervals;   // ���������� �����, �������� � ����� ���������.

    float stepY = (float)height / (float)(MAX_VALUE - MIN_VALUE);

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

        //Painter::DrawVLine(x + i, y0 - deltaMIN, y0 - deltaMAX);
        VLine(deltaMIN - deltaMAX).Draw(x + i, y0 - deltaMIN);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryWindow::DrawScreenArea()
{
    float relBegin = (float)SHIFT_IN_MEMORY / FPGA::NumPoints();          // ������������� ������ �������� ������� ������������ ���� ������
    float relWidth = (float)(Grid::Width() + 1) / FPGA::NumPoints();      // ������������� (������������ ���� ������� ����) ������ ������� �������

    int begin = (int)(relBegin * Width() + 0.5F);   // �������� ������ �������� ������� ������������ ������ ���� ������
    int width = (int)(relWidth * Width() + 0.5F);   // �������� ������ ������� �������

    Region(width, Height()).Draw(X() + begin, Y(), Color::GRAY_20);

    Rectangle(width, Height()).Draw(X() + begin, Y(), Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryWindow::Width()
{
    return 278;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryWindow::Height()
{
    return Grid::Top() - 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryWindow::X()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryWindow::Y()
{
    return 0;
}
