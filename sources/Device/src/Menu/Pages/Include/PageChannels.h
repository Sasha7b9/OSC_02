#pragma once
#include "Menu/MenuItems.h"


#define S_RSHIFT(channel)       set.ch[channel].rShift
#define S_RSHIFT_A              S_RSHIFT(0)
#define S_RSHIFT_B              S_RSHIFT(1)
#define S_RSHIFT_MATH           set.math.rShift

#define S_CHANNEL_ENABLED(chan) set.ch[chan].enabled

#define S_INVERSE(chan)         set.ch[chan].inverse
#define S_INVERSE_A             S_INVERSE(ChanA)
#define S_INVERSE_B             S_INVERSE(ChanB)


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
