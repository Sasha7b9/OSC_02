#pragma once
#include "Settings/Settings.h"


/*
    ������ � ����������� EEPROM AT25160N, ��������������� ��� �������� ��������.
*/
class AT25160N
{
friend class Settings;
public:
    static void Init();
    static void Test();
private:
    static void Save(Settings &set);
    static void Load(Settings &set);
};
