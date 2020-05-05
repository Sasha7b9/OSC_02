#pragma once
#include "Menu/MenuItems.h"


// ��������.
struct Divider
{
    enum E
    {
        _1,
        _10
    };

    static int ToAbs(Divider::E v) { return (v == _1) ? 1 : 10; };
};


struct SettingsChannel
{ //-V802
    int16               rShift;              // ����� ������ �� ���������
    Range::E            range;               // ������� ������ �� ���������
    ModeCouple::E       couple;              // ����� �� �����
    bool                enabled;             // �������/�������� �����
    Bandwidth::E        bandwidth;           // ����������� ������
    bool                inverse;
    Divider::E          divider;             // ���������
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
