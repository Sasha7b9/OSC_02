#pragma once
#include "Menu/MenuItems.h"


struct CalibratorMode
{
    enum E
    {
        Freq,
        DC,
        GND
    } value;
    explicit CalibratorMode(E v) : value(v) {};
};

struct SettingsService
{
    CalibratorMode::E   calibratorMode;            ///< ����� ������ �����������.
    uint8               soundVolume;               ///< ��������� �����
    bool                showInfoVoltage;           ///< ���������� ���������� ������� �� ������
};



struct PageService
{
    static const Page * const self;

    static void OnPress_ResetSettings();

    struct PageBattery
    {
        static const Page * const self; 
    };


    struct PageCalibrate
    {
        static const Page * const self;
    };


    struct PageInformation
    {
        static const Page * const self;
    };
};



struct PageRTC
{
    static const Page * const self;
};
