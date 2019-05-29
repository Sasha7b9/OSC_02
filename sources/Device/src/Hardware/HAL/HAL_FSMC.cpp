#include <stm32f4xx_hal.h>
#include "defines.h"
#include "DataBus.h"
#include "HAL.h"
#include "Hardware/Timer.h"
#include "Keyboard/DecoderDevice.h"
#include "Utils/Debug.h"
#include "Osci/Osci.h"

#include "Recorder/Recorder.h"


using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ADDR_ALTERA1    ((uint8 *)NOR_MEMORY_ADRESS1)
#define ADDR_ALTERA2    ((uint8 *)NOR_MEMORY_ADRESS2)
#define ADDR_ALTERA3    ((uint8 *)NOR_MEMORY_ADRESS3)
#define ADDR_DISPLAY    ((uint8 *)NOR_MEMORY_ADRESS4)

#define PORT_NE4        GPIOG
#define PIN_NE4         GPIO_PIN_12

#define PORT_PAN_1      GPIOC
#define PIN_PAN_1       GPIO_PIN_4

#define PORT_PAN_0      GPIOA
#define PIN_PAN_0       GPIO_PIN_7

#define NE4_SET         (PORT_NE4->BSRR = PIN_NE4)
#define NE4_RESET       (PORT_NE4->BSRR = (uint)PIN_NE4 << 16U)

#define PAN_BUSY                        (ReadPAN() == 0)
#define PAN_READY_TRANSMIT              (ReadPAN() == 1)
#define PAN_READY_RECEIVE               (ReadPAN() == 2)
#define PAN_RECIEVE_TRANSMIT_CONFIRM    (ReadPAN() == 3)

