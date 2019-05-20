#include "defines.h"
#include "Transceiver.h"
#include <stm32f4xx_hal.h>


namespace Transceiver
{
    /// ��� ������� ����� �������� ������ ��� ��� ������������� ����� �� ���� ��� ��������.
    void(*CallbackOnInitPins)();

    namespace Transmitter
    {
        /// ���������������� ������ � ����� ��������.
        void InitPins();
        /// ��������� ������ ������ - ���� ��� ��������; "0" - ������ � ������, "1" - ������ �� ������.
        void Write_MODE(int);
        /// ����� ������ ��� ������. �������� ������� - "0".
        void Write_SELECT(int);
        /// ��������� �������� ����, ��� ������ ���������� �� ��.
        void Write_READY_WR(int);
        /// ������� ������������� ����, �� ������ ��������� ������.
        int Read_READY_RD();
        /// ���������� � ������ ������� ����, ��� ������������� �������.
        void Write_CONFIRM(int);
        /// ���������� ��������� ����� �� � ������������ � ������ data 
        void SetData(uint8 data);
    }

    namespace Receiver
    {
        void InitPinsReceive();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init(void (*callbackInitPins)())
{
    CallbackOnInitPins = callbackInitPins;

    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_12;              // SELECT - ����� ������ ��� ������
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOG, &gpio);

    Transmitter::Write_SELECT(1);       // ��������� ����� � �������

    gpio.Pin = GPIO_PIN_7;              // MODE - ����� ������ � �������
    HAL_GPIO_Init(GPIOA, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitPins()
{
    CallbackOnInitPins();

    Write_MODE(0);                      // ����� ���������� � ������

    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_5;              // WRITE_READY - ���� ����� ���������� ������� ����, ��� ������ ���������� � ������ ��� ������
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOD, &gpio);

    Write_READY_WR(1);                  // ������������� ������� ����, ��� ������ �� ���� �� ����������

    gpio.Pin = GPIO_PIN_4;              // CONFIRM_READ - ���� ���������� ������� ����, ��� ������������� �� ������ ��������
    HAL_GPIO_Init(GPIOC, &gpio);

    Write_CONFIRM(1);                   // ������������� ������� ����, ��� ������ �� ������������

    gpio.Pin = GPIO_PIN_4;              // READ_REDY - ������ ��������� ������������� ������ �� ������
    gpio.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin =  GPIO_PIN_0  |           // D2
                GPIO_PIN_1  |           // D3
                GPIO_PIN_14 |           // D0
                GPIO_PIN_15;            // D1
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Pin =  GPIO_PIN_7 |            // D4
                GPIO_PIN_8 |            // D5
                GPIO_PIN_9 |            // D6
                GPIO_PIN_10;            // D7
    HAL_GPIO_Init(GPIOE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitPinsReceive()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
    InitPins();

    Write_READY_WR(1);

    Write_SELECT(0);                     // �������� ������ �����������, � ������� ����� ������������� �����

    for (uint i = 0; i < size; i++)
    {
        Write_CONFIRM(1);                // ������������� ������ ����� ������������ ���� ������ �������

        SetData(data[i]);                // ������������� ����� �� � ������������ � ��������� ������������ ������

        Write_READY_WR(0);               // ��� ���� ������, ��� ������ ���������� � ����� �� ���������. 

        while (Read_READY_RD() == 1) {}; // ��� ������������� ������ �� ������. ������ ���������� "0" �� ���� ������, ����� ��������� ������

        Write_CONFIRM(0);                // ���������� ������� ����, ��� �������� ������������� ������.

        while (Read_READY_RD() == 0) {}; // � ��� �� ���, ���, ���� ������ ��� �� ���������. ����� ����, ��� ������ ���������� CONFIRM == 0, ��� ���������� READY_RD � ������� � ��� ��
                                         // �� ��������� �������� ����������
        Write_READY_WR(1);
    }

    Write_SELECT(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_MODE(int mode)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, (GPIO_PinState)mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_SELECT(int select)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, (GPIO_PinState)select);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_CONFIRM(int confirm)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, (GPIO_PinState)confirm);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_READY_WR(int ready)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, (GPIO_PinState)ready);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::SetData(uint8 data)
{
                                    // D0          D1           D2          D3          D4          D5          D6          D7
    static GPIO_TypeDef *ports[] = { GPIOD,       GPIOD,       GPIOD,      GPIOD,      GPIOE,      GPIOE,      GPIOE,      GPIOE };
    static uint16 pins[] =         { GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10 };

    for (int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(ports[i], pins[i], (GPIO_PinState)((data >> i) & 0x01));
    }
}
