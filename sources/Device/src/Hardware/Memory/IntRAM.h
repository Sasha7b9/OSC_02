#pragma once
#include "Settings/SettingsOsci.h"


class IntRAM
{
public:
    // ��������� �� 16k ���� ������, ��������������� ��� ������ �����������
    static uint16 *Averager16k(Chan::E ch);
    // ������ ��� ������ ������ � ������ �������������
    static uint8 *DataRand(Chan::E ch);
};
