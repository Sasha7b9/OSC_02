#pragma once
#include "Menu/MenuItems.h"
#include "Osci/ParametersOsci.h"


// �������� �������������
struct TrigSource
{
    static void Set(Chan::E ch = Chan::Count);
};

// ����� ������ ������������
struct TrigModeFind
{
    enum E
    {
        Hand,      // ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
        Auto       // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
    };
};

struct TrigInput
{
    enum E
    {
        Full,
        HF,
        LF
    };
    // ���������� � �������������� ��������� ������, ���������� �� �������� � ���� �������������
    static void Load();
};

struct TrigPolarity
{
    enum E
    {
        Rising,
        Falling
    };
    static void Load();
};


// ����� �������.
struct TrigStartMode
{
    enum E
    {
        Auto,     // ��������������.
        Wait,     // ������.
        Single,   // �����������.
        Count
    };
    static void Set(TrigStartMode::E);
};


#define S_TRIG_SOURCE (set.trig.source)

struct SettingsTrig
{
    Chan::E          source;
    TrigInput::E     input;
    TrigPolarity::E  polarity;
    int16            level[Chan::Count];
    TrigStartMode::E startMode;         // ����� �������.
    TrigModeFind::E  modeFind;          // ����� ������������� - ������� ��� �������������.
                                        // \todo ����������� trig_holdOff ������������
    int16            holdOff;           // ��������� �������������
    bool             holdOffEnabled;    // ��������� ��������� �������������
};


struct PageTrig
{
    static const Page * const self;


    struct HoldOff
    {
        static const Page * const self;
    };


    struct Find
    {
        static const Page * const self;
    };
};
