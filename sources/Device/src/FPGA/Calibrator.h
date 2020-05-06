#pragma once
#include "Osci/ParametersOsci.h"


class Calibrator
{
public:
    // ��������� ��������� ����������
    static void Calibrate();
    
    // ������������� ����� �� ���� ����������
    static bool BalanceChannel(Chan::E ch, bool showHint = true);

    struct Mode
    {
#define S_SERV_CALIBRATOR_MODE  (set.serv._calibratorMode)

        enum E
        {
            Freq,
            DC,
            GND
        };
    };
};
