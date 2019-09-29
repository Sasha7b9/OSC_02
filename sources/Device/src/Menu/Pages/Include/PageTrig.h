#pragma once
#include "Menu/MenuItems.h"


#define START_MODE              (set.trig.startMode)
#define START_MODE_IS_AUTO      (START_MODE == TrigStartMode::Auto)
#define START_MODE_IS_WAIT      (START_MODE == TrigStartMode::Wait)
#define START_MODE_IS_SINGLE    (START_MODE == TrigStartMode::Single)

#define TRIG_POLARITY           (set.trig.polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig.input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig.TimeDelay)


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

struct TrigPolarity
{
    enum E
    {
        Rising,
        Falling
    } value;
    explicit TrigPolarity(E v) : value(v) {};

    static void Load();
};

/// ����� �������.
struct TrigStartMode
{
    enum E
    {
        Auto,     ///< ��������������.
        Wait,     ///< ������.
        Single    ///< �����������.
    } value;
    explicit TrigStartMode(E v) : value(v) {};
};


struct SettingsTrig
{
    TrigSource::E    source;
    TrigInput::E     input;
    TrigPolarity::E  polarity;
    uint16                             lev[Chan::Size];
    TrigStartMode::E startMode;           ///< ����� �������.
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
