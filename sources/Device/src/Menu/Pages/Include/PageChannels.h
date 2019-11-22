#pragma once
#include "Menu/MenuItems.h"
#include "Settings/SettingsOsci.h"


#define SET_ENABLED_BOTH        (set.ch[Chan::A].enabled && set.ch[Chan::B].enabled)
#define SET_DISABLED_BOTH       (!set.ch[Chan::A].enabled && !set.ch[Chan::B].enabled)


/// ����� ����������.
struct CalibrationMode
{
    enum E
    {
        x1,
        x10,
        Disable
    } value;
};

/// ��������.
struct Divider
{
    enum E
    {
        _1,
        _10
    } value;
    Divider(Chan::E ch);
    int ToAbs() const;
    static int ToAbs(Divider::E v) { return (v == _1) ? 1 : 10; };
    operator uint() { return value; }
};


struct SettingsChannel
{ //-V802
    int16               rShift;              ///< ����� ������ �� ���������
    Range::E            range;               ///< ������� ������ �� ���������
    ModeCouple::E       couple;              ///< ����� �� �����
    bool                enabled;             ///< �������/�������� �����
    int8                balanceShiftADC;     ///< ���������� �������� ��� ������������ ���
    Bandwidth           bandwidth;           ///< ����������� ������
    bool                inverse;
    Divider::E          divider;             ///< ���������
    CalibrationMode::E  calibrationMode;     ///< ����� ����������
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
