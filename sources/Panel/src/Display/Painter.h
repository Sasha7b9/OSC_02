#pragma once
#include "defines.h"
#include "Command.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** @addtogroup Display
 *  @{
 *  @defgroup Painter
 *  @brief ������� ���������
 *  @{
 */

class Painter
{
public:
    /// ���������� � ������ ��������� ������� �����. ��������� ����� ������ color
    static void BeginScene(Color color);
    /// ���������� � ����� ��������� ������� �����. ��������� ���������� ������ �� �����
    static void EndScene();

    static void SetColor(Color color);
    /// ���������� ������� ���� ���������
    static Color GetColor();
    /// ������������� ����� �������� ��������. ����������� � ������� LoadPalette() ��� SetPalette()
    static void SetColorValue(Color color, uint value);
    /// ��������� � ������� ��� �����
    static void LoadPalette();

    static void DrawTesterData(uint8 mode, Color color, uint16 x[TESTER_NUM_POINTS], uint8 y[TESTER_NUM_POINTS]);
    /// ���������� ���� ���������
    /// ���������� ����� ������� ������
    static void SetPoint(int x, int y);

    static void DrawVPointLine(int x, int y, int delta, int count);

    static void DrawHPointLine(int x, int y, int delta, int count);
    /// ���������� �������������� �����
    static void DrawHLine(int y, int x0, int x1);
    /// ���������� ������������ �����
    static void DrawVLine(int x, int y0, int y1);
    /// ���������� ������������ �����
    static void DrawLine(int x0, int y0, int x1, int y1);
    /// \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. 
    /// dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    /// ������ ����������� ������������ �����.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawRectangle(int x, int y, int width, int height);

    static void FillRegion(int x, int y, int width, int height);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    static void SetFont(Font::Type::E typeFont);

    static int DrawText(int x, int y, const char *text);
    /// ������� ����� �� �������������� ����� colorBackgound
    static int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);

    static int DrawFormatText(int x, int y, char *format, ...);
    /// ����� ������ � ������� x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);
    /// �������� ������ ����������� numString � ����������
    static void SendScreenToDevice();

private:

    static Color currentColor;

    static Font::Type::E currentTypeFont;
};


/** @} @}
 */
