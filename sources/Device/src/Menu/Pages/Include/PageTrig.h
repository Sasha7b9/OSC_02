#pragma once
#include "Menu/MenuItems.h"


struct SettingsTrig
{
    Osci::Settings::Trig::Source::E    source;
    Osci::Settings::Trig::Input::E     input;
    Osci::Settings::Trig::Polarity::E  polarity;
    uint16                             lev[Chan::Size];
    Osci::Settings::Trig::StartMode::E startMode;           ///< ����� �������.
    Osci::Settings::Trig::ModeFind::E  modeFind;            ///< ����� ������������� - ������� ��� �������������.
                                            /// \todo ����������� trig_holdOff ������������
    int16                              holdOff;             ///< ��������� �������������
    bool                               holdOffEnabled;      ///< ��������� ��������� �������������
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageTrig
{
    static void OnChanged_Mode(bool active);

    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageHoldOff
    {
        static const Page * const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageFind
    {
        static const Page * const self;
    };
};
