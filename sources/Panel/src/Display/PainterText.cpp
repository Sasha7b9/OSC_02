#include "Painter.h"
#include "Display/Font/Font.h"
#include "Utils/Math.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void CalculateCurrentColor();

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawTextOnBackground(int x, int y, const char *text, Color colorBackground)
{
    int width = Font::GetLengthText(text);
    int height = Font::GetSize();

    Color colorText(GetColor());
    FillRegion(x - 1, y, width, height, colorBackground);
    SetColor(colorText);

    return DrawText(x, y, text);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawFormatText(int x, int y, char *format, ...)
{
    char buffer[200];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return DrawText(x, y, buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawFormText(int x, int y, Color color, pString text, ...)
{
#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    char buffer[SIZE_BUFFER_DRAW_FORM_TEXT];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, (char *)text, args);
    va_end(args);
    return DrawText(x, y, buffer, color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawStringInCenterRect(int eX, int eY, int width, int eHeight, const char *text, Color color)
{
    SetColor(color);
    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeightSymbol(text[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;
    return DrawText(x + 1, y, text);
}
