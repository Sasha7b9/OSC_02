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
    } value;
    static BalanceADC &Ref();
    static bool IsHand()            { return Ref().value == Hand; }
    static int16 &Value(Chan::E ch) { return Ref().balance[ch]; }
    int16  balance[Chan::Count];                    ///< �������� ��������������� �������� ��� ��� ������ ������������.
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
    } value;
    static StretchADC &Ref();
    static StretchADC::E Type()    { return Ref().value; }
    static void SetDisabled()      { Ref().value = Disabled; }
    static void SetReal()          { Ref().value = Real; }
    static bool IsDisabled()       { return (Ref().value == Disabled); }
    static bool IsReal()           { return (Ref().value == Real); }
    static bool IsHand()           { return (Ref().value == Hand); }
    static float Value(Chan::E ch) { return Ref().stretch[ch]; }
    float  stretch[Chan::Count];            ///< �������� � ����� ����, ����� �������� �������� �����������, ����� ��������� �� 1000 � ��������� �������.
};


/// �������������� �������� ���
struct ShiftADC
{
    enum E
    {
        Disable,    ///< �������������� �������� �� ����������
        Real,       ///< ����������� �������������� ��������, ������������ ���� ����������
        Count
    } value;
    static ShiftADC &Ref();
    static void SetDisabled() { Ref().value = Disable; };
    static void SetReal()     { Ref().value = Real; };
    static bool IsReal()      { return Ref().value == Real; };
    static void Set(Chan::E ch, Range::E range, int8 value) { Ref().shift[ch][range] = value; }
    static int8 Value(Chan::E ch, Range::E range) { return IsReal() ? Ref().shift[ch][range] : 0; };
    int8  shift[Chan::Count][Range::Count];        ///< ���������� ��������, ������� ������� ���� ��� ���������� � ������������
};


struct SettingsNRST
{
    int16       numAveForRand;          ///< �� �������� ���������� ��������� ������ � ������ �������������.
    int16       numSmoothForRand;       ///< ����� ����� ��� ����������� ������ � �������������.
    int16       correctionTime;         ///< ����������� ��������� �������.
    int16       enum_gate_max;          ///< ����������� ����� � ������������� ������
    int16       enum_gate_min;          ///< ����������� ����� � ������������� �����
    BalanceADC  balanceADC;             ///< ��� ������������.
    ShiftADC    shiftADC;               ///< ��� ������������ ��� ��������� ��������������� ��������
    StretchADC  stretchADC;             ///< ��� �������� ������.
};


struct SettingsDebug
{ //-V802
    int8         showConsole;           ///< ���������� �� �������
    int16        numStrings;            ///< ����� ����� � �������.
    int8         sizeFont;              ///< ������ ������ ������� - 0 - 5, 1 - 8.
    bool         modePauseConsole;      ///< ���� true, �� ����� � ������� ��������������� �������� ������ ����/����.
    bool         showAll;
    bool         showFlag;
    bool         showRShift[2];
    bool         showTrigLev;
    bool         showRange[2];
    bool         showChanParam[2];
    bool         showTrigParam;
    bool         showTShift;
    bool         showTBase;
    int16        numMeasuresForGates;   ///< ����� ��������� ��� �����.
    bool         showRandInfo;          ///< �������� ���������� �� ������������� - ������ � ��������� ��������.
    bool         showRandStat;          ///< �������� ������ ����������.
    int16        timeCompensation;      ///< �������������� �������� �� ������� ��� ������ �������� ������ �������������.
    int16        pretriggered;          ///< ������������ �������� ����������� ��� ������������ �������������.
    bool         ShowStats;             ///< ���������� ���������� �� ������ (fps, ��������).
    bool         runTest;               ///< ���� true, �� ����� ��������� ���� ��� �������
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
