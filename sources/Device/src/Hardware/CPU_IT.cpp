#include "defines.h"
#include "Sound.h"
#include "Utils/Debug.h"
#include "FlashDrive/FlashDrive.h"      
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"

#include "Utils/Debug.h"
#include "Display/Painter.h"


using namespace Osci::Settings;

using Hardware::Timer;


#ifdef __cplusplus
extern "C" {
#endif
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HardFault_Handler()
{
    __IO const char *file = Debug::file;
    __IO int line = Debug::line;

    LOG_WRITE_AND_SHOW("%s %d", file, line);

    while (1)           // -V776
    {
        file = file;    // -V570
        line = line;    // -V570
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DMA1_Stream5_IRQHandler()
{
    HAL_DMA_IRQHandler(Sound::handleDAC.DMA_Handle1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(FPGA::HAL::_ADC::Handle());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    /// \todo ��������� �������. �� � ������������� ��� ������� ������ �� ������ ����������

    if (IN_RANDOM_MODE)
    {
        FPGA::SetValueADC((uint16)HAL_ADC_GetValue(hadc));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TIM3_IRQHandler()
{
    if ((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF)
    {
        if ((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
        {
            TIM3->SR = ~TIM_DIER_UIE;
            Timer::ElapsedCallback();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    HAL_IncTick();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// VCP
void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler(&VCP::handlePCD);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ������
void OTG_HS_IRQHandler()
{
    HAL_HCD_IRQHandler(&FDrive::handleHCD);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void NMI_Handler(void)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemManage_Handler()
{
    while (1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BusFault_Handler()
{
    while (1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UsageFault_Handler()
{
    while (1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SVC_Handler(void)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugMon_Handler(void)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PendSV_Handler(void)
{
}

#ifdef __cplusplus
}
#endif
