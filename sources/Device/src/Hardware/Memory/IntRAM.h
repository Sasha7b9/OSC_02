#pragma once
#include "Settings/SettingsOsci.h"


class IntRAM
{
public:
    // ������ ��������� RAM, ��� �������� ��������� �������
    static uint8 *Begin();
    // ����� �������� RAM, ��� �������� ��������� ������� (��������� �� ��������� ���� �� ����������)
    static uint8 *End();

    // ��������� �� 16k ���� ������, ��������������� ��� ������ �����������
    static uint16 *Averager16k(Chan::E ch);
};
