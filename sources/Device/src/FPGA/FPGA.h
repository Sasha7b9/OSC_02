#pragma once
#include "FPGATypes.h"
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

    void Update();

    void Start();

    void OnPressStart();

    void GiveStart();

    void Stop(bool pause = false);

    uint NumPoints();

    bool IsRunning();

    void Reset();

    void SetBandwidth(Chan::E ch);

    void SetValueADC(uint16 value);
    /// ������� ����������
    void DoCalibration();

    void ReadData();

    static uint16 ReadLastRecord();

    void ReadDataChanenl(Chan::E ch, uint8 data[FPGA_MAX_NUM_POINTS]);
    /// ������ ����� � ������������� � ������ address
    static void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data);

    static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);

    void ClearDataRand();

    static int CalculateShift();
    /// ����� ������ ������
    extern uint timeStart;
    /// ��������, ��������� �� handleADC
    extern uint16 valueADC;
    
    extern StateWorkFPGA fpgaStateWork;

    extern uint16 post;
    extern uint16 pred;

    extern bool isRunning;

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
