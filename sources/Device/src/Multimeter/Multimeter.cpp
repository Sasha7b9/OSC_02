#include "defines.h"
#include "Multimeter.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Device.h"
#include <cstring>

#include "Hardware/CPU.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char                Multimeter::buffer[11] = {0};
UART_HandleTypeDef  Multimeter::handlerUART;
/// � ���� ����� ������������ ���������� � ����������� ���������� �����
static uint8 bufferUART[10];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multimeter::SetMeasure(const uint8 buf[10])
{
    std::memcpy(buffer, buf, 10);
    buffer[2] |= 0x30;
    buffer[3] |= 0x30;
    buffer[4] |= 0x30;
    buffer[5] |= 0x30;
    buffer[6] |= 0x30;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeMode()
{
    std::memset(buffer, '8', 10); //-V512
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Init()
{
    handlerUART.Instance = USART3;
    handlerUART.Init.BaudRate = 9600;
    handlerUART.Init.WordLength = UART_WORDLENGTH_8B;
    handlerUART.Init.StopBits = UART_STOPBITS_1;
    handlerUART.Init.Parity = UART_PARITY_NONE;
    handlerUART.Init.Mode = UART_MODE_TX_RX;
    handlerUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handlerUART.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&handlerUART) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    uint8 send[4] = {0x02, 'V', '0', 0x0a};

    HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    HAL_UART_Transmit(&handlerUART, send, 4, 10);

    if (HAL_UART_Receive_IT(&handlerUART, bufferUART, 10) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeAVP()
{
    ChangeMode();
    char send[4] = {0x02, 'Z', MULTI_AVP == AVP::On ? '1' : '0', 0x0a};
    HAL_UART_Transmit(&handlerUART, (uint8*)send, 4, 100);
    HAL_UART_Receive_IT(&handlerUART, bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Update()
{
    if (!Device::State::InModeMultimeter())
    {
        return;
    }
    
    uint8 range = 0;
    if(MULTI_MEASURE == Measure::VoltageDC)        { range = (uint8)MULTI_RANGE_DC; }
    else if(MULTI_MEASURE == Measure::VoltageAC)   { range = (uint8)MULTI_RANGE_AC; }
    else if(MULTI_MEASURE == Measure::Resistance)  { range = (uint8)MULTI_RANGE_RESISTANCE; }
    else
    {
        // ������ ������� ����
    }

    char symbol = Measure(MULTI_MEASURE).Symbol();

    uint8 send[4] = {0x02, (uint8)symbol, (uint8)(range + 0x30), 0x0a};
    HAL_UART_Transmit(&handlerUART, send, 4, 100);
    HAL_UART_Receive_IT(&handlerUART, bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *)
{
    Multimeter::SetMeasure(bufferUART);

    HAL_UART_Receive_IT(&Multimeter::handlerUART, bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart == &Multimeter::handlerUART)
    {
        huart = &Multimeter::handlerUART;
    }
}
