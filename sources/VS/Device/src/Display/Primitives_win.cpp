#include "defines.h"
#include "Transceiver.h"
#include "Display/Primitives.h"
#include "Display/Painter.h"
#include <SDL.h>
#include <cstring>


extern SDL_Renderer *renderer;



static int DrawChar(int x, int y, char symbol);

static int DrawBigChar(int eX, int eY, int size, char _symbol);
/// ���������� ���� �������������� ������ �� count ����� c ����������� delta ����� ���������� �������
static void DrawHPointLine(int x, int y, int count, int delta);
/// ���������� ���� ������������ ������ �� count ����� c ����������� delta ����� ���������� �������
static void DrawVPointLine(int x, int y, int count, int delta);



void Region::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderFillRect(renderer, &rect);
}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderDrawRect(renderer, &rect);
    Transceiver::Send(nullptr, 0);                            // ��� ����� ���� ��� ����, ����� ����������� ����� �����
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    SDL_RenderDrawLine(renderer, x, y, x + width, y);
}


void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    SDL_RenderDrawLine(renderer, x, y, x, y + height);
}


void Pixel::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    SDL_RenderDrawPoint(renderer, x, y);
}


void Line::Draw(Color color)
{
    color.SetAsCurrent();
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}


int Text::DrawSmall(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint numSymbols = std::strlen(text);

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawChar(x, y, text[i]);
        x += Font::GetSpacing();
    }

    return x;
}


void Text::DrawBig(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint numSymbols = std::strlen(text);

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, y, sizeOfType, text[i]);
        x += sizeOfType;
    }
}


static int DrawChar(int eX, int eY, char s)
{
    uint8 symbol = static_cast<uint8>(s);

    int8 width = static_cast<int8>(Font::GetWidth(symbol));
    int8 height = static_cast<int8>(Font::GetHeight());

    int delta = Font::IsBig() ? 0 : (9 - height);

    for (int row = 0; row < height; row++)
    {
        if (Font::RowNotEmpty(symbol, row))
        {
            int x = eX;
            int y = eY + row + delta;
            for (int bit = 0; bit < width; bit++)
            {
                if (Font::BitIsExist(symbol, row, bit))
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}


static int DrawBigChar(int eX, int eY, int size, char s)
{
    uint8 symbol = static_cast<uint8>(s);

    int8 width = static_cast<int8>(Font::GetWidth(symbol));
    int8 height = static_cast<int8>(Font::GetHeight());

    for (int b = 0; b < height; b++)
    {
        if (Font::RowNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (Font::BitIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            SDL_RenderDrawPoint(renderer, x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


void MultiHPointLine::Draw(int x, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}


static void DrawHPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        SDL_RenderDrawPoint(renderer, x, y);
        x += delta;
    }
}


void MultiVPointLine::Draw(int y, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}


static void DrawVPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        SDL_RenderDrawPoint(renderer, x, y);
        y += delta;
    }
}
