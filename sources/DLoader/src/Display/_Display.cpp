#include "defines.h"
#include <ff.h>
#include "Display.h"
#include "Display/Painter.h"
#include "main.h"
#include "common/Display/Font/Font_d.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include <cmath>
#include <cstdlib>



enum TypeWelcomeScreen
{
    TypeWelcomeScreen_Vague,
    TypeWelcomeScreen_Wave,
    TypeWelcomeScreen_VagueWave
};




static void DrawProgressBar(uint dT);
static void DrawBigMNIPI();
static int RandValue(int min, int max);
static void InitPoints();

struct Vector
{
    uint16 x;
    uint8  y;
    uint8  notUsed;
};

#ifdef MSVC
#define __attribute__(x)
#endif

static int numPoints = 0;
static Vector array[7000] __attribute__ ((section("CCM_DATA")));

static TypeWelcomeScreen typeScreen = TypeWelcomeScreen_Wave;

#define VAGUE (typeScreen == TypeWelcomeScreen_Vague)
#define WAVE (typeScreen == TypeWelcomeScreen_Wave)
#define ALL (typeScreen == TypeWelcomeScreen_VagueWave)
#define VAGUE_OR_ALL (VAGUE || ALL)
#define WAVE_OR_ALL (WAVE || ALL)


void Display::Init()
{
    ms->display.value = 0.0F;
    ms->display.isRun = false;
    ms->display.timePrev = 0;
    ms->display.direction = 10.0F;

    Color::InitGlobalColors();

    for (int i = 0; i < 14; i++)
    {
        float red = i / 14.0F * 31.0F + 0.5F;
        float green = i / 14.0F * 63.0F + 0.5F;
        float blue = i / 14.0F * 31.0F + 0.5F;
        set.display.colors[i + 2] = static_cast<uint16>(MAKE_COLOR(red, green, blue));
    }

    Painter::ResetFlash();

    Painter::LoadPalette();

    Painter::LoadFont(DTypeFont::_8);

    DFont::Set(DTypeFont::_8);

    InitPoints();
}



static void DrawButton(int x, int y, const char *text)
{
    int width = 25;
    int height = 20;
    Painter::DrawRectangle(x, y, width, height);
    Painter::DrawStringInCenterRect(x, y, width + 2, height - 1, text);
}


void Display::Update()
{
    ms->display.isRun = true;

    uint dT = TIME_MS - ms->display.timePrev;
    ms->display.timePrev = TIME_MS;

    Painter::BeginScene(Color::BLACK);

    Painter::SetColor(Color::WHITE);

    if (ms->state == State_Start || ms->state == State_Ok)
    {
        Painter::BeginScene(Color::BACK);
        Painter::DrawRectangle(0, 0, 319, 239, Color::FILL);
        DrawBigMNIPI();
        Painter::DrawStringInCenterRect(0, 180, 320, 20, "��� ��������� ������ ������� � ����������� ������ ������", Color::WHITE);
        Painter::DrawStringInCenterRect(0, 205, 320, 20, "����� ����������: ���./����. 8-017-237-23-40");
        Painter::DrawStringInCenterRect(0, 220, 320, 20, "������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-237-22-15");
        Painter::EndScene();
    }
    else if (ms->state == State_Mount)
    {
        DrawProgressBar(dT);
    }
    else if (ms->state == State_WrongFlash)
    {
        Painter::DrawStringInCenterRect(0, 0, 320, 200, "�� ������� ��������� ����", Color::FLASH_10);
        Painter::DrawStringInCenterRect(0, 20, 320, 200, "���������, ��� �������� ������� FAT32", Color::WHITE);
    }
    else if (ms->state == State_RequestAction)
    {
        Painter::DrawStringInCenterRect(0, 0, 320, 200, "���������� ����������� �����������");
        Painter::DrawStringInCenterRect(0, 20, 320, 200, "���������� ���?");

        DrawButton(290, 55, "��");
        DrawButton(290, 195, "���");
    }
    else if (ms->state == State_Upgrade)
    {
        Painter::DrawStringInCenterRect(0, 0, 320, 190, "��������� ����������");
        Painter::DrawStringInCenterRect(0, 0, 320, 220, "��������� ������������ �����������");

        int height = 30;
        int fullWidth = 280;
        int width = static_cast<int>(fullWidth * ms->percentUpdate);

        Painter::FillRegion(20, 130, width, height);
        Painter::DrawRectangle(20, 130, fullWidth, height);
    }
    else
    {
        // ������ ���
    }

    Painter::EndScene();
    ms->display.isRun = false;
}



