#include "defines.h"

#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


using namespace HAL;



#define VALUE_FOR_RTC 0x644


#define RTC_CLOCK_SOURCE_LSE
// #define RTC_CLOCK_SOURCE_LSI


#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV 0x7f
#define RTC_SYNCH_PREDIV 0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV 0x7f
#define RTC_SYNCH_PREDIV 0x00ff
#endif



static RTC_HandleTypeDef handleRTC =
{
    RTC,
    {
        RTC_HOURFORMAT_24,          // HourFormat
        RTC_ASYNCH_PREDIV,          // AsynchPrediv
        RTC_SYNCH_PREDIV,           // SynchPrediv
        RTC_OUTPUT_DISABLE,         // OutPut
        RTC_OUTPUT_POLARITY_HIGH,   // OutPutPolarity
        RTC_OUTPUT_TYPE_OPENDRAIN   // OutPutType
    }
};



void HAL_RTC::Init()
{
    RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    __HAL_RCC_RTC_ENABLE();

    HAL_StatusTypeDef status = HAL_RTC_Init((RTC_HandleTypeDef*)&handleRTC);

    if (status != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_RTCEx_BKUPRead((RTC_HandleTypeDef*)&handleRTC, RTC_BKP_DR0) != VALUE_FOR_RTC)
    {
        if (SetPackedTime(PackedTime()))
        {
            HAL_RTCEx_BKUPWrite((RTC_HandleTypeDef*)&handleRTC, RTC_BKP_DR0, VALUE_FOR_RTC);
        }
    }
}


PackedTime HAL_RTC::GetPackedTime()
{
    PackedTime time;

    RTC_TimeTypeDef isTime;

    HAL_RTC_GetTime((RTC_HandleTypeDef*)&handleRTC, &isTime, FORMAT_BIN);

    time.hours = isTime.Hours;
    time.minutes = isTime.Minutes;
    time.seconds = isTime.Seconds;

    RTC_DateTypeDef isDate;
    HAL_RTC_GetDate((RTC_HandleTypeDef*)&handleRTC, &isDate, FORMAT_BIN);

    time.year = isDate.Year;
    time.month = isDate.Month;
    time.day = isDate.Date;

    time.timeMS = TIME_MS;

    return time;
}


bool HAL_RTC::SetPackedTime(const PackedTime &time)
{
    RTC_DateTypeDef dateStruct;
    dateStruct.WeekDay = RTC_WEEKDAY_MONDAY;
    dateStruct.Month = (uint8)time.month;
    dateStruct.Date = (uint8)time.day;
    dateStruct.Year = (uint8)time.year;

    if (HAL_RTC_SetDate((RTC_HandleTypeDef*)&handleRTC, &dateStruct, FORMAT_BIN) != HAL_OK)
    {
        return false;
    };

    RTC_TimeTypeDef timeStruct;
    timeStruct.Hours = (uint8)time.hours;
    timeStruct.Minutes = (uint8)time.minutes;
    timeStruct.Seconds = (uint8)time.seconds;
    timeStruct.TimeFormat = RTC_HOURFORMAT_24;
    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    timeStruct.StoreOperation = RTC_STOREOPERATION_SET;

    if (HAL_RTC_SetTime((RTC_HandleTypeDef*)&handleRTC, &timeStruct, FORMAT_BIN) != HAL_OK)
    {
        return false;
    };

    return true;
}
