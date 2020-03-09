#include "defines.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"
#include <stm32f4xx_hal.h>



static ADC_HandleTypeDef handleIT =
{
    ADC3,
    {
        ADC_CLOCKPRESCALER_PCLK_DIV2,       // ClockPrescaler
        ADC_RESOLUTION12b,                  // Resolution
        ADC_DATAALIGN_RIGHT,                // DataAlign
        DISABLE,                            // ScanConvMode
        ENABLE,                             // EOCSelection
        DISABLE,                            // ContinuousConvMode
        1,                                  // NbrOfConversion
        DISABLE,                            // DiscontinuousConvMode
        0,                                  // NbrOfDiscConversion
        ADC_EXTERNALTRIGCONV_Ext_IT11,      // ExternalTrigConv
        ADC_EXTERNALTRIGCONVEDGE_RISING,    // ExternalTrigConvEdge
        DISABLE                             // DMAContinuousRequests
    }
};


static ADC_HandleTypeDef handlePoll =
{
    ADC3,
    {
        ADC_CLOCKPRESCALER_PCLK_DIV2,       // ClockPrescaler
        ADC_RESOLUTION12b,                  // Resolution
        ADC_DATAALIGN_RIGHT,                // DataAlign
        DISABLE,                            // ScanConvMode
        DISABLE,                            // EOCSelection
        DISABLE,                            // ContinuousConvMode
        1,                                  // NbrOfConversion
        DISABLE,                            // DiscontinuousConvMode
        0,                                  // NbrOfDiscConversion
        ADC_EXTERNALTRIGCONV_T1_CC1,        // ExternalTrigConv
        ADC_EXTERNALTRIGCONVEDGE_NONE,      // ExternalTrigConvEdge
        DISABLE                             // DMAContinuousRequests
    }
};


void HAL_ADC3::Init()
{
    // ������ �������� ���� ��� �� �������������
    // PF10 - ADC3 IN8
    // PB11 - EXTI11
    // ����� ������:
    // - ��������� �� 1 ����������� ������
    // - ��������� ��������� �� ������ �������� ������� (���������� �� PB11)

    __ADC3_CLK_ENABLE();

    HAL_PIO::Init(PIN_ADC3, HMode::Analog, HPull::No);

    HAL_PIO::Init(PIN_ADC3_IT, HMode::RisingIT, HPull::No);

    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    if (HAL_ADC_Init(&handleIT) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handleIT, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_ADC_Start_IT(&handleIT) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


INTERRUPT_BEGIN


void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&handleIT);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef*)
{
    /// \todo ��������� �������. �� � ������������� ��� ������� ������ �� ������ ����������

    if (OSCI_IN_MODE_RANDOMIZER)
    {
//        Osci::valueADC = static_cast<uint16>(HAL_ADC_GetValue(&handleIT));
//
//        HAL_ADC_Stop_IT(&handleIT);
//        HAL_ADC_Start_IT(&handleIT);

        uint start = TIME_US;

        uint16 val1 = static_cast<uint16>(ADC3->DR);
        
        HAL_ADC_Stop_IT(&handleIT);
        
        HAL_ADC_Init(&handlePoll);

        HAL_ADC_Start(&handlePoll);
        
        HAL_ADC_PollForConversion(&handlePoll, 10);
        
        uint16 val2 = static_cast<uint16>(ADC3->DR);
        
        Osci::valueADC = (uint16)(((float)val1 + val2 + 0.5F) / 2.0F);
        
        HAL_ADC_Init(&handleIT);

        HAL_ADC_Start_IT(&handleIT);

        static int counter = 0;

        if(counter > 1000)
        {
            uint time = TIME_US - start;

            LOG_WRITE("1 = %d, 2 = %d, adc = %d", val1, val2, Osci::valueADC);
            counter = 0;
            LOG_WRITE("����� %d ���", time);
        }

        counter++;
    }
}

INTERRUPT_END
