#include "stdafx.h"
#ifdef WIN32
#ifndef DEVICE
#include "Recorder/Recorder_win.h"
#endif
#endif
#include <stm32f4xx_hal.h>



void HAL_GPIO_WritePin(GPIO_TypeDef *, uint16_t, GPIO_PinState)
{

}


void  HAL_GPIO_Init(GPIO_TypeDef  *, GPIO_InitTypeDef *)
{

}

void  HAL_GPIO_DeInit(GPIO_TypeDef  *, uint32_t)
{

}

#ifdef WIN32
#ifndef DEVICE

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if (GPIOx == GPIOG && GPIO_Pin == GPIO_PIN_1)
    {
        return RecorderHAL::ReadyPoint() ? GPIO_PIN_SET : GPIO_PIN_RESET;
    }

    return GPIO_PIN_RESET;
}

#endif
#else

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *, uint16_t)
{
    return GPIO_PIN_RESET;
}

#endif



void HAL_GPIO_EXTI_IRQHandler(uint16_t)
{

}
