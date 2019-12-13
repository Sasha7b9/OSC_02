#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


void HAL_PCD::Init()
{
    __GPIOA_CLK_ENABLE();
    __USB_OTG_FS_CLK_ENABLE(); //-V760
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_11 | GPIO_PIN_12,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF10_OTG_FS
    };

    HAL_GPIO_Init(GPIOA, &isGPIO);

    HAL_PIO::Init(PIN_PCD_VBUS, HMode::Input, HPull::No, HSpeed::VeryHigh);

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);

    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}
