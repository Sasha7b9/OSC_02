#include "defines.h"
#include "log.h"
#include "DecoderLoader.h"
#include "Display/Painter.h"
#include "Keyboard/BufferButtons.h"

#include <cstdlib>



Decoder decoder;

#define SIZE_BUFFER 1024
static uint8 buffer[SIZE_BUFFER];
static int pointer = 0;

/// ����������� �������
static pFuncBU8 curFunc;
/// ������� ���� ����������� �������
static int step;

static void RunStep(uint8 data);

static bool ButtonPress(uint8);

static bool FuncLengthText(uint8);

/// ��� ������� ���� �������� ����� ���������� ���������� ����
static void FinishCommand();
/// ��������� ��������� ������ � �������
static bool AddToConsole(uint8);



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


static bool EmptyFunc(uint8)
{
    return true;
}


static void RunStep(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    commands[Command::Count] =
    {
        EmptyFunc,      // None,
        ButtonPress,    // ButtonPress,
        EmptyFunc,      // Paint_BeginScene,
        EmptyFunc,      // Paint_EndScene,
        EmptyFunc,      // Paint_SetColor,
        EmptyFunc,      // Paint_FillRegion,
        EmptyFunc,      // Paint_DrawText,
        EmptyFunc,      // Paint_SetPalette,
        EmptyFunc,      // Paint_DrawRectangle,
        EmptyFunc,      // Paint_DrawVLine,
        EmptyFunc,      // Paint_DrawHLine,
        EmptyFunc,      // Paint_SetFont,
        EmptyFunc,      // Paint_SetPoint,
        EmptyFunc,      // Paint_DrawLine,
        EmptyFunc,      // Paint_TesterLines,
        EmptyFunc,      // Paint_DrawBigText,
        EmptyFunc,      // Screen
        EmptyFunc,      // Paint_VPointLine
        EmptyFunc,      // Paint_HPointLine
        EmptyFunc,      // Paint_SetMonoSpaceFont
        EmptyFunc,      // Paint_SetTextSpacing
        AddToConsole,   // AddToConsole
        FuncLengthText  // Text_Length
    };

    if (step == 0)
    {
        if (data < Command::Count)
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


static bool ButtonPress(uint8 data)
{
    static Key::E button;
    if (step == 0)
    {
        return false;
    }
    else if (step == 1)
    {
        button = static_cast<Key::E>(data);
        return false;
    }
    else
    {
        BufferButtons::Push(KeyEvent(button, static_cast<TypePress::E>(data)));
    }

    return true;
}


void Decoder::SetBufferForScreenRow(uint8 *)
{
}


static bool FuncLengthText(uint8 data)
{
    if(step == 0)
    {
        return false;
    }

    if(step == 1)
    {
        Font::SetLength(data);
    }

    return true;
}


static bool AddToConsole(uint8 data)
{
    static char *text = nullptr;        // ����� ����� ��������� �������� ������

    static uint8 allSymbols = 0;        // ���������� �������� � ������ ��� ����� ������������ ����

    static uint8 recvSymbols = 0;       // ������� �������� ��� �������

    if (step == 0)
    {
        return false;
    }

    if (step == 1)
    {
        allSymbols = data;
        text = static_cast<char *>(std::malloc(allSymbols + 1U));
        recvSymbols = 0;
        return false;
    }

    text[recvSymbols++] = static_cast<char>(data);

    if (recvSymbols == allSymbols)
    {
        LOG_WRITE(text);
        std::free(text);
    }

    return (recvSymbols == allSymbols);
}


static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}
