#include "defines.h"
#include "log.h"
#include "DataBus.h"
#include "Transceiver.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>
#include "Keyboard/DecoderDevice.h"


#define PORT_MODE0  GPIOA
#define PIN_MODE0   GPIO_PIN_7
#define MODE0       PORT_MODE0, PIN_MODE0

#define PORT_MODE1  GPIOC
#define PIN_MODE1   GPIO_PIN_4
#define MODE1       PORT_MODE1, PIN_MODE1

#define PORT_READY  GPIOG
#define PIN_READY   GPIO_PIN_12
#define READY       PORT_READY, PIN_READY

#define PORT_FL0    GPIOD
#define PIN_FL0     GPIO_PIN_5
#define FL0         PORT_FL0, PIN_FL0


namespace Transceiver
{

    struct Mode
    {
        enum E
        {
            Disabled,   ///< ����� ����� ������������ �� ���
            Send,       ///< �������� ������ � ������
            Receive,    ///< ���� ������ �� ������
            Forbidden   ///< ������������ �����
        };
    };

    struct State
    {
        enum E
        {
            Passive,
            Active
        };
    };


    /// ��������� ������ ������
    void Set_MODE(Mode::E mode);

    State::E State_READY();

    namespace Transmitter
    {
        /// ���������������� ������ � ����� ��������.
        void InitPinsTransmit();
        void SetData(uint8 data);
    }

    namespace Receiver
    {
        /// ������������� FL0 �� ������
        void Init_FL0_IN();
        /// ���������������� ���� ��� ����� �� ������
        void InitPinsReceive();
        /// ���������� ��������� FL0
        State::E State_FL0();
        /// ��������� ���� ������ � ��
        uint8 ReadData();
    }
}


//                                      D0           D1           D2          D3          D4          D5          D6          D7
static const GPIO_TypeDef *ports[] = { GPIOD,       GPIOD,       GPIOD,      GPIOD,      GPIOE,      GPIOE,      GPIOE,      GPIOE };
static const uint16 pins[]         = { GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10 };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_MODE0;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_MODE0, &gpio);   // MODE0 - ������������ ��� ������ ������

    gpio.Pin = PIN_MODE1;
    HAL_GPIO_Init(PORT_MODE1, &gpio);   // MODE1 - ������������ ��� ������ ������

    gpio.Pin = PIN_READY;                 
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_READY, &gpio);   // READY - ������������ ��� ������ ������������� �� ������

    Set_MODE(Mode::Disabled);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Init_FL0_IN()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_FL0, &gpio);     // ����� �� ���� ������ ��������, ���� �� � ������ ������ ��� ��������
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitPinsTransmit()
{
    if (DataBus::GetMode().IsDeviceTransmit())
    {
        return;
    }

    DataBus::SetModeTransmit();

    /* �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3 */
    /* ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP. */
    GPIOD->MODER &= 0x0ffffff0;
    GPIOD->MODER |= 0x50000005;
    /* �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7 */
    /* ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP. */
    GPIOE->MODER &= 0xffc03fff;
    GPIOE->MODER |= 0x00154000;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitPinsReceive()
{
    GPIO_InitTypeDef gpio;

    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;

    gpio.Pin = GPIO_PIN_0  |           // D2
               GPIO_PIN_1  |           // D3
               GPIO_PIN_14 |           // D0
               GPIO_PIN_15;            // D1
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Pin = GPIO_PIN_7 |            // D4
               GPIO_PIN_8 |            // D5
               GPIO_PIN_9 |            // D6
               GPIO_PIN_10;            // D7
    HAL_GPIO_Init(GPIOE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 data)
{
    Send(&data, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
    InitPinsTransmit();                         // �������������� ���� ��� ��������

    for (uint i = 0; i < size; i++)
    {
        SetData(data[i]);                       // ������������� ���� ������

        Set_MODE(Mode::Send);                   // ��� ������ ������, ��� ����� ��������� ������

        while (State_READY() == State::Passive) {};   // ������� ������ �������������

        Set_MODE(Mode::Disabled);               // ��� �������, ��� ������������� ��������. ������ ������ ������ ������ ������ READY

        while (State_READY() == State::Active) {};
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Update()
{
    DataBus::SetModeReceive();

    Receiver::Init_FL0_IN();                    // �������������� FL0 �� ������

    Set_MODE(Mode::Receive);                    // �������� ������, ��� ������ ������� ������

    while (State_READY() == State::Passive) {}; // ������� ������ ���������� �� ������

    if (Receiver::State_FL0() == State::Passive)      // ���� ������ �������� � ���, ��� ������ ���
    {
        Set_MODE(Mode::Disabled);               // �� ��������� �������������� � �������

        return false;                            // � �������
    }

    Receiver::InitPinsReceive();                // �������������� ���� ������ �� ����

    uint8 data = Receiver::ReadData();          // ������ ����
    
    Decoder::AddData(data);                     // � ���������� ��� �� ����������

    Set_MODE(Mode::Disabled);

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Transceiver::State::E Transceiver::State_READY()
{
    return (HAL_GPIO_ReadPin(READY) == GPIO_PIN_SET) ? State::Active : State::Passive;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Set_MODE(Mode::E mode)
{
    if (mode == Mode::Send)
    {
        HAL_GPIO_WritePin(MODE0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MODE1, GPIO_PIN_SET);
    }
    else if (mode == Mode::Receive)
    {
        HAL_GPIO_WritePin(MODE1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MODE0, GPIO_PIN_SET);
    }
    else if (mode == Mode::Disabled)
    {
        HAL_GPIO_WritePin(MODE0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MODE1, GPIO_PIN_RESET);
        /// \todo � ���� ���� ���-�� ������. ���������, ������ ��� �������� �� ��������
        //Timer::PauseOnOPS(200);
    }
    else
    {
        // ����� ������ �� ����
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Transceiver::State::E Transceiver::Receiver::State_FL0()
{
    return (HAL_GPIO_ReadPin(FL0) == GPIO_PIN_SET) ? State::Active : State::Passive;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::SetData(uint8 data)
{
    //                                                        ���� 0,1                           ���� 2, 3
    GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + (uint16)(((int16)data & 0x03) << 14) + (((uint16)(data & 0x0c)) >> 2);

    //                                                    ���� 4,5,6,7
    GPIOE->ODR = (GPIOE->ODR & 0xf87f) + (uint16)(((int16)data & 0xf0) << 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Transceiver::Receiver::ReadData()
{
    uint8 result = 0;

    for (int i = 7; i >= 0; i--)
    {
        result |= HAL_GPIO_ReadPin((GPIO_TypeDef *)ports[i], pins[i]);

        if (i != 0)
        {
            result <<= 1;
        }
    }

    return result;
}
