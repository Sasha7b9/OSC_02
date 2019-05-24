#include "defines.h"
#include "Transceiver.h"
#include <stm32f4xx_hal.h>


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
        } state;
        explicit State(E s) : state(s) {};
        bool IsPassive() const { return state == Passive; };
    };


    /// ��� ������� ����� �������� ������ ��� ��� ������������� ����� �� ���� ��� ��������.
    void(*CallbackOnInitPins)();
    /// ��������� ������ ������
    void Set_MODE(Mode::E mode);

    namespace Transmitter
    {
        /// ���������������� ������ � ����� ��������.
        void InitPins();
        void SetData(uint8 data);
        void Set_FL0(State::E state);
        State State_READY();
    }

    namespace Receiver
    {
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init(void (*callbackInitPins)())
{
    CallbackOnInitPins = callbackInitPins;

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
void Transceiver::Transmitter::InitPins()
{
    CallbackOnInitPins();

    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_FL0, &gpio);     // FL0 - ���� ����� ���������� ������������� ����� ������
    
    Set_FL0(State::Passive);

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
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
#define ALL_DATAS_SEND (i == size - 1)

    InitPins();

    Set_MODE(Mode::Disabled);

    for (uint i = 0; i < size; i++)
    {
        Set_FL0(State::Passive);                // ������� �������������� ������� ����, ��� �������� ������������� �����

        SetData(data[i]);                       // ������������� ������ �� ��

        Set_MODE(Mode::Send);                   // ��� ���������� ������ �� ������ ������

        while (State_READY().IsPassive()) {};   // ������� �� ������ ������� � ����� ������

        if (ALL_DATAS_SEND)                     // ���� ��������� ��� ������
        {
            Set_MODE(Mode::Disabled);           // �� ��������� �������������� �� ����.
        }

        Set_FL0(State::Active);                 // ��� ������ �������������, ��� �� ������� � �������������
    }

    Set_MODE(Mode::Disabled);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Transceiver::State Transceiver::Transmitter::State_READY()
{
    if (HAL_GPIO_ReadPin(READY) == GPIO_PIN_SET)
    {
        return State(State::Active);
    }

    return State(State::Passive);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{

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
    }
    else
    {
        // ����� ������ �� ����
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Set_FL0(State::E state)
{
    HAL_GPIO_WritePin(FL0, (state == State::Passive) ? GPIO_PIN_RESET : GPIO_PIN_SET);
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
