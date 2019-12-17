#include "defines.h"
#include "common/Transceiver.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stm32f4xx_hal.h>


#define ADDR_ALTERA1    ((uint8 *)NOR_MEMORY_ADRESS1)
#define ADDR_ALTERA2    ((uint8 *)NOR_MEMORY_ADRESS2)
#define ADDR_ALTERA3    ((uint8 *)NOR_MEMORY_ADRESS3)
#define ADDR_DISPLAY    ((uint8 *)NOR_MEMORY_ADRESS4)


uint8 *HAL_FSMC::addrData0 = nullptr;
uint8 *HAL_FSMC::addrData1 = nullptr;


void HAL_FSMC::Init()
{
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    HAL_PIO::Init(PIN_PAN0, HMode::Input, HPull::Down);
    HAL_PIO::Init(PIN_PAN1, HMode::Input, HPull::Down);
    HAL_PIO::Init(PIN_NE4, HMode::Output_PP, HPull::Down);

    HAL_PIO::Set(PIN_NE4);

    GPIO_InitTypeDef is;
    is.Mode = GPIO_MODE_AF_PP;
    is.Pull = GPIO_PULLUP;
    is.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    is.Alternate = GPIO_AF12_FMC;

    // ������������� �������� ��� ����� ���� ������
    // GPIOD 14, 15, 0, 1 - D0, D1, D2, D3
    // GPIOE  7, 8, 9, 10 - D4, D5, D6, D7

    //GPIOD->PUPDR &= 0x0ffffff0U;     // �������� ���� 15, 14, 1, 0
    //GPIOD->PUPDR |= 0xa000000aU;     // ������������� ��� ���� ����� GPIO_PULLDOWN

    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &is);

    //GPIOE->PUPDR &= 0xffc03fffU;     // �������� ���� 7, 8, 9, 10
    //GPIOE->PUPDR |= 0x00268000U;     // ������������� ��� ���� ����� GPIO_PULLDOWN

    is.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &is);

    // �������� �������� ������ ��� ����

    //               A0           A1           A2           A3           A4           A5
    //isGPIO.Pin = GPIO _PIN_0 | GPIO _PIN_1 | GPIO _PIN_2 | GPIO _PIN_3 | GPIO _PIN_4 | GPIO _PIN_5;
    //HAL_GPIO_Init(GPIOF, &isGPIO);

    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOF, &is);

    GPIOF->AFR[0] &= HEX_FROM_2(ff00, 0000);
    GPIOF->AFR[0] |= HEX_FROM_2(00cc, cccc);    // ������������� GPIO_AF12_FMC

    GPIOF->MODER &= HEX_FROM_2(ffff, f000);
    GPIOF->MODER |= HEX_FROM_2(0000, 0aaa);     // ������������� Alternate function mode

    GPIOF->OSPEEDR |= HEX_FROM_2(0000, 0fff);   // ������������� very high speed

    GPIOF->OTYPER &= HEX_FROM_2(ffff, c000);    // ������������� output push-pull

    GPIOF->PUPDR &= HEX_FROM_2(ffff, f000);
    GPIOF->PUPDR |= HEX_FROM_2(0000, 0aaa);     // ������������� pull-down

    //               D4           D5           D6           D7
    //isGPIO.Pin = GPIO _PIN_7 | GPIO _PIN_8 | GPIO _PIN_9 | GPIO _PIN_10;
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
    FMC_NORSRAM_TimingTypeDef *Timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming);

    FMC_NORSRAM_Init(hsram->Instance, &(hsram->Init));

    FMC_NORSRAM_Timing_Init(hsram->Instance, Timing, hsram->Init.NSBank);

    hsram->Extended->BWTR[hsram->Init.NSBank] = 0x0FFFFFFFU;

    __FMC_NORSRAM_ENABLE(hsram->Instance, hsram->Init.NSBank);
}


void HAL_FSMC::Configure()
{
    DataBusMode::state = DataBusMode::FPGA;

    static GPIO_InitTypeDef isGPIO =
    {   //    NOE          NWE          NE1
        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FMC
    };

    /// \todo ����� �� �������� - �� �����, ������-��

    HAL_GPIO_Init(GPIOD, const_cast<GPIO_InitTypeDef *>(&isGPIO));

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

    isGPIO.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &isGPIO);

    isGPIO.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &isGPIO);
}


void HAL_FSMC::WriteToFPGA16(uint8 *address, uint16 value)
{
    if (DataBusMode::state != DataBusMode::FPGA)
    {
        Configure();
    }

    PAUSE_ON_TICKS(100);    /// \todo ��� ���� ������ �������� ����� ��� ���������� ����������� � TBase >= 0.5��

    *address = static_cast<uint8>(value);
    *(address + 1) = static_cast<uint8>(value >> 8);
}


void HAL_FSMC::WriteToFPGA8(uint8 *address, uint8 value)
{
    if (DataBusMode::state != DataBusMode::FPGA)
    {
        Configure();
    }
    
    *address = value;
}


uint8 HAL_FSMC::ReadFromFPGA(const uint8 *address)
{
    if (DataBusMode::state != DataBusMode::FPGA)
    {
        Configure();
    }

    return *address;
}


void HAL_FSMC::SetAddrData(uint8 *address0, uint8 *address1)
{
    addrData0 = address0;
    addrData1 = address1;

    if (DataBusMode::state != DataBusMode::FPGA)
    {
        Configure();
    }
}


uint8 HAL_FSMC::ReadData0()
{
    int delta = VALUE::AVE - static_cast<int>(*addrData0);

    uint8 result = static_cast<uint8>(VALUE::AVE - static_cast<int>(delta * GetStretch(addrData0)));

    Math::Limitation(&result, VALUE::MIN, VALUE::MAX);

    return result;
}


uint8 HAL_FSMC::ReadData1()
{
    int delta = VALUE::AVE - static_cast<int>(*addrData1);

    uint8 result = static_cast<uint8>(VALUE::AVE - static_cast<int>(delta * GetStretch(addrData0)));

    Math::Limitation(&result, VALUE::MIN, VALUE::MAX);

    return result;
}


float HAL_FSMC::GetStretch(const uint8 *address)
{
    if (StretchADC::IsDisabled())
    {
        return 1.0F;
    }

    static const float *stretchs[4] =
    {
        &StretchADC::Ref().stretch[Chan::A],
        &StretchADC::Ref().stretch[Chan::A],
        &StretchADC::Ref().stretch[Chan::B],
        &StretchADC::Ref().stretch[Chan::B]
    };

    int delta = address - RD::DATA_A;

    float result = *stretchs[delta];

    return result;
}
