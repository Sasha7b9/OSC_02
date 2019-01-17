#pragma once
#include "FPGA_Types.h"
#include "Settings/SettingsService.h"
#include <stm32f4xx_hal.h>


/// �� ������� ������ ����� �������� �������� ��������, ����� ������ �������� �� ���������� ������������ �� ���� �����.
#define STEP_RSHIFT     (((Osci::Settings::RShift::MAX - Osci::Settings::RShift::MIN) / 24) / 20)
#define STEP_TRIGLEV    STEP_RSHIFT

#define FPGA_IN_STATE_STOP (FPGA::fpgaStateWork == StateWorkFPGA_Stop)
#define FPGA_IN_STATE_WORK (FPGA::fpgaStateWork == StateWorkFPGA_Work)
#define FPGA_IN_STATE_WAIT (FPGA::fpgaStateWork == StateWorkFPGA_Wait)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    void Init();

    void OnPressStart();
    /// �������������� ������ �������������
    void GiveStart();

    uint NumPoints();

    bool IsRunning();

    void Reset();

    void SetValueADC(uint16 value);
    /// ������� ����������
    void DoCalibration();

    void ReadData();

    static uint16 ReadLastRecord();

    void ReadDataChanenl(Chan::E ch, uint8 data[FPGA_MAX_NUM_POINTS]);

    void ClearDataRand();
    /// ����� ������ ������
    extern uint timeStart;
    /// ��������, ��������� �� handleADC
    extern uint16 valueADC;
    
    extern StateWorkFPGA fpgaStateWork;

    extern bool isRunning;

    extern uint16 post;
    extern uint16 pred;

    extern struct State
    {
        bool needCalibration;                       ///< ������������� � true �������� ��������, ��� ���������� ���������� ����������.
        StateWorkFPGA stateWorkBeforeCalibration;
        StateCalibration stateCalibration;          ///< ������� ��������� ����������. ������������ � �������� ����������.
    } state;

    class FreqMeter
    {
    public:
        static float GetFreq();
    };

    class ForTester
    {
    public:
        static bool Read(uint8 *dataA, uint8 *dataB);
        /// ��������� ���� ������ ��� ������-����������. � ������� time ������ ������ ���� ������� numPoints �����
        static void Start();
    };
};
