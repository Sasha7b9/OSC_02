#pragma once
#include "defines.h"
#include "Command.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Painter
{
    /// ���������� � ������ ��������� ������� �����. ��������� ����� ������ color
    void BeginScene();
    /// ���������� � ����� ��������� ������� �����. ��������� ���������� ������ �� �����
    void EndScene();
    /// ���������� ������ � ������
    void SendRow(int row);

    void SetColor(Color color);
    /// ���������� ������� ���� ���������
    Color GetColor();
    /// ������������� ����� �������� ��������. ����������� � ������� LoadPalette() ��� SetPalette()
    void SetColorValue(Color color, uint value);
    /// ��������� � ������� ��� �����
    void LoadPalette();

    void DrawTesterData(uint8 mode, Color color, uint16 x[TESTER_NUM_POINTS], uint8 y[TESTER_NUM_POINTS]);
    /// ���������� ���� ���������
    /// ���������� ����� ������� ������
    void SetPoint(int x, int y);

    void DrawVPointLine(int x, int y, int delta, int count);

    void DrawHPointLine(int x, int y, int delta, int count);
    /// ���������� �������������� �����
    void DrawHLine(int y, int x0, int x1);
    /// ���������� ������������ �����
    void DrawVLine(int x, int y0, int y1);
    /// ���������� ������������ �����
    void DrawLine(int x0, int y0, int x1, int y1);
    /// \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. 
    /// dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    /// ������ ����������� ������������ �����.
    void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    void DrawRectangle(int x, int y, int width, int height);

    void FillRegion(int x, int y, int width, int height);

    uint ReduceBrightness(uint colorValue, float newBrightness);
    /// ������� ����� �� �������������� ����� colorBackgound
    int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);

    int DrawFormatText(int x, int y, char *format, ...);
    /// ����� ������ � ������� x, y
    int DrawFormText(int x, int y, Color color, pString text, ...);
    /// �������� ������ ����������� numString � ����������
    void SendScreenToDevice();
//
//    extern Color currentColor;
//
//    extern Font::Type::E currentTypeFont;
};
