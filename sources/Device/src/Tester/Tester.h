#pragma once
#include "Command.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Settings/SettingsOsci.h"



namespace Tester
{
#define Port_TEST_ON  HAL::PIO::Port::_F
#define Port_PNP      HAL::PIO::Port::_F
#define Port_U        HAL::PIO::Port::_F
#define Port_I        HAL::PIO::Port::_G
#define Port_TEST_STR HAL::PIO::Port::_C

    extern uint16 Pin_TEST_ON;
    extern uint16 Pin_PNP;
    extern uint16 Pin_U;
    extern uint16 Pin_I;
    extern uint16 Pin_TEST_STR;

    static const int NUM_STEPS = 5;
    /// ������������� ���������� �����
    void Init();

    void DeInit();

    void Enable();

    void Disable();

    void StartStop();

    void Update();

    static void ProcessStep();
    /// ��������� ���������� �� Settings
    void LoadPolarity();
    /// ������������� ��� ��������� ���������� � ����������� � ����������� Settings
    void LoadStep();

    int DeltaRShiftA();

    namespace Display
    {
        void Update();
        /// ������������� ����� ��� ���������, ��������������� ���� numStep
        void SetPoints(int numStep, const uint16 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS]);
    };

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
            Number
        } value;
        Chan::E ch;
        explicit Scale(Range::E range, Chan::E _ch) : value(static_cast<E>(range)), ch(_ch) {};
        pString ToString() const;
    };

    /// ��������
    struct Shift
    {
        Shift(uint16 rShift, Chan::E _ch) : shift(rShift), ch(_ch) {};
        String ToString(Scale::E scale);
    private:
        uint16 shift;
        Chan::E ch;
    };
};
