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
            Forbidden,  ///< ������������ �����
            Send,       ///< �������� ������ � ������
            Receive,    ///< ���� ������ �� ������
            Disabled    ///< ����� ����� ������������ �� ���
        };
    };

    Mode::E Get_MODE();

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

        void Write_READY(int);
        int Read_FL0();
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
    
    Receiver::Write_READY(1);

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
    Mode::E mode = Get_MODE();

    if (mode == Mode::Disabled)
    {
        return;
    }

    if (mode == Mode::Receive)
    {
        Transmitter::TransmitData();
    }

    if (mode == Mode::Send)
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
    if (Get_MODE() != Mode::Send)
    {
        return;
    }

    uint startTime = TIME_MS;

    do
    {
        uint8 data = ReadDataPins();
        
        if(data != 0)
        {
            data = data;
        }

        Decoder::AddData(data);

|        Write_READY(0);

        while (Read_FL0() == 1)
        {
            if(TIME_MS - startTime > 500)
            {
                Write_READY(1);
                return;
            }
        };

        Write_READY(1);
    } while (Get_MODE() == Mode::Send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Write_READY(int ready)
{
    HAL_GPIO_WritePin(READY, (GPIO_PinState)ready);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Transceiver::Receiver::Read_FL0()
{
    return HAL_GPIO_ReadPin(FL0);
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
Transceiver::Mode::E Transceiver::Get_MODE()
{
    //                  MODE    0  1
    static const Mode::E modes [2][2] =
    {
        {Mode::Forbidden, Mode::Send},
        {Mode::Receive,   Mode::Disabled}
    };

    int m0 = HAL_GPIO_ReadPin(MODE0);
    int m1 = HAL_GPIO_ReadPin(MODE1);

    return modes[m0][m1];
}
