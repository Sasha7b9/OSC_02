#pragma once
#include "Settings/SettingsOsci.h"


struct Calibrator
{
    static void Calibrate();
    /// ������������� �����
    static void Balance(Chan::E ch);

private:

    static void BalanceChannel(Chan::E ch, Range::E range);

    static void Stretch(Chan::E ch);
};