static void Configure();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FSMC::Init()
{
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =               // PAN_0
    {
        PIN_PAN_0,
        GPIO_MODE_INPUT,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(PORT_PAN_0, &isGPIO);

    isGPIO.Pin = PIN_PAN_1;                 // PAN_1
    HAL_GPIO_Init(PORT_PAN_1, &isGPIO);

    isGPIO.Pin = PIN_NE4;
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_NE4, &isGPIO);        // NE4

    NE4_SET;

    // ������������� �������� ��� ����� ���� ������
    // GPIOD 14, 15, 0, 1 - D0, D1, D2, D3
    // GPIOE  7, 8, 9, 10 - D4, D5, D6, D7

    GPIOD->PUPDR &= 0x0ffffff0U;     // �������� ���� 15, 14, 1, 0
    GPIOD->PUPDR |= 0xa000000aU;     // ������������� ��� ���� ����� GPIO_PULLDOWN

    GPIOE->PUPDR &= 0xffc03fffU;     // �������� ���� 7, 8, 9, 10
    GPIOE->PUPDR |= 0x00268000U;     // ������������� ��� ���� ����� GPIO_PULLDOWN

    // �������� �������� ������ ��� ����

    //               A0           A1           A2           A3           A4           A5
    //isGPIO.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    //HAL_GPIO_Init(GPIOF, &isGPIO);

    GPIOF->AFR[0] &= HEX_FROM_2(ff00, 0000);
    GPIOF->AFR[0] |= HEX_FROM_2(00cc, cccc);    // ������������� GPIO_AF12_FMC

    GPIOF->MODER &= HEX_FROM_2(ffff, f000);
    GPIOF->MODER |= HEX_FROM_2(0000, 0aaa);     // ������������� Alternate function mode

    GPIOF->OSPEEDR |= HEX_FROM_2(0000, 0fff);   // ������������� very high speed

    GPIOF->OTYPER &= HEX_FROM_2(ffff, c000);    // ������������� output push-pull

    GPIOF->PUPDR &= HEX_FROM_2(ffff, f000);
    GPIOF->PUPDR |= HEX_FROM_2(0000, 0aaa);     // ������������� pull-down

    //               D4           D5           D6           D7
    //isGPIO.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    //HAL_GPIO_Init(GPIOE, &isGPIO);

    GPIOE->OSPEEDR |= HEX_FROM_2(003f, 8000);

    GPIOE->OTYPER &= HEX_FROM_2(ffc0, 3fff);

    GPIOE->PUPDR &= HEX_FROM_2(ffc0, 3fff);
    GPIOE->PUPDR |= HEX_FROM_2(003a, 8fff);

    static SRAM_HandleTypeDef gSramHandle =
    {
        FMC_NORSRAM_DEVICE,
        FMC_NORSRAM_EXTENDED_DEVICE,
        {
            FMC_NORSRAM_BANK1,                 // Init.NSBank
            FMC_DATA_ADDRESS_MUX_DISABLE,      // Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_8,       // Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE            // Init.WriteBurst
        }
    };

    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        0,                 // FSMC_AddressSetupTime
        0,                 // FSMC_AddressHoldTime
        4,                 // FSMC_DataSetupTime   ��� �������� 9 32�� ������������ � RAM �� 1000���. ����������
                           // �� ���� ������ ��������� ���� �������� �� 90 ���. ���� 3 - 32�� ��������� �� 460 ���.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    SRAM_HandleTypeDef *hsram = &gSramHandle;
    FMC_NORSRAM_TimingTypeDef *Timing = (FMC_NORSRAM_TimingTypeDef *)&sramTiming;

    FMC_NORSRAM_Init(hsram->Instance, &(hsram->Init));

    FMC_NORSRAM_Timing_Init(hsram->Instance, Timing, hsram->Init.NSBank);

    hsram->Extended->BWTR[hsram->Init.NSBank] = 0x0FFFFFFFU;

    __FMC_NORSRAM_ENABLE(hsram->Instance, hsram->Init.NSBank);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Configure()
{
    static const GPIO_InitTypeDef isGPIO =
    {   //    NOE          NWE          NE1
        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_PULLDOWN,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FMC
    };

    /// \todo ����� �� �������� - �� �����, ������-��

    HAL_GPIO_Init(GPIOD, (GPIO_InitTypeDef *)&isGPIO);

    //uint startTime = TIME_TICKS;

    // �������������� GPIOD 0, 1, 4, 5, 7, 14, 15 - D2, D3, NOE, NWE, NE1, D0, D1

    // �������������� GPIOD 0, 1, 4, 5 - D2, D3, NOE, NWE
    GPIOD->AFR[0] &= HEX_FROM_2(ffff, ff00);
    GPIOD->AFR[0] |= HEX_FROM_2(0000, 00cc);

    GPIOD->MODER &= HEX_FROM_2(ffff, fff0);
    GPIOD->MODER |= HEX_FROM_2(0000, 000a);

    // �������������� GPIOD 14, 15 - D0, D1
    GPIOD->AFR[1] &= HEX_FROM_2(00ff, ffff);    // PIN_14, PIN_15
    GPIOD->AFR[1] |= HEX_FROM_2(cc00, 0000);    // GPIO_AF12_FMC

    GPIOD->MODER &= HEX_FROM_2(0fff, ffff);
    GPIOD->MODER |= HEX_FROM_2(a000, 0000);     // Alternate function mode

    // �������������� GPIOE 7...8 - D4...D7
    GPIOE->AFR[0] &= HEX_FROM_2(0fff, ffff);
    GPIOE->AFR[0] |= HEX_FROM_2(c000, 0000);
    GPIOE->AFR[1] &= HEX_FROM_2(ffff, f000);
    GPIOE->AFR[1] |= HEX_FROM_2(0000, 0ccc);    // GPIO_AF12_FMC

    GPIOE->MODER &= HEX_FROM_2(ffc0, 3fff);
    GPIOE->MODER |= HEX_FROM_2(002a, 8fff);     // Alternate function mode
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteToFPGA16(uint8 *address, uint16 value)
{
    if (!DataBus::GetMode().IsFPGA())
    {
        Configure();
    }

    PAUSE_ON_TICKS(100);    /// \todo ��� ���� ������ �������� ����� ��� ���������� ����������� � TBase >= 0.5��

    *address = (uint8)value;
    *(address + 1) = (uint8)(value >> 8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteToFPGA8(uint8 *address, uint8 value)
{
    if (!DataBus::GetMode().IsFPGA())
    {
        Configure();
    }

    *address = value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 FSMC::ReadFromFPGA(const uint8 *address)
{
    if (!DataBus::GetMode().IsFPGA())
    {
        Configure();
    }

    return *address;
}
