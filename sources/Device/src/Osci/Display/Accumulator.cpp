#include "defines.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"



// ���������� ������������ ��������
static int numFrames = 0;



// ���������� ����� ���������� �������
static uint CurrentFrame();



void DisplayOsci::Accumulator::NextFrame()
{
    if (set.disp.modeAccumulation == ModeAccumulation::Reset)
    {
        static uint prevFrame = 0;

        if (CurrentFrame() == prevFrame)
        {
            return;
        }

        prevFrame = CurrentFrame();

        numFrames++;
        if (numFrames >= ENumAccum::Number())
        {
            DisplayOsci::SetFlagRedraw();
            numFrames = 0;
        }
    }
    else
    {
        DisplayOsci::SetFlagRedraw();
    }
}


void DisplayOsci::Accumulator::Reset()
{
    numFrames = 0;
}


static uint CurrentFrame()
{
    return (DS) ? DS->id : 0;
}
