#pragma once
#include "FPGATypes.h"
#include "Settings/SettingsService.h"
#include <stm32f4xx_hal.h>


/// �� ������� ������ ����� �������� �������� ��������, ����� ������ �������� �� ���������� ������������ �� ���� �����.
#define STEP_RSHIFT     (((FPGA::SET::RShift::MAX - FPGA::SET::RShift::MIN) / 24) / 20)
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

    static void ReadFlag();

    static uint16 ReadLastRecord();

    void ReadDataChanenl(Chan::E ch, uint8 data[FPGA_MAX_NUM_POINTS]);
    /// ������ ����� � ������������� � ������ address
    static void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data);

    static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);

    void ClearDataRand();

    static int CalculateShift();
    /// True, ���� ��� ������
    extern bool givingStart;
    /// ����� ������ ������
    extern uint timeStart;

    extern uint16 flag;
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

    struct Flag
    {
        enum E
        {
            _DATA_READY        = 0,  ///< ������ ������ ��� ���������� (��������� �������� ������������)
            _TRIG_READY        = 1,  ///< ���� ��������������
            _PRED              = 2,  ///< ���� 1, �� ���������� ��������, ����� ������ �������������� ������ (���������
            _P2P               = 3,  ///< 
            _FREQ_READY        = 4,  ///< ���� ���������� ��������� �������
            _PERIOD_READY      = 5,  ///< ���� ���������� ��������� �������
            _HOLD_OFF_FLAG     = 7,  ///< �������������� ������ ����� ������ ������ ����� ���� ���� � �������
            _FREQ_OVERFLOW     = 8,  ///< ������� ������������ �������� �������
            _PERIOD_OVERFLOW   = 9,  ///< ������� ������������ �������� �������
            _FREQ_IN_PROCESS   = 10, ///< ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
            _PERIOD_IN_PROCESS = 11  ///< ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
        } value;
    };

    struct GetFlag
    {
        static bool DATA_READY();
        static bool TRIG_READY();
        static bool PRED();
        static bool P2P();
        static bool FREQ_READY();
        static bool HOLD_OFF();
        static bool PERIOD_READY();
        static bool FREQ_OVERFLOW();
        static bool PERIOD_OVERFLOW();
        static bool FREQ_IN_PROCESS();
        static bool PERIOD_IN_PROCESS();
    };

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
