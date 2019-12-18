#pragma once
#include "Menu/MenuItems.h"
#include "Settings/SettingsOsci.h"


/// �������� �������������
struct TrigSource
{
    TrigSource() {};
    static void Set(Chan::E ch = Chan::Count);
    static Chan::E &Ref();
    operator Chan::E() { return Ref(); };

};

/// ����� ������ ������������
struct TrigModeFind
{
    enum E
    {
        Hand,      ///< ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
        Auto       ///< ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
    };

    static TrigModeFind::E &Ref();
    static bool IsAuto() { return Ref() == Auto; }
    static bool IsHand() { return Ref() == Hand; }
};

struct TrigInput
{
    enum E
    {
        Full,
        HF,
        LF
    };
    TrigInput() {};
    static TrigInput::E &Ref();
    operator TrigInput::E() { return Ref(); }
    /// ���������� � �������������� ��������� ������, ���������� �� �������� � ���� �������������
    static void Load();
};

struct TrigPolarity
{
    enum E
    {
        Rising,
        Falling
    };
    TrigPolarity() {}
    static TrigPolarity::E &Ref();
    void Load();
    operator TrigPolarity::E() { return Ref(); }
};

/// ����� �������.
struct TrigStartMode
{
    enum E
    {
        Auto,     ///< ��������������.
        Wait,     ///< ������.
        Single,   ///< �����������.
        Count
    };
    TrigStartMode() {}
    static TrigStartMode::E &Ref();
    operator TrigStartMode::E() { return Ref(); }
    static bool IsSingle()      { return Ref() == Single; }
    static bool IsWait()        { return Ref() == Wait; }
    static bool IsAuto()        { return Ref() == Auto; }
};


struct SettingsTrig
{
    Chan::E          source;
    TrigInput::E     input;
    TrigPolarity::E  polarity;
    int16            level[Chan::Count];
    TrigStartMode::E startMode;         ///< ����� �������.
    TrigModeFind::E  modeFind;          ///< ����� ������������� - ������� ��� �������������.
                                        /// \todo ����������� trig_holdOff ������������
    int16            holdOff;           ///< ��������� �������������
    bool             holdOffEnabled;    ///< ��������� ��������� �������������
};



struct PageTrig
{
    static void OnChanged_Mode(bool active);

    static const Page * const self;


    struct PageHoldOff
    {
        static const Page * const self;
    };


    struct PageFind
    {
        static const Page * const self;
    };
};
