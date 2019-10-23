#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>


TIM_HandleTypeDef handlerTIM2 = { TIM2 };
TIM_HandleTypeDef handlerTIM3 = { TIM3 };


void HAL_TIM2::Init(uint prescaler, uint period)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    handlerTIM2.Init.Prescaler = prescaler;
    handlerTIM2.Init.CounterMode = TIM_COUNTERMODE_UP;
    handlerTIM2.Init.Period = period;
    handlerTIM2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_DeInit(&handlerTIM2);
}


void HAL_TIM3::Init(uint prescaler, uint period)
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    handlerTIM3.Init.Prescaler = prescaler;
    handlerTIM3.Init.CounterMode = TIM_COUNTERMODE_UP;
    handlerTIM3.Init.Period = period;
    handlerTIM3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_DeInit(&handlerTIM3);

}


void HAL_TIM2::Start()
{
    HAL_TIM_Base_Start(&handlerTIM2);
}


void HAL_TIM3::Start()
{
    HAL_TIM_Base_Start(&handlerTIM3);
}


void HAL_TIM2::Stop()
{
    HAL_TIM_Base_Stop(&handlerTIM2);
}


void HAL_TIM3::Stop()
{
    HAL_TIM_Base_Stop(&handlerTIM3);
}


void HAL_TIM2::DeInit()
{
    __HAL_RCC_TIM2_CLK_DISABLE();
}


void HAL_TIM3::DeInit()
{
    __HAL_RCC_TIM3_CLK_DISABLE();
}


void HAL_TIM3::EnableIRQ(uint mainPriority, uint subPriority)
{
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    HAL_NVIC_SetPriority(TIM3_IRQn, mainPriority, subPriority);
}


void HAL_TIM3::DisableIRQ()
{
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
}


void HAL_TIM3::StartIT(uint period)
{
    handlerTIM3.Init.Period = period;
    HAL_TIM_Base_Init(&handlerTIM3);
    HAL_TIM_Base_Start_IT(&handlerTIM3);
}


void HAL_TIM3::StopIT()
{
    HAL_TIM_Base_Stop_IT(&handlerTIM3);
}


uint HAL_TIM2::TimeUS()
{
    return TIM2->CNT / 90;
}


uint HAL_TIM2::TimeTicks()
{
    return TIM2->CNT;
}


void HAL_TIM2::StartMultiMeasurement()
{
    TIM2->CR1 &= (uint)~TIM_CR1_CEN;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}
