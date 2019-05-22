#include "defines.h"
#include "Transceiver.h"
#include "Utils/DecoderPanel.h"
#include "Hardware/Timer.h"


#define PORT_MODE0  GPIOC
#define PIN_MODE0   GPIO_PIN_14
#define MODE0       PORT_MODE0, PIN_MODE0

#define PORT_MODE1  GPIOC
#define PIN_MODE1   GPIO_PIN_15
#define MODE1       PORT_MODE1, PIN_MODE1

#define PORT_READY  GPIOC
#define PIN_READY   GPIO_PIN_13
#define READY       PORT_READY, PIN_READY

#define PORT_FL0    GPIOD
#define PIN_FL0     GPIO_PIN_4
#define FL0         PORT_FL0, PIN_FL0


namespace Transceiver
{
    /// /// ��� ������� ����� �������� ������ ��� ��� ������������� ����� �� ���� ��� ��������.
    void(*CallbackOnInitPins)();
    /// � ���� ������� ��� ���� ������ ���� ���������������� �� ����, ����� �� ����������� ����.
    void DeInitPins();

    struct Mode
    {
        enum E
        {
            Disabled,   ///< ����� ����� ������������ �� ���
            Send,       ///< �������� ������ � ������
            Receive,    ///< ���� ������ �� ������
            Forbidden   ///< ������������ �����
        } mode;
        explicit Mode(E m) : mode(m) {};
        bool IsDisabled() const { return mode == Disabled; };
        bool IsSend() const { return mode == Send; };
        bool IsReceive() const { return mode == Receive; };
    };

    struct State
    {
        enum E
        {
            Passive,
            Active
        } state;
        explicit State(State::E s) : state(s) {};
        bool IsPassive() const { return state == Passive; }
    };

    Mode Mode_Device();

    namespace Transmitter
    {
        void InitDataPins();
        /// �������� ������ � ����������, ���� ������� �������
        void TransmitData();
    }

    namespace Receiver
    {
        /// ��� ��������� ��������� ���������� ��� ������������ ������
        void Update();
        /// �������������� 8 ������� ������ �� ����
        void InitDataPins();
        /// ��������� ���� � ������� ������
        uint8 ReadDataPins();
        /// ��������� ��� ������������ ����������� ������
        void ReceiveData();

        void Set_READY(State::E state);
        State State_FL0();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init(void (*callbackInitPins)())
{
    CallbackOnInitPins = callbackInitPins;

    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_MODE0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_MODE0, &gpio);   // MODE0 - ������������ ��� ������ ������ ���������� //-V525

    gpio.Pin = PIN_MODE1;
    HAL_GPIO_Init(PORT_MODE1, &gpio);   // MODE1 - ������������ ��� ������ ������ ����������

    gpio.Pin = PIN_FL0;
    HAL_GPIO_Init(PORT_FL0, &gpio);     // FL0 - ������������ ��� ������ ������������� �� ����������

    gpio.Pin = PIN_READY;               
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_READY, &gpio);   // READY - ������������ ��� ������������� ������ ������
    
    Receiver::Set_READY(State::Passive);

    DeInitPins();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::DeInitPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_FL0, &gpio); // FL0 �������������� �� ������������� - ����� �� ������ ������ ����

    Receiver::InitDataPins();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 * /*data*/, uint /*size*/)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{
    Mode mode = Mode_Device();

    if (mode.IsDisabled())
    {
        return;
    }

    if (mode.IsReceive())
    {
        Transmitter::TransmitData();
    }

    if (mode.IsSend())
    {
        ReceiveData();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::TransmitData()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::ReceiveData()
{
    if (!Mode_Device().IsSend())
    {
        return;
    }

    do
    {
        uint8 data = ReadDataPins();
        
        Decoder::AddData(data);

        Set_READY(State::Active);

        while (State_FL0().IsPassive())
        {
        };

        Set_READY(State::Passive);

    } while (Mode_Device().IsSend());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Set_READY(State::E state)
{
    HAL_GPIO_WritePin(READY, (state == State::Active) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Transceiver::State Transceiver::Receiver::State_FL0()
{
    if (HAL_GPIO_ReadPin(FL0) == GPIO_PIN_SET)
    {
        return State(State::Active);
    }

    return State(State::Passive);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Transceiver::Receiver::ReadDataPins()
{
    int bit[8];

    bit[0] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0);
    bit[1] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1);
    bit[2] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
    bit[3] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3);
    bit[4] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);
    bit[5] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5);
    bit[6] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);
    bit[7] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7);

    uint8 result = 0;

    for (int i = 0; i < 8; i++)
    {
        result += (bit[i] << i) & 0x01;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Transceiver::Mode Transceiver::Mode_Device()
{
    //                  MODE    0  1
    static const Mode::E modes [2][2] =
    {
        {Mode::Disabled, Mode::Send},
        {Mode::Receive,  Mode::Forbidden}
    };

    int m0 = HAL_GPIO_ReadPin(MODE0);
    int m1 = HAL_GPIO_ReadPin(MODE1);

    return Mode(modes[m0][m1]);
}
