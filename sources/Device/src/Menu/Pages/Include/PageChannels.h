#pragma once
#include "Menu/MenuItems.h"
#include "FPGA/FPGA_Settings.h"


struct SettingsChannel
{
    uint16                         shift[Chan::Size];      ///< ����� ������ �� ���������
    Osci::Settings::Range::E       range[Chan::Size];      ///< ������� ������ �� ���������
    FPGA::Settings::ModeCouple::E  couple[Chan::Size];     ///< ����� �� �����
    bool                           enabled[Chan::Size];    ///< �������/�������� �����
    int8                           balanceShiftADC[2];     ///< ���������� �������� ��� ������������ ���.
    FPGA::Settings::Bandwidth      bandwidth[2];           ///< ����������� ������.
    bool                           inverse[2];
    Divider::E                     divider[2];             ///< ���������.
    CalibrationMode::E             calibrationMode[2];     ///< ����� ����������.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageChannelA
{
    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageChannelB
{
    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const Page * const self;
};
