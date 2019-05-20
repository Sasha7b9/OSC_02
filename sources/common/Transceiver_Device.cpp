#include "defines.h"
#include "Transceiver.h"
#include <stm32f4xx_hal.h>


#define PORT_SELECT         GPIOG
#define PIN_SELECT          GPIO_PIN_12
#define SELECT              PORT_SELECT, PIN_SELECT

#define PORT_MODE           GPIOA
#define PIN_MODE            GPIO_PIN_7
#define MODE                PORT_MODE, PIN_MODE

#define PORT_WRITE_READY    GPIOD
#define PIN_WRITE_READY     GPIO_PIN_5
#define WRITE_READY         PORT_WRITE_READY, PIN_WRITE_READY

#define PORT_READ_READY     GPIOC
#define PIN_READ_READY      GPIO_PIN_4
#define READ_READY          PORT_READ_READY, PIN_READ_READY

#define PORT_CONFIRM        GPIOD
#define PIN_CONFIRM         GPIO_PIN_4
#define CONFIRM             PORT_CONFIRM, PIN_CONFIRM


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
    gpio.Pin = PIN_SELECT;              // SELECT - ����� ������ ��� ������
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_SELECT, &gpio);

    Transmitter::Write_SELECT(1);       // ��������� ����� � �������

    gpio.Pin = PIN_MODE;                // MODE - ����� ������ � �������
    HAL_GPIO_Init(PORT_MODE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitPins()
{
    CallbackOnInitPins();

    Write_MODE(0);                      // ����� ���������� � ������

    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_WRITE_READY;         // WRITE_READY - ���� ����� ���������� ������� ����, ��� ������ ���������� � ������ ��� ������
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_WRITE_READY, &gpio);

    Write_READY_WR(1);                  // ������������� ������� ����, ��� ������ �� ���� �� ����������

    gpio.Pin = PIN_CONFIRM;             // CONFIRM_READ - ���� ���������� ������� ����, ��� ������������� �� ������ ��������
    HAL_GPIO_Init(PORT_CONFIRM, &gpio);

    Write_CONFIRM(1);                   // ������������� ������� ����, ��� ������ �� ������������

    gpio.Pin = PIN_READ_READY;          // READ_REDY - ������ ��������� ������������� ������ �� ������
    gpio.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(PORT_READ_READY, &gpio);

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
    HAL_GPIO_WritePin(MODE, (GPIO_PinState)mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_SELECT(int select)
{
    HAL_GPIO_WritePin(SELECT, (GPIO_PinState)select);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_CONFIRM(int confirm)
{
    HAL_GPIO_WritePin(CONFIRM, (GPIO_PinState)confirm);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_READY_WR(int ready)
{
    HAL_GPIO_WritePin(WRITE_READY, (GPIO_PinState)ready);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Transceiver::Transmitter::Read_READY_RD()
{
    return HAL_GPIO_ReadPin(READ_READY);
}
