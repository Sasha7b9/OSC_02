#include "defines.h"
#include "HAL.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTime.h"
#include "Osci/Osci_Settings.h"
#include "FPGA/FPGA.h"


using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ADC_HandleTypeDef handle;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::ADC3_::Init()
{
    // ������ �������� ���� ��� �� �������������
    // PF10 - ADC3 IN8
    // PB11 - EXTI11
    // ����� ������:
    // - ��������� �� 1 ����������� ������
    // - ��������� ��������� �� ������ �������� ������� (���������� �� PB11)

    __ADC3_CLK_ENABLE();

    static GPIO_InitTypeDef isGPIOadc =
    {
        GPIO_PIN_10,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOF, &isGPIOadc);

    static GPIO_InitTypeDef isGPIOexti =
    {
        GPIO_PIN_11,
        GPIO_MODE_IT_RISING,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOB, &isGPIOexti);

    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    handle.Instance = ADC3;
    handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handle.Init.Resolution = ADC_RESOLUTION12b;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.ScanConvMode = DISABLE;
    handle.Init.EOCSelection = ENABLE;
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DMAContinuousRequests = DISABLE;
    handle.Init.NbrOfConversion = 1;
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.NbrOfDiscConversion = 0;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;

    if (HAL_ADC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handle, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_ADC_Start_IT(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


extern "C"
{
    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    void ADC_IRQHandler(void)
    {
        HAL_ADC_IRQHandler(&handle);
    }

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
    {
        /// \todo ��������� �������. �� � ������������� ��� ������� ������ �� ������ ����������

        if (IN_RANDOM_MODE)
        {
            FPGA::SetValueADC((uint16)HAL_ADC_GetValue(hadc));
        }
    }
}
