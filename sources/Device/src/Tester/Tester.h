#pragma once
#include "Command.h"
#include "Hardware/HAL/HAL_PIO.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
};