void DrawProgressBar(uint dT)
{
    const int WIDTH = 300;
    const int HEIGHT = 20;
    const int X = 10;
    const int Y = 200;
    
    float step = dT / ms->display.direction;

    ms->display.value += step;

    if (ms->display.direction > 0.0F && ms->display.value > WIDTH)
    {
        ms->display.direction = -ms->display.direction;
        ms->display.value -= step;
    }
    else if (ms->display.direction < 0.0F && ms->display.value < 0)
    {
        ms->display.direction = -ms->display.direction;
        ms->display.value -= step;
    }
    else
    {
        // ������ ���
    }

    int dH = 15;
    int y0 = 50;

    Painter::DrawStringInCenterRect(X, y0, WIDTH, 10, "��������� USB-����.", Color::WHITE);
    Painter::DrawStringInCenterRect(X, y0 + dH, WIDTH, 10, "��� ����� ������������ �����������");
    Painter::DrawStringInCenterRect(X, y0 + 2 * dH, WIDTH, 10, "���������...");

    Painter::DrawRectangle(X, Y, WIDTH, HEIGHT);
    Painter::FillRegion(X, Y, static_cast<int>(ms->display.value), HEIGHT);
}



bool Display::IsRun()
{
    return ms->display.isRun;
}



static void DrawBigMNIPI()
{
    static uint startTime = 0;
    static bool first = true;
    
    if(first)
    {
        first = false;
        startTime = TIME_MS;
    }

    uint time = TIME_MS - startTime;

    int numColor = (int)(time / (float)TIME_WAIT * 14.0F);
    Limitation(&numColor, 0, 13);

    Painter::SetColor(static_cast<Color>(static_cast<uint8>(numColor + 2)));

    float amplitude = 3.0F - (time / (TIME_WAIT / 2.0F)) * 3;
    LIMIT_BELOW(amplitude, 0.0F);
    float frequency = 0.05F;

    float radius = 5000.0F * (TIME_WAIT) / 3000.0F / time;
    LIMIT_BELOW(radius, 0);

    float shift[240];

    for (int i = 0; i < 240; i++)
    {
        shift[i] = WAVE_OR_ALL ? amplitude * std::sinf(frequency * time + i / 5.0F) : 0;
    }

    for (int i = 0; i < numPoints; i++)
    {
        int x = array[i].x + (VAGUE_OR_ALL ? RandValue(static_cast<int>(-radius), static_cast<int>(radius)) : 0) + static_cast<int>(shift[array[i].y]); //-V537
        int y = array[i].y + (VAGUE_OR_ALL ? RandValue(static_cast<int>(-radius), static_cast<int>(radius)) : 0);
        if (x > 0 && x < 319 && y > 0 && y < 239)
        {
            Painter::SetPoint(x, y);
        }
    }
}


static int RandValue(int min, int max)
{
    int value = static_cast<int>(std::rand() % (max - min + 1));

    return value + min;
}


static int DrawBigCharInBuffer(int eX, int eY, int size, uint8 symbol, uint8 buffer[320][240])
{
    uint8 width = DFont::GetWidth (symbol);
    uint8 height = DFont::GetHeight();

    for (int b = 0; b < height; b++)
    {
        if (DFont::RowNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (DFont::BitIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            int fullX = x + i;
                            int fullY = y + j;

                            if (fullX >= 0 && fullX < 320 && fullY >= 0 && fullY < 240)
                            {
                                buffer[fullX][fullY] = 1;
                            }
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


static void DrawBigTextInBuffer(int eX, int eY, int size, const char* text, uint8 buffer[320][240])
{
    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 240; y++)
        {
            buffer[x][y] = 0;
        }
    }

    int numSymbols = static_cast<int>(strlen(text));

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigCharInBuffer(x, eY, size, static_cast<uint8>(text[i]), buffer);
        x += size;
    }
}


static void InitPoints()
{
    uint8 buffer[320][240];

    DrawBigTextInBuffer(31, 70, 9, "�����", buffer);

    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 240; y++)
        {
            if (buffer[x][y])
            {
                array[numPoints].x = static_cast<uint16>(x);
                array[numPoints].y = static_cast<uint8>(y);
                numPoints++;
            }
        }
    }
}


void Display::AddStringToIndicating(pString)
{

}
