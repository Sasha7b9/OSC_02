#pragma once
#include "Settings/SettingsOsci.h"


class IntRAM
{
public:
    // ��������� �� 16k ���� ������, ��������������� ��� ������ �����������
    static uint16 *Averager16k(Chan::E ch);

private:
    // ������ ��������� RAM, ��� �������� ��������� �������
    static uint8 *Begin();
    // ����� �������� RAM, ��� �������� ��������� ������� (��������� �� ��������� ���� �� ����������)
    static uint8 *End();
};
