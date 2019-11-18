#pragma once
#include "common/Command.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Settings/SettingsOsci.h"


struct DisplayTester
{
    static void Update();
    /// ������������� ����� ��� ���������, ��������������� ���� numStep
    static void SetPoints(int numStep, const uint16 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS]);
};


struct Tester
{
#define Port_TEST_ON  HPort::_F
#define Port_PNP      HPort::_F
#define Port_U        HPort::_F
#define Port_I        HPort::_G
#define Port_TEST_STR HPort::_C

    static uint16 Pin_TEST_ON;
    static uint16 Pin_PNP;
    static uint16 Pin_U;
    static uint16 Pin_I;
    static uint16 Pin_TEST_STR;

    static const int NUM_STEPS = 5;
    /// ������������� ���������� �����
    static void Init();

    void DeInit();

    static void Enable();

    static void Disable();

    static void StartStop();

    static void Update();

    static void ProcessStep();
    /// ��������� ���������� �� Settings
    static void LoadPolarity();
    /// ������������� ��� ��������� ���������� � ����������� � ����������� Settings
    static void LoadStep();

    static int16 DeltaRShiftA();

    /// �������
    struct Scale
    {
        enum E
        {
            _2uA,
            _5uA,
            _10uA,
            _20uA,
            _50uA,
            _100uA,
            _200uA,
            _500uA,
            _1mA,
            _2mA,
            _5mA,
            _10mA,
            _20mA,
            Count
        } value;
        Chan::E ch;
        explicit Scale(Range::E range, Chan::E _ch) : value(static_cast<E>(range)), ch(_ch) {};
        pString ToString() const;
    };

    /// ��������
    struct Shift
    {
        Shift(int16 rShift, Chan::E _ch) : shift(rShift), ch(_ch) {};
        String ToString(Scale::E scale);
    private:
        int16 shift;
        Chan::E ch;
    };
};
