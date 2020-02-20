#pragma once
#include "Settings/ParametersOsci.h"


#define SHIFT_IN_MEMORY     DisplayOsci::PainterData::FirstPointOnScreen()


struct DisplayOsci
{
    static void Update();

    void DrawScaleLine(int x, bool forTrigLev);
    /// ���������� ������� ����, ��� ������� ��������� � �����������
    static void SetFlagRedraw();

    struct HiPart
    {
        static void Draw();
    private:
        /// ���������� ������ ����� - ������������� � ����� ������
        static void DrawRightPart(int x0, int y0);
    };

    struct BottomPart
    {
        static void Draw(int x, int y);
    };

    struct Accumulator
    {
        /// ��� ������� ����� �������� ����� ������ ��������� ��������
        static void NextFrame();
        /// ����� ����������
        static void Reset();
    };

    struct PainterData
    {
        static void DrawData();
        /// ������ ������ �����, ��������� ������ �����
        static int FirstPointOnScreen();
        /// ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
        static BitSet64 PointsOnDisplay();
        /// \brief ��������� ����� ������ � ��������� ����� � ����������� ������
        static BitSet64 BytesOnDisplay();
        /// ��� ������� ���� �������� ��� ������������ TPos ��� ����������� �������� ������� ���������� ����� ������������ ������ ���� ������
        static void ChangeTPos();
    private:
        /// ���������� ���������� ������ - ��������������� ������� ����������
        static void DrawCurrent();
        /// ���������� ������ �� ���
        static void DrawRAM();
        /// ���������� ������ �� ����
        static void DrawROM();

        static void DrawChannel(Chan::E ch);

        static void DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale);

        static void DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x);

        static void DrawModeLinesPeakDetOff(int center, const uint8 *data, float scale, int x);

        static void DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale);

        static void DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x);

        static void DrawModePointsPeakDetOff(int center, const uint8 *data, float scale, int x);
        /// ���������� ������
        static void DrawSpectrum();

        static void DrawSpectrum(const uint8 *dataIn, uint numPoints, Chan::E ch);

        static void DrawSpectrumChannel(const float *spectrum, Color color);

        static void WriteParametersFFT(Chan::E ch, float freq0, float density0, float freq1, float density1);
    };

    struct MemoryWindow
    {
        static void Draw();

        static int X();

        static int Y();

        static int Width();

        static int Height();
    };
};
