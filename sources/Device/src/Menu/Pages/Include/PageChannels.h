#pragma once
#include "Menu/MenuItems.h"


struct SettingsChannel
{ //-V802
    int16         rShift;    // ����� ������ �� ���������
    Range::E      range;     // ������� ������ �� ���������
    ModeCouple::E couple;    // ����� �� �����
    bool          enabled;   // �������/�������� �����
    Bandwidth::E  bandwidth; // ����������� ������
    bool          inverse;
    Divider::E    divider;   // ���������
};



struct PageChannelA
{
    static void OnChanged_Couple(bool active);

    static const Page * const self;
};



struct PageChannelB
{
    static void OnChanged_Couple(bool active);

    static const Page * const self;
};
