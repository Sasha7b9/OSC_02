#pragma once
#include "Menu/MenuItems.h"
#include "Osci/ParametersOsci.h"


// �������� �������������
struct TrigSource
{
    static void Set(Chan::E ch = Chan::Count);
};


// ����� ������ ������������
#define S_TRIG_MODE_FIND            (set.trig.modeFind)
#define S_TRIG_MODE_FIND_IS_AUTO    (S_TRIG_MODE_FIND == TrigModeFind::Auto)
struct TrigModeFind
{
    enum E
    {
        Hand,      // ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
        Auto       // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
    };
};


#define S_TRIG_INPUT    (set.trig.input)
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


#define S_TRIG_POLARITY     (set.trig.polarity)
#define S_TRIG_POLARITY_U8  (static_cast<uint8>(S_TRIG_POLARITY))
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
#define S_TRIG_START_MODE           (set.trig.startMode)
#define S_TRIG_START_MODE_IS_SINGLE (S_TRIG_START_MODE == TrigStartMode::Single)
#define S_TRIG_START_MODE_IS_AUTO   (S_TRIG_START_MODE == TrigStartMode::Auto)
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


#define S_TRIG_SOURCE           (set.trig.source)

#define S_TRIG_LEVEL(channel)   (set.trig.level[channel])
#define S_TRIG_LEVEL_SOURCE     S_TRIG_LEVEL(S_TRIG_SOURCE)

#define S_TRIG_HOLDOFF_VALUE    (set.trig.holdOff)
#define S_TRIG_HOLDOFF_ENABLED  (set.trig.holdOffEnabled)


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
