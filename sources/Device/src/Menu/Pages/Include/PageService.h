#pragma once
#include "FPGA/Calibrator.h"
#include "Menu/MenuItems.h"


struct SettingsService
{
    Calibrator::Mode::E calibratorMode;     // ����� ������ �����������.
    uint8               enumVolume;         // ��������� �����
    bool                showInfoVoltage;    // ���������� ���������� ������� �� ������
    uint8               SoundVolume();      // ���������� �������� �� 0 (����) �� 100 (����)
};



struct PageService
{
    static const Page * const self;

    static void OnPress_ResetSettings();

    static void DecodePassword(const KeyEvent &event);

    struct Battery
    {
        static const Page * const self; 
    };


    struct Calibrate
    {
        static const Page * const self;
    };


    struct Information
    {
        static const Page * const self;
    };
};



struct PageRTC
{
    static const Page * const self;
};
