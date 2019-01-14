#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA_HAL.h"
#include "Hardware/FSMC.h"
#include "Hardware/GPIO.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTime.h"
#include "Utils/Math.h"
#endif


using namespace FPGA::HAL::GPIO;

using FPGA::HAL::GPIO::Pin;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ADC_HandleTypeDef handleADC;

struct PinStruct
{
    GPIO_TypeDef   *gpioTD;
    uint            pin;
};

static PinStruct pins[Pin::Number] =
{
    {GPIOC, GPIO_PIN_10},   // SPI3_SCK
    {GPIOC, GPIO_PIN_12},   // SPI3_DAT
    {GPIOD, GPIO_PIN_3},    // SPI3_CS1
    {GPIOG, GPIO_PIN_13},   // SPI3_CS2
    {GPIOD, GPIO_PIN_10},   // A1
    {GPIOD, GPIO_PIN_11},   // A2
    {GPIOD, GPIO_PIN_12},   // A3
    {GPIOD, GPIO_PIN_13},   // A4
    {GPIOG, GPIO_PIN_2},    // LF1
    {GPIOG, GPIO_PIN_3},    // LF2
    {GPIOG, GPIO_PIN_4},    // A1S
    {GPIOG, GPIO_PIN_5},    // A0S
    {GPIOG, GPIO_PIN_6}     // LFS
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::HAL::LoadRegUPR()
{
    uint8 data = 0;

    if (SET_PEAKDET_EN)
    {
        data |= 1 << BIT_UPR_PEAK;
    }

    static const uint8 mask[3] =
    {
        (1 << BIT_UPR_CALIBR_AC_DC),
        (1 << BIT_UPR_CALIBR_ZERO),
        (0)
    };

    data |= mask[CALIBRATOR_MODE];

    FSMC::WriteToFPGA8(WR_UPR, data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ADC_HandleTypeDef *FPGA::HAL::HandleADC()
{
    return &handleADC;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::Init()
{
    GPIO_InitTypeDef isGPIO =
    {
        0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };

    for (int i = 0; i < Pin::Number; i++)
    {
        isGPIO.Pin = GetPin((Pin::E)i);
        HAL_GPIO_Init(GetPort((Pin::E)i), &isGPIO);
    }

    for (int i = 0; i < Pin::Number; i++)
    {
        gpio.SetOutputPP_PullDown(GetPort((Pin::E)i), (uint)Math::LowSignedBit(GetPin((Pin::E)i)));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::HAL::GPIO::GetPin(Pin::E pin)
{
    return pins[pin].pin;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
GPIO_TypeDef *FPGA::HAL::GPIO::GetPort(Pin::E pin)
{
    return pins[pin].gpioTD;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::WriteRegisters(Pin::E cs, uint16 value)
{
    ResetPin(cs);

    if (cs == Pin::SPI3_CS1)
    {
        for (int i = 15; i >= 0; --i)
        {
            WritePin(Pin::SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(Pin::SPI3_SCK);
            ResetPin(Pin::SPI3_SCK);
        }
    }
    else if (cs == Pin::SPI3_CS2)
    {
        for (int i = 0; i < 16; ++i)
        {
            WritePin(Pin::SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(Pin::SPI3_SCK);
            ResetPin(Pin::SPI3_SCK);
        }
    }
    else
    {
        // ��� ��������
    }

    SetPin(cs);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::WritePin(Pin::E pin, int enable)
{
    if (enable)
    {
        GetPort(pin)->BSRR = GetPin(pin);
    }
    else
    {
        GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::SetPin(Pin::E pin)
{
    GetPort(pin)->BSRR = GetPin(pin);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::ResetPin(Pin::E pin)
{
    GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
}
