#pragma once
#include "FreqMeter/FreqMeter.h"


struct DisplayFreqMeter
{
    // ��������� ��������
    static void Draw();

private:
    // ������� ���������� ����������
    static void DrawDebugInfo();
};


struct ProgressBarFreqMeter
{
    static void Draw(int x, int y);
};
