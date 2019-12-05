#pragma once
#include "Menu/MenuItems.h"
#include "Settings/SettingsOsci.h"


/// \brief ��� ������������ ��� �������.
/// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
struct BalanceADC
{
    enum E
    {
        Disable,     ///< ������������ ���������.
        Settings,    ///< ������������ �������� ������������, ������� �������� �������������.
        Hand         ///< ������������ �������� ������������, �������� �������.
    };
};

/// ��� �������� ���
struct StretchADC
{
    enum E
    {
        Disable,
        Real,
        Hand,
        Count
    };
};


/// �������������� �������� ���
struct AddRShiftADC
{
    enum E
    {
        Disable,    ///< �������������� �������� �� ����������
        Real,       ///< ����������� �������������� ��������, ������������ ���� ����������
        Count
    };

    static void SetDisabled();
    static void SetReal();
};


struct SettingsNRST
{
    int16           numAveForRand;                              ///< �� �������� ���������� ��������� ������ � ������ �������������.
    int16           numSmoothForRand;                           ///< ����� ����� ��� ����������� ������ � �������������.
    int16           correctionTime;                             ///< ����������� ��������� �������.
    int16           enum_gate_max;                              ///< ����������� ����� � ������������� ������
    int16           enum_gate_min;                              ///< ����������� ����� � ������������� �����

    BalanceADC::E   balanceADCtype;                             ///< ��� ������������.
    int16           balanceADC[Chan::Count];                    ///< �������� ��������������� �������� ��� ��� ������ ������������.
    AddRShiftADC::E addRShiftADCtype;                           ///< ��� ������������ ��� ��������� ��������������� ��������
    int8            rShiftAdd[Chan::Count][Range::Count];       ///< ���������� ��������, ������� ������� ���� ��� ���������� � ������������
    StretchADC::E   stretchADCtype;                             ///< ��� �������� ������.
    int16           stretchADC[Chan::Count][StretchADC::Count]; ///< \brief ����������� ����������� ��� �������, �������������� �
                    ///< ������������ �������. ����� �������� � ����� ����, ����� �������� �������� �����������, ����� ��������� �� 1000 �
                    ///< ��������� �������.

    int16           rShiftAddStable[Chan::Count][3];            ///< ���������� �������� ��� ��� ����� �������������� ����������. ������� �������� ��� ���������
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

float   GetStretchADC(Chan::E ch);

void    SetStretchADC(Chan::E ch, float kStretch);




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

    };
};
