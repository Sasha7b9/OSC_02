#include "defines.h"
#include "Transceiver.h"
#include "Utils/DecoderPanel.h"
#include "Hardware/Timer.h"
#include <cstring>


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


    /// /// ��� ������� ����� �������� ������ ��� ��� ������������� ����� �� ���� ��� ��������.
    void(*CallbackOnInitPins)();
    /// � ���� ������� ��� ���� ������ ���� ���������������� �� ����, ����� �� ����������� ����.
    void DeInitPins();
    /// ������� �� ������ ���������� ������
    void DiscardTransmittedData();

    Mode::E Mode_Device();

    void Set_READY(State::E state);
    ///  ������������������ D
    void DeInit_FL0();
   
    namespace Transmitter
    {
        void InitDataPins();
        /// �������� ������ � ����������, ���� ������� �������
        void TransmitData();
        /// ���������� ������ �� ����
        void SetData(uint8 data);
        /// ���������������� FL0 �� ����� - ����� ����� ���� �������� � �������/�������� ������ ��� ��������
        void Init_FL0_OUT();

        void Set_FL0(State::E state);
    }

    namespace Receiver
    {
        /// �������������� 8 ������� ������ �� ����
        void InitDataPins();
        /// ��������� ��� ������������ ����������� ������
        void ReceiveData();
    }

    static uint8 buffer[1024];

    static uint bytesInBuffer = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_MODE0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(PORT_MODE0, &gpio);   // MODE0 - ������������ ��� ������ ������ ���������� //-V525

    gpio.Pin = PIN_MODE1;
    HAL_GPIO_Init(PORT_MODE1, &gpio);   // MODE1 - ������������ ��� ������ ������ ����������

    gpio.Pin = PIN_FL0;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(PORT_FL0, &gpio);     // FL0 - ������������ ��� ������ ������������� �� ����������

    gpio.Pin = PIN_READY;               
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_READY, &gpio);   // READY - ������������ ��� ������������� ������ ������
    
    Set_READY(State::Passive);

    DeInitPins();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::DeInitPins()
{
    DeInit_FL0();

    Receiver::InitDataPins();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
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
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
    std::memcpy(&buffer[bytesInBuffer], data, size);
    bytesInBuffer += size;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Update()
{
    Mode::E mode = Mode_Device();

    if (mode == Mode::Receive)
    {
        Transmitter::TransmitData();

        return true;
    }

    if (mode == Mode::Send)
    {
        Receiver::ReceiveData();

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::TransmitData()
{
    Init_FL0_OUT();                             // �������������� FL0 ��� ����, ����� ��������� �� �� ������� ������� ��� ���������� ������

    if (bytesInBuffer == 0)
    {
        Set_FL0(State::Passive);                // ���������� ������� ����, ��� ������ ��� �������� ���
    }
    else
    {
        Set_FL0(State::Active);                 // ���������� ������� ����, ��� ���� ������ ��� ��������
        
        InitDataPins();                         // �������������� ���� ����� ��� ��������

        SetData(buffer[0]);                     // ������������� ���� �� �� � ������������ � ������������� �������

        DiscardTransmittedData();
    }

    Set_READY(State::Active);                   // ������������� ������� ����, ��� ������ ����������

    while (Mode_Device() == Mode::Receive) {};  // ��� �� ������� ������������� ����, ��� ������ �������

    Set_READY(State::Passive);                  // ������� �� ������ ��������

    DeInitPins();                               // ���������������� ������
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::DiscardTransmittedData()
{
    if (bytesInBuffer > 0)
    {
        bytesInBuffer--;                        // ������� ���������� ������ �� ������

        std::memmove(&buffer[0], &buffer[1], bytesInBuffer);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::ReceiveData()
{
    Decoder::AddData((uint8)GPIOE->IDR);    // ������ � ������������ ���� ������

    Set_READY(State::Active);               // ������������� �������, ��� ������ �������

    while (Mode_Device() == Mode::Send) {}; // ��� ������ �������������

    Set_READY(State::Passive);              // � ������� ������ ����������
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Set_READY(State::E state)
{
    HAL_GPIO_WritePin(READY, (state == State::Active) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Set_FL0(State::E state)
{
    HAL_GPIO_WritePin(FL0, (state == State::Active) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::SetData(uint8 data)
{
    static const uint16 pins[8] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

    for (int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(GPIOE, pins[i], (GPIO_PinState)(data & 0x01));
        data >>= 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Init_FL0_OUT()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_FL0, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::DeInit_FL0()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(PORT_FL0, &gpio);     // FL0 �������������� �� ������������� - ����� �� ������ ������ ����
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Transceiver::Mode::E Transceiver::Mode_Device()
{
    //                  MODE    0  1
    static const Mode::E modes [2][2] =
    {
        {Mode::Disabled, Mode::Send},
        {Mode::Receive,  Mode::Forbidden}
    };

    int m0 = HAL_GPIO_ReadPin(MODE0);
    int m1 = HAL_GPIO_ReadPin(MODE1);

    return modes[m0][m1];
}
