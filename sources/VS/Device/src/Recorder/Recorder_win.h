#pragma once
#include "Settings/SettingsOsci.h"


struct RecorderHAL
{
    /// ���������� true, ���� ����� ������ � ����������
    static bool ReadyPoint();

    static uint8 ReadData(Chan::E channel);
};
