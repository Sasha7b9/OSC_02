#pragma once
#include "Menu/MenuItems.h"
#include "Settings/SettingsOsci.h"


/// \brief ��� ������������ ��� �������.
/// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
struct BalanceADC
{
    enum E
    {
        Disabled,   ///< ������������ ���������.
        Settings,   ///< ������������ �������� ������������, ������� �������� �������������.
        Hand        ///< ������������ �������� ������������, �������� �������.
    };
};

/// ��� �������� ���
struct StretchADC
{
    enum E
    {
        Disabled,
        Real,
        Hand,
        Count
    };
    static StretchADC::E &Ref();
    static StretchADC::E Type() { return Ref(); }
    static void SetDisabled()   { Ref() = Disabled; }
    static void SetReal()       { Ref() = Real; }
    static bool IsDisabled()    { return (Ref() == Disabled); }
    static bool IsReal()        { return (Ref() == Real); }
    static bool IsHand()        { return (Ref() == Hand); }
};


/// �������������� �������� ���
struct ShiftADC
{
    enum E
    {
        Disable,    ///< �������������� �������� �� ����������
        Real,       ///< ����������� �������������� ��������, ������������ ���� ����������
        Count
    };

    ShiftADC(Chan::E _ch) : ch(_ch) {};
    static ShiftADC::E &Ref();
    int8 Value() const;
    static void SetDisabled() { Ref() = Disable; };
    static void SetReal()     { Ref() = Real; };
    static bool IsReal()      { return Ref() == Real; };
    void Set(Range::E range, int8 value);

private:
    Chan::E ch;
};


struct SettingsNRST
{
    float           stretchADC[Chan::Count];                    ///< �������� � ����� ����, ����� �������� �������� �����������, ����� ��������� �� 1000 � ��������� �������.
    int16           rShiftAddStable[Chan::Count][3];            ///< ���������� �������� ��� ��� ����� �������������� ����������. ������� �������� ��� ���������
    int8            shiftADC[Chan::Count][Range::Count];        ///< ���������� ��������, ������� ������� ���� ��� ���������� � ������������
    int16           balanceADC[Chan::Count];                    ///< �������� ��������������� �������� ��� ��� ������ ������������.
    int16           numAveForRand;                              ///< �� �������� ���������� ��������� ������ � ������ �������������.
    int16           numSmoothForRand;                           ///< ����� ����� ��� ����������� ������ � �������������.
    int16           correctionTime;                             ///< ����������� ��������� �������.
    int16           enum_gate_max;                              ///< ����������� ����� � ������������� ������
    int16           enum_gate_min;                              ///< ����������� ����� � ������������� �����
    BalanceADC::E   balanceADCtype;                             ///< ��� ������������.
    ShiftADC::E     shiftADCtype;                               ///< ��� ������������ ��� ��������� ��������������� ��������
    StretchADC::E   stretchADCtype;                             ///< ��� �������� ������.

};


struct SettingsDebug
{ //-V802
    int8         showConsole;                   ///< ���������� �� �������
    int16        numStrings;                    ///< ����� ����� � �������.
    int8         sizeFont;                      ///< ������ ������ ������� - 0 - 5, 1 - 8.
    bool         modePauseConsole;              ///< ���� true, �� ����� � ������� ��������������� �������� ������ ����/����.
    bool         showAll;
    bool         showFlag;
    bool         showRShift[2];
    bool         showTrigLev;
    bool         showRange[2];
    bool         showChanParam[2];
    bool         showTrigParam;
    bool         showTShift;
    bool         showTBase;
    int16        numMeasuresForGates;           ///< ����� ��������� ��� �����.
    bool         showRandInfo;                  ///< �������� ���������� �� ������������� - ������ � ��������� ��������.
    bool         showRandStat;                  ///< �������� ������ ����������.
    int16        timeCompensation;              ///< �������������� �������� �� ������� ��� ������ �������� ������ �������������.
    int16        pretriggered;                  ///< ������������ �������� ����������� ��� ������������ �������������.
    bool         ShowStats;                     ///< ���������� ���������� �� ������ (fps, ��������).
    bool         runTest;                       ///< ���� true, �� ����� ��������� ���� ��� �������
    SettingsNRST nrst;
};


struct PageDebug
{
    static const Page *const self;



    struct PageConsole
    {
        static const Page *const self;
    };


    struct PageRegisters
    {
        static const Page *const self;
    };


    struct PageRand
    {
        static const Page *const self;
    };


    struct PageSettings
    {
        static const Page *const self;
    };


    struct PageSerialNumber
    {
        static const Page *const self;
    };


    struct PageTests
    {
        static const Page *const self;

        static void OnPress_Run();
    };


    struct PageADC
    {
        static const Page *const self;


        struct PageBalance
        {
            static const Page *const self;
        };


        struct PageStretch
        {
            static const Page *const self;

            static void OnChanged_Mode(bool);
        };


        struct PageShift
        {
            static const Page *const self;
        };

    };
};
