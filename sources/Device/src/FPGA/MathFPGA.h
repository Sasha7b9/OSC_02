#pragma once
#include "Settings/SettingsOsci.h"


struct MathFPGA
{
    static float VoltageCursor(float shiftCurU, Range::E range, int16 rShift);

    static float TimeCursor(float shiftCurT, TBase::E tBase);

    static void CalculateFFT(float *data, uint numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);
};
