#pragma once
#include "Menu/MenuItems.h"


/// �������� �������������
struct TrigSource
{
    enum E
    {
        A,    /// ����� 1
        B     /// ����� 2
    } value;

    static void Load();
};

/// ����� ������ ������������
struct TrigModeFind
{
    enum E
    {
        Hand,      ///< ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
        Auto       ///< ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
    } value;
    explicit TrigModeFind(E v) : value(v) {};
};

struct TrigInput
{
    enum E
    {
        Full,
        HF,
        LF
    } value;

    /// ���������� � �������������� ��������� ������, ���������� �� �������� � ���� �������������
    static void Load();
};


struct SettingsTrig
{
    TrigSource::E    source;
    TrigInput::E     input;
    Osci::Settings::Trig::Polarity::E  polarity;
    uint16                             lev[Chan::Size];
    Osci::Settings::Trig::StartMode::E startMode;           ///< ����� �������.
    TrigModeFind::E  modeFind;            ///< ����� ������������� - ������� ��� �������������.
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
