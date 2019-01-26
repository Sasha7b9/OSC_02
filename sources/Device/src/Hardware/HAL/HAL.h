#pragma once
#include "defines.h"
#include "Data/DataSettings.h"
#include <stm32f4xx_hal.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ERROR_HANDLER()                             ::HAL::ErrorHandler(__FILE__, __LINE__)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Hardware
{
    class Clock;
    class Battery;
    class BatteryPrivate;
}

namespace HAL
{
    void ErrorHandler(const char *, int);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class RTC_
    {
    friend class Hardware::Clock;
    private:
        static void Init();
        static PackedTime GetPackedTime();
        static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class ADC1_
    {
    friend class Hardware::Battery;
    friend class Hardware::BatteryPrivate;
    private:
        static void Init();

        static void SetActiveChannel2();

        static void SetActiveChannel9();

        static uint ReadValue();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class UART_
    {
    public:
        static void Init(pFuncVV recvCallback);

        static void Transmit(void *buffer, uint size, uint timeout);

        static void StartReceiveIT(void *buffer, uint size);
    };
}
