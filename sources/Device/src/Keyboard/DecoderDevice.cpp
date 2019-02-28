#include "defines.h"
#include "log.h"
#include "DecoderDevice.h"
#include "Menu/Menu.h"

#include "Utils/Debug.h"
#include "Display/Painter.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Decoder decoder;

#define SIZE_BUFFER 1024
static uint8 buffer[SIZE_BUFFER];
static int pointer = 0;

/// ����������� �������
static pFuncBU8 curFunc;
/// ������� ���� ����������� �������
static int step;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void RunStep(uint8 data);

static bool ButtonPress(uint8);

static bool FuncScreen(uint8);
/// ��� ������� ���� �������� ����� ���������� ���������� ����
static void FinishCommand();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Decoder::AddData(uint8 data)
{
    if (pointer < SIZE_BUFFER)
    {
        buffer[pointer++] = data;
    }
    else
    {
        LOG_WRITE("��� ����� � ������� ������");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Decoder::Data()
{
    return &buffer[0];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::ClearBuffer()
{
    pointer = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::Update()
{
    if (pointer)
    {
        for (int i = 0; i < pointer; i++)
        {
            RunStep(buffer[i]);
        }
        pointer = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Decoder::BytesInBuffer()
{
    return pointer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RunStep(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    commands[Command::Size] =
    {
        EmptyFuncBtU8,      // None,
        ButtonPress,        // ButtonPress,
        EmptyFuncBtU8,      // Paint_BeginScene,
        EmptyFuncBtU8,      // Paint_EndScene,
        EmptyFuncBtU8,      // Paint_SetColor,
        EmptyFuncBtU8,      // Paint_FillRegion,
        EmptyFuncBtU8,      // Paint_DrawText,
        EmptyFuncBtU8,      // Paint_SetPalette,
        EmptyFuncBtU8,      // Paint_DrawRectangle,
        EmptyFuncBtU8,      // Paint_DrawVLine,
        EmptyFuncBtU8,      // Paint_DrawHLine,
        EmptyFuncBtU8,      // Paint_SetFont,
        EmptyFuncBtU8,      // Paint_SetPoint,
        EmptyFuncBtU8,      // Paint_DrawLine,
        EmptyFuncBtU8,      // Paint_TesterLines,
        EmptyFuncBtU8,      // Paint_DrawBigText,
        FuncScreen,         // Screen
        EmptyFuncBtU8,      // Paint_VPointLine
        EmptyFuncBtU8       // Paint_HPointLine
    };

    if (step == 0)
    {
        if (data < Command::Size)
        {
            curFunc = commands[data].func;
            if (curFunc == 0)
            {
                LOG_ERROR("��� �����������");
            }
        }
        else
        {
            FinishCommand();
            return;
        }
    }

    if (curFunc)
    {
        if (curFunc(data))
        {
            FinishCommand();
        }
        else
        {
            ++step;
        }
    }
    else
    {
        FinishCommand();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ButtonPress(uint8 data)
{
    static Key::E button;
    if (step == 0)
    {
        return false;
    }
    else if (step == 1)
    {
        button = (Key::E)data;
        return false;
    }
    else
    {
        Menu::ButtonEvent(KeyEvent(button, (TypePress::E)data));
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncScreen(uint8 data)
{
    static int numString = 0;

    /// \todo volatile ������, ��� ���������� �������������� �� ��������������� ����� ������-��
    volatile static uint8 pixels[320];

    if (step == 0)
    {
        return false;
    }

    if (step == 1)
    {
        numString = data;
        return false;
    }

    if (step < 321)
    {
        pixels[step - 2] = data;
        return false;
    }

    if (step == 321)
    {
        Painter::SaveRow(numString);
    }


    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}
