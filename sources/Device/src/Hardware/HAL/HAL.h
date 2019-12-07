#pragma once
#include "defines.h"
#include "Data/DataSettings.h"
#include "Hardware/Memory/Sector.h"


#define PRIORITY_SOUND_DMA1_STREAM5 5, 0

#define ERROR_HANDLER()            ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL_RTC
{
    static void Init();
    static PackedTime GetPackedTime();
    static bool SetPackedTime(const PackedTime &time);
};

struct HAL_ADC1
{
    static void Init();
    /// ������ ��� �������
    static uint ReadValueAKK();
    /// ������ ��� ��������� ����������
    static uint ReadValuePOW();
private:
    static void SetActiveChannel2();
    static void SetActiveChannel9();
    static uint ReadValue();
};

struct HAL_ADC3
{
    static void Init();
};

struct HAL_DAC1
{
    static void Init();
    static void StartDMA(void *points, uint numPoints);
    static void StopDMA();
    static void ConfigTIM7(uint16 prescaler, uint16 period);
};

struct HAL_TIM2
{
    static uint TimeUS();
    static uint TimeTicks();
    static void Init(uint prescaler, uint period);
    static void DeInit();
    static void Start();
    static void Stop();
    static void StartMultiMeasurement();
};

struct HAL_TIM3
{
    static void Init(uint prescaler, uint period);
    static void DeInit();
    static void Start();
    static void Stop();
    static void EnableIRQ(uint mainPriority, uint subPriority);
    static void DisableIRQ();
    static void StartIT(uint period);
    static void StopIT();
};

struct HAL_USART3
{
    static void Init(pFuncVV recvCallback);
    static void Transmit(void *buffer, uint size, uint timeout);
    static void StartReceiveIT(void *buffer, uint size);
};

struct HAL_DAC2
{
    static void Init();
    static void SetValue(uint value);
};


struct HAL_ROM
{
    static const Sector sectors[Sector::Count];

    static void WriteBytes(uint address, const uint8 *data, uint size);
    static void WriteBufferBytes(uint address, const void *data, uint size);
};

struct HAL_OTP
{
    /// ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
    static int GetSerialNumber(char buffer[17]);
    static bool SaveSerialNumber(char *servialNumber);

};

struct HAL_CRC32
{
    static void Init();
    static uint Calculate(uint address, uint size);
};

struct HAL_NVIC
{
    static uint irqEXTI1;
    static uint irqEXTI9_5;
    static void EnableIRQ(uint irq);
    static void DisableIRQ(uint irq);
    static void SetPriorityIRQ(uint irq, uint main, uint sub);
    static void SystemReset();
};

struct HAL_FSMC
{
    static void Init();
    static void WriteToFPGA8(uint8 *address, uint8 value);
    static void WriteToFPGA16(uint8 *address, uint16 value);
    static uint8 ReadFromFPGA(const uint8 *address);

    /// ���������� ����� ��� ������ ������
    static void SetAddrData(uint8 *address0, uint8 *address1 = nullptr);
    /// ������ ������ � �������������� ������
    static uint8 ReadData0();
    static uint8 ReadData1();
private:
    /// ������ ����� ������ ������
    static uint8 *addrData0;
    /// ������ ����� ������ ������
    static uint8 *addrData1;
    /// ��������������� ��� ������ � ��������
    static void Configure();
    /// ���������� �������� ��� ������� ������
    static float GetStretch(const uint8 *address);
};

struct HAL
{
    static void Init();

    static void ErrorHandler(const char *, int);

    static void Delay(uint timeMS);

    static uint TimeMS();
};

