#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handle = { DAC };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::DAC1_::Init()
{
    __DMA1_CLK_ENABLE();
    __TIM7_CLK_ENABLE();
    __DAC_CLK_ENABLE();

    GPIO_InitTypeDef structGPIO =
    {
        GPIO_PIN_4,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL,
        0, 0
    };

    HAL_GPIO_Init(GPIOA, &structGPIO);

    static DMA_HandleTypeDef hdmaDAC1 =
    {
        DMA1_Stream5,
        {
            DMA_CHANNEL_7,
            DMA_MEMORY_TO_PERIPH,
            DMA_PINC_DISABLE,
            DMA_MINC_ENABLE,
            DMA_PDATAALIGN_BYTE,
            DMA_MDATAALIGN_BYTE,
            DMA_CIRCULAR,
            DMA_PRIORITY_HIGH,
            DMA_FIFOMODE_DISABLE,
            DMA_FIFO_THRESHOLD_HALFFULL,
            DMA_MBURST_SINGLE,
            DMA_PBURST_SINGLE
        },
        HAL_UNLOCKED, HAL_DMA_STATE_RESET, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    HAL_DMA_Init(&hdmaDAC1);

    __HAL_LINKDMA(&handle, DMA_Handle1, hdmaDAC1);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, PRIORITY_SOUND_DMA1_STREAM5);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);


    DAC_ChannelConfTypeDef config =
    {
        DAC_TRIGGER_T7_TRGO,
        DAC_OUTPUTBUFFER_ENABLE
    };

    HAL_DAC_DeInit(&handle);

    HAL_DAC_Init(&handle);

    HAL_DAC_ConfigChannel(&handle, &config, DAC_CHANNEL_1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::DAC1_::StartDMA(void *points, uint numPoints)
{
    HAL_DAC_Start_DMA(&handle, DAC_CHANNEL_1, (uint32_t*)points, numPoints, DAC_ALIGN_8B_R);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::DAC1_::StopDMA()
{
    HAL_DAC_Stop_DMA(&handle, DAC_CHANNEL_1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::DAC1_::ConfigTIM7(uint16 prescaler, uint16 period)
{
    static TIM_HandleTypeDef htim =
    {
        TIM7, {}, HAL_TIM_ACTIVE_CHANNEL_1, {}, HAL_UNLOCKED, HAL_TIM_STATE_RESET
    };

    htim.Init.Prescaler = prescaler;
    htim.Init.Period = period;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&htim);

    TIM_MasterConfigTypeDef masterConfig =
    {
        TIM_TRGO_UPDATE,
        TIM_MASTERSLAVEMODE_DISABLE
    };

    HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);

    HAL_TIM_Base_Stop(&htim);
    HAL_TIM_Base_Start(&htim);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void InitGPIOforBell()
{
    GPIO_InitTypeDef structGPIO =
    {
        GPIO_PIN_4,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL,
        0, 0
    };

    HAL_GPIO_Init(GPIOA, &structGPIO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void InitTIMforBell()
{
    __TIM6_CLK_ENABLE();

    static TIM_HandleTypeDef  htim;
    TIM_MasterConfigTypeDef sMasterConfig;

    htim.Instance = TIM6;

    htim.Init.Period = 0x7FF;
    htim.Init.Prescaler = 0;
    htim.Init.ClockDivision = 0;
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&htim);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

    HAL_TIM_Base_Start(&htim);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void InitDMAforBell()
{
    static DMA_HandleTypeDef hdmaDAC1 =
    {
        DMA1_Stream5,
        {
            DMA_CHANNEL_7,
            DMA_MEMORY_TO_PERIPH,
            DMA_PINC_DISABLE,
            DMA_MINC_ENABLE,
            DMA_PDATAALIGN_BYTE,
            DMA_MDATAALIGN_BYTE,
            DMA_CIRCULAR,
            DMA_PRIORITY_HIGH,
            DMA_FIFOMODE_DISABLE,
            DMA_FIFO_THRESHOLD_HALFFULL,
            DMA_MBURST_SINGLE,
            DMA_PBURST_SINGLE
        },
        HAL_UNLOCKED, HAL_DMA_STATE_RESET, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    HAL_DMA_Init(&hdmaDAC1);

    __HAL_LINKDMA(&handle, DMA_Handle1, hdmaDAC1);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, PRIORITY_SOUND_DMA1_STREAM5);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void InitDACforBell()
{
    DAC_ChannelConfTypeDef config =
    {
        DAC_TRIGGER_T6_TRGO,
        DAC_OUTPUTBUFFER_ENABLE
    };

    HAL_DAC_DeInit(&handle);

    HAL_DAC_Init(&handle);

    HAL_DAC_ConfigChannel(&handle, &config, DAC_CHANNEL_1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void InitBell()
{
    __DMA1_CLK_ENABLE();
    __TIM6_CLK_ENABLE();
    __TIM7_CLK_ENABLE();
    __DAC_CLK_ENABLE();

    InitGPIOforBell();

    InitDMAforBell();

    InitDACforBell();

    InitTIMforBell();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::DAC1_::BellOn()
{
    InitBell();

    if (HAL_DACEx_TriangleWaveGenerate(&handle, DAC_CHANNEL_1, DAC_TRIANGLEAMPLITUDE_255) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_DAC_Start(&handle, DAC_CHANNEL_1) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_DAC_SetValue(&handle, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0x100) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::DAC1_::BellOff()
{

}


INTERRUPT_BEGIN

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DMA1_Stream5_IRQHandler()
{
    HAL_DMA_IRQHandler(handle.DMA_Handle1);
}

INTERRUPT_END
