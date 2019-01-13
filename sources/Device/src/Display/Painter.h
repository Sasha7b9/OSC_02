#pragma once
#include "Colors.h"
#include "Display_Types.h"
#include "Display/Font/Font.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Painter
{
    void Init();

    void BeginScene(Color color);

    void EndScene();

    void SetFont(Font::Type::E typeFont);

    void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);
    /// ���������� ������� �������.
    void SetBrightnessDisplay(int16 brightness);

    void Draw10SymbolsInRect(int x, int y, char eChar);
    /// ���������� ������ ������������ �����. ����� �������� ���� �� ������. y0y1 - ������ ������������ ���������.
    void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);
    /// \brief ���������� numLines ������������ �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. �������������� ����������
    /// ������ ����� ������ ����� ������������� ���������� �������� ������� x[]
    void DrawMultiVPointLine(int numLines, int y, const uint16 *x, int delta, int count, Color color = Color::NUMBER);

    void DrawTesterData(uint8 mode, Color color, const uint8 *x, const uint8 *y);

    void SaveScreenToDrive();
};
