#pragma once
#include "Osci/ParametersOsci.h"


struct Calibrator
{
    // ��������� ��������� ����������
    static void Calibrate();
    
    // ������������� ����� �� ���� ����������
    static bool BalanceChannel(Chan::E ch, bool showHint = true);

    struct Mode
    {
        enum E
        {
            Freq,
            DC,
            GND
        };
        Mode() {}
        static Mode::E &Ref();
        operator Mode::E() { return Ref(); }
    };
};
