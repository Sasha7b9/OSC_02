#pragma once
#include "Settings/ParametersOsci.h"


struct RecorderHAL
{
    /// ���������� true, ���� ����� ������ � ����������
    static bool ReadyPoint();

    static uint8 ReadData(Chan::E channel);
};
