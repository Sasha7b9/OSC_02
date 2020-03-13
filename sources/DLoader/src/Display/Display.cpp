#include "defines.h"
#include "common/Decoder_d.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "common/Display/Font/Font_d.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/BufferButtons.h"
#include <cstring>


static void EmptyFunc() { }


// �������������� ������� ���������. ����������� ����� ����������� ���������, �� ����� ������� EndScene;
volatile static pFuncVV funcAdditionDraw = EmptyFunc;

// true ��������, ��� ���������� ������� ���������
static bool inStateDraw = false;


void Display::Init()
{
    for(uint8 i = 0; i < Color::NUMBER.value; i++)
    {
        Color(i).LoadValueRGB();
    }

    Painter::Init();

    LoadBrightness();
}


void Display::Update()
{

}


bool Display::InProcess()
{
    return inStateDraw;
}


void Display::LoadBrightness()
{
    HAL_BUS::Panel::Send(Command::Display_Brightness, 110);
}


bool Display::IsRun()
{
    return true;
}
