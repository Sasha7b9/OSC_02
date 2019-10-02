#pragma once
#include "defines.h"
#include "Data/DataSettings.h"


#define PRIORITY_SOUND_DMA1_STREAM5 5, 0

#define ERROR_HANDLER()                             ::HAL::ErrorHandler(__FILE__, __LINE__)

typedef void(*pFuncTester)();



namespace Hardware
{
    class Clock;
    class ADC1_;
}

namespace FPGA
{
    class ADC3_;
}

namespace Beeper
{
    class DAC1_;
}

namespace Timer
{
    class TIM2_;
    class TIM3_;
}

class Battery;

struct HAL_RTC
{
    static void Init();
    static PackedTime GetPackedTime();
    static bool SetPackedTime(const PackedTime &time);
};

namespace HAL
{
    void Init();

    void ErrorHandler(const char *, int);

    void Delay(uint timeMS);

    uint TimeMS();

    ////
    class ADC1_
    {
    friend class ::Battery;
    friend class Hardware::ADC1_;
    private:
        static void Init();

        static void SetActiveChannel2();

        static void SetActiveChannel9();

        static uint ReadValue();
    };

    class ADC3_
    {
    friend class FPGA::ADC3_;
    private:
        static void Init();
    };

    struct USART3_
    {
        static void Init(pFuncVV recvCallback);

        static void Transmit(void *buffer, uint size, uint timeout);

        static void StartReceiveIT(void *buffer, uint size);
    };

    class DAC1_
    {
    friend class Beeper::DAC1_;
    private:
        static void Init();

        static void StartDMA(void *points, uint numPoints);

        static void StopDMA();

        static void ConfigTIM7(uint16 prescaler, uint16 period);
    };

    class DAC2_
    {
    public:
        static void Init();
        static void SetValue(uint value);
    };

    ////
    class TIM2_
    {
    public:
        static uint TimeUS();

        static uint TimeTicks();

    friend class Timer::TIM2_;
    private:
        static void Init(uint prescaler, uint period);

        static void DeInit();

        static void Start();

        static void Stop();

        static void StartMultiMeasurement();
    };

    ////
    class TIM3_
    {
    friend class Timer::TIM3_;
    private:
        static void Init(uint prescaler, uint period);

        static void DeInit();

        static void Start();

        static void Stop();

        static void EnableIRQ(uint mainPriority, uint subPriority);

        static void DisableIRQ();

        static void StartIT(uint period);

        static void StopIT();
    };

    ////
    class EEPROM_
    {
    public:
        static void EraseSector(uint startAddress);
        static void WriteBytes(uint address, const uint8 *data, int size);
        static void WriteBufferBytes(uint address, void *buffer, int size);
    };

    ////
    class OTP_
    {
    public:
        /// ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
        static int GetSerialNumber(char buffer[17]);
        static bool SaveSerialNumber(char *servialNumber);

    };


    ////
    class CRC32_
    {
    public:
        static void Init();
        static uint Calculate(uint address, uint size);
    };

    ////
    class NVIC_
    {
    public:
        static uint irqEXTI1;
        static uint irqEXTI9_5;
        static void EnableIRQ(uint irq);
        static void DisableIRQ(uint irq);
        static void SetPriorityIRQ(uint irq, uint main, uint sub);
    };

    ////
    class FSMC
    {
    public:
        static void Init();

        static void WriteToFPGA8(uint8 *address, uint8 value);

        static void WriteToFPGA16(uint8 *address, uint16 value);

        static uint8 ReadFromFPGA(const uint8 *address);
    };

    ////
    namespace ADDRESSES
    {
        namespace FPGA
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
    }
}

