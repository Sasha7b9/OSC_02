#pragma once
#include "Settings/SettingsOsci.h"
#include "FPGA/Context.h"


#define FPGA_IN_STATE_STOP (FPGA::fpgaStateWork == StateWorkFPGA_Stop)
#define FPGA_IN_STATE_WORK (FPGA::fpgaStateWork == StateWorkFPGA_Work)
#define FPGA_IN_STATE_WAIT (FPGA::fpgaStateWork == StateWorkFPGA_Wait)


namespace Address
{
    namespace WR
    {
        extern uint8 *START;        ///< �����
        extern uint8 *TBASE;        ///< ��������
        extern uint8 *PRED_LO;      ///< ����������
        extern uint8 *PRED_HI;
        extern uint8 *POST_LO;      ///< �����������
        extern uint8 *POST_HI;

        extern uint8 *UPR;          ///< ����������
#define BIT_UPR_RAND            0       ///< ���. ������������
#define BIT_UPR_PEAK            1       ///< ������� ��������
#define BIT_UPR_CALIBR_AC_DC    2       ///< ���������� AC/DC
#define BIT_UPR_CALIBR_ZERO     3       ///< ���������� 0/4�

        extern uint8 *FREQMETER;    ///< ���������� ������������
        extern uint8 *TRIG;         ///< ���������� ��������������/��������
#define BIT_TRIG_POLARITY       0       ///< ���������� ����������� �������������
#define BIT_TRIG_ENABLED        1       ///< ��� ��������� ������������. 0 - ���, 1 - ����

        extern uint8 *START_ADDR;               ///< \brief ����� ������. ����� ����������� �� ����� ������, ����� ������ ������ � 
                                                ///  ������������� ������ (������� �� ���������� � ����������)
        extern uint8 *RESET_COUNTER_FREQ;       ///< ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������
        extern uint8 *RESET_COUNTER_PERIOD;     ///< ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������

        extern uint8 *TRIG_HOLD_ENABLE;         ///< ���������� ��������� �������������. "0" - ����, "1" - ���
        extern uint8 *TRIG_HOLD_VALUE_LOW;      ///< ������� ���� �������� �������������
        extern uint8 *TRIG_HOLD_VALUE_MID;      ///<    �������� �������������� �� ������� N = 0 - (�������� / 100��). ������������ �������� - 1.5 ���
        extern uint8 *TRIG_HOLD_VALUE_HI;       ///< ������� ���� �������� �������������
    }

    namespace RD
    {
        extern uint8 *DATA_A;               ///< ������ ������� ������
        extern uint8 *DATA_A_PEAK_MAX;      ///< ������ �������� ��������� ������� ������
        extern uint8 *DATA_A_PEAK_MIN;
        extern uint8 *DATA_B;               ///< ������ ������� ������
        extern uint8 *DATA_B_PEAK_MAX;      ///< ������ ������� ��������� ������� ������
        extern uint8 *DATA_B_PEAK_MIN;
        extern uint8 *LAST_RECORD_LO;       ///< ����� ��������� ������
        extern uint8 *LAST_RECORD_HI;
        extern uint8 *FREQ_BYTE_0;          ///< ���������� �������� �������
        extern uint8 *FREQ_BYTE_1;
        extern uint8 *FREQ_BYTE_2;
        extern uint8 *FREQ_BYTE_3;
        extern uint8 *PERIOD_BYTE_0;        ///< ���������� �������� �������
        extern uint8 *PERIOD_BYTE_1;
        extern uint8 *PERIOD_BYTE_2;
        extern uint8 *PERIOD_BYTE_3;
        extern uint8 *FLAG_LO;              ///< ������� ���� �����
        extern uint8 *FLAG_HI;              ///< ������� ���� �����
    }
};

