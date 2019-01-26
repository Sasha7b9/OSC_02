#include "defines.h"
#include "HAL.h"


using namespace HAL;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��� ������� ���������� ������������
static ADC_HandleTypeDef handle;
/// ��� ������������
static ADC_ChannelConfTypeDef config =
{
    ADC_CHANNEL_2,
    1,
    ADC_SAMPLETIME_3CYCLES,
    0
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ADC1_::Init()
{
    // �������� ����� ��� ��� �������� ����������
    // 36 : PA2 - ADC1 IN2 - �������� ���
    // 47 : PB1 - ADC1 IN9 - �������� ���������
    // ����� ������:

    __ADC1_CLK_ENABLE();

    static GPIO_InitTypeDef isGPIOadc =
    {
        GPIO_PIN_2,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOA, &isGPIOadc);

    isGPIOadc.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &isGPIOadc);

    handle.Instance = ADC1;
    handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handle.Init.Resolution = ADC_RESOLUTION_12B;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.ScanConvMode = DISABLE;
    handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DMAContinuousRequests = DISABLE;
    handle.Init.NbrOfConversion = 1;
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.NbrOfDiscConversion = 0;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;  // ����������� ������ �������������� �������
    handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    if (HAL_ADC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    SetActiveChannel2();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint ADC1_::ReadValue()
{
    if (HAL_ADC_Start(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    do
    {
    } while (HAL_ADC_PollForConversion(&handle, 1) != HAL_OK);

    return HAL_ADC_GetValue(&handle);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ADC1_::SetActiveChannel2()
{
    config.Channel = ADC_CHANNEL_2;

    HAL_ADC_ConfigChannel(&handle, &config);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ADC1_::SetActiveChannel9()
{
    config.Channel = ADC_CHANNEL_9;

    HAL_ADC_ConfigChannel(&handle, &config);
}