struct Flag
{
    enum E
    {
        _DATA_READY = 0,  ///< ������ ������ ��� ���������� (��������� �������� ������������)
        _TRIG_READY = 1,  ///< ���� ��������������
        _PRED = 2,  ///< ���� 1, �� ���������� ��������, ����� ������ �������������� ������ (���������
        _P2P = 3,  ///< 
        _FREQ_READY = 4,  ///< ���� ���������� ��������� �������
        _PERIOD_READY = 5,  ///< ���� ���������� ��������� �������
        _HOLD_OFF_FLAG = 7,  ///< �������������� ������ ����� ������ ������ ����� ���� ���� � �������
        _FREQ_OVERFLOW = 8,  ///< ������� ������������ �������� �������
        _PERIOD_OVERFLOW = 9,  ///< ������� ������������ �������� �������
        _FREQ_IN_PROCESS = 10, ///< ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
        _PERIOD_IN_PROCESS = 11  ///< ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
    } value;
};

struct FPin
{
    enum E
    {
        SPI3_SCK,
        SPI3_DAT,
        SPI3_CS1,
        SPI3_CS2,
        A1,
        A2,
        A3,
        A4,
        LF1,
        LF2,
        A1S,
        A0S,
        LFS,
        Number
    } value;
    explicit FPin(E v) : value(v) {};
};


struct FPGA
{
    static const uint MAX_NUM_POINTS = (16 * 1024U);

    static void Init();

    static void OnPressStart();
    /// �������������� ������ �������������
    static void GiveStart();
    /// ���������� ���������� ���� � ������ �� ������� ����������
    static uint BytesInChannel();

    static bool IsRunning();

    static void Reset();

    static void SetValueADC(uint16 value);

    static void ReadData();

    static uint16 ReadLastRecord(Chan::E ch);

    static bool ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS]);

    static void ClearDataRand();
    /// ��������/��������� ����������.
    static void LoadCalibratorMode();
    /// ����� ������ ������
    static uint timeStart;
    /// ��������, ��������� �� handleADC
    static uint16 valueADC;
    
    static StateWorkFPGA fpgaStateWork;

    static bool isRunning;

    static uint16 post;
    static uint16 pred;

    struct FreqMeter
    {
        static float GetFreq();
    };

    struct ForTester
    {
        static bool Read(uint16 *dataA, uint8 *dataB);
        /// ��������� ���� ������ ��� ������-����������. � ������� time ������ ������ ���� ������� numPoints �����
        /// ���� ���������� false - ����� �� ������
        static bool Start();
    };

    struct VALUE
    {
        static const uint8 AVE = 127;
        static const uint8 MIN = AVE - 125;
        static const uint8 MAX = AVE + 125;
        /// ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������
        static const uint8 NONE = 0;
    };

    struct GetFlag
    {
        static bool DATA_READY();
        static bool TRIG_READY();
        static bool PRED();
        static bool FREQ_READY();
        static bool HOLD_OFF();
        static bool PERIOD_READY();
        static bool FREQ_OVERFLOW();
        static bool PERIOD_OVERFLOW();
        static bool FREQ_IN_PROCESS();
        static bool PERIOD_IN_PROCESS();
    };

    static void LoadRegUPR();

    static void ReadFlag();

    static uint16 flag;

    struct GPIO
    {
        static void Init();
        static uint16 GetPin(FPin::E pin);
        static void WritePin(FPin::E pin, int enable);
        static void SetPin(FPin::E pin);
        static void ResetPin(FPin::E pin);
        static void WriteRegisters(FPin::E cs, uint16 value);
    };
};

namespace MathFPGA
{
    float RShift2Abs(int rShift, Range::E range);

    float VoltageCursor(float shiftCurU, Range::E range, uint16 rShift);

    int RShift2Rel(float rShixftAbs, Range::E range);

    float TimeCursor(float shiftCurT, TBase::E tBase);

    void PointsRel2Voltage(const uint8 *points, int numPoints, Range::E range, int16 rShift, float *voltage);

    uint8 Voltage2Point(float voltage, Range::E range, uint16 rShift);

    float Point2Voltage(uint8 value, Range::E range, uint16 rShift);

    void PointsVoltage2Rel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);

    float TShift2Abs(int tShift, TBase::E tBase);
}
