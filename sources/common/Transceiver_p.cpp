#include "defines.h"
#include "Transceiver.h"
#include "Hardware/DecoderPanel.h"
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


/// ����� ������ ���������� (�� ������)
struct ModeDevice
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
        Passive,    ///< ���������� "0"
        Active      ///< ����������� "1"
    };
};


/// /// ��� ������� ����� �������� ������ ��� ��� ������������� ����� �� ���� ��� ��������.
void(*CallbackOnInitPins)();
/// � ���� ������� ��� ���� ������ ���� ���������������� �� ����, ����� �� ����������� ����.
void DeInitPins();
/// ������� �� ������ ���������� ������
void DiscardTransmittedData();

ModeDevice::E Mode_Device();

void Set_READY(State::E state);
///  ������������������ D
void DeInit_FL0();

void InitDataPins();
/// �������� ������ � ����������, ���� ������� �������
void TransmitData();
/// ���������� ������ �� ����
void SetData(uint8 data);
/// ���������������� FL0 �� ����� - ����� ����� ���� �������� � �������/�������� ������ ��� ��������
void Init_FL0_OUT();

void Set_FL0(State::E state);

struct Receiver
{
    /// �������������� 8 ������� ������ �� ����
    static void InitDataPins();
};

static uint8 buffer[1024];

static uint _bytesInBuffer = 0;



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


void DeInitPins()
{
    DeInit_FL0();

    Receiver::InitDataPins();
}


void Receiver::InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &gpio);
}


void InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOE, &gpio);
}


void Transceiver::Send(const uint8 *data, uint size)
{
    std::memcpy(&buffer[_bytesInBuffer], data, size);
    _bytesInBuffer += size;
}


bool Transceiver::Receive()
{
    ModeDevice::E mode = Mode_Device();

    if (mode == ModeDevice::Receive)
    {
        TransmitData();

        return true;
    }

    if (mode == ModeDevice::Send)
    {
        PDecoder::AddData((uint8)GPIOE->IDR);       // ������ � ������������ ���� ������
        
        PORT_READY->BSRR = PIN_READY;               // ������������� �������, ��� ������ ������� - "1" �� READY
        
        while (PORT_MODE1->IDR & PIN_MODE1) {};     // ��� ������ �������������. ���� MODE1 ��������� � "1" - ���������� �� ��������� ������������� //-V712

        PORT_READY->BSRR = (uint)PIN_READY << 16U;  // � ������� ������ ����������, ������������ READY � "0"

        return true;
    }

    return false;
}


void TransmitData()
{
    Init_FL0_OUT();                             // �������������� FL0 ��� ����, ����� ��������� �� �� ������� ������� ��� ���������� ������

    if (_bytesInBuffer == 0)
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

    while (Mode_Device() == ModeDevice::Receive) {};  // ��� �� ������� ������������� ����, ��� ������ �������

    Set_READY(State::Passive);                  // ������� �� ������ ��������

    DeInitPins();                               // ���������������� ������
}


void DiscardTransmittedData()
{
    if (_bytesInBuffer > 0)
    {
        _bytesInBuffer--;                        // ������� ���������� ������ �� ������

        std::memmove(&buffer[0], &buffer[1], _bytesInBuffer);
    }
}


void Set_READY(State::E state)
{
    HAL_GPIO_WritePin(READY, (state == State::Active) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void Set_FL0(State::E state)
{
    HAL_GPIO_WritePin(FL0, (state == State::Active) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void SetData(uint8 data)
{
    static const uint16 pins[8] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

    for (int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(GPIOE, pins[i], static_cast<GPIO_PinState>(data & 0x01));
        data >>= 1;
    }
}


void Init_FL0_OUT()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_FL0, &gpio);
}


void DeInit_FL0()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(PORT_FL0, &gpio);     // FL0 �������������� �� ������������� - ����� �� ������ ������ ����
}


ModeDevice::E Mode_Device()
{
    //                  MODE    0  1
    static const ModeDevice::E modes [2][2] =
    {
        {ModeDevice::Disabled, ModeDevice::Send},
        {ModeDevice::Receive,  ModeDevice::Forbidden}
    };

    //int m0 = HAL_GPIO_ReadPin(MODE0);
    int m0 = (PORT_MODE0->IDR & PIN_MODE0) ? 1 : 0;
    
    //int m1 = HAL_GPIO_ReadPin(MODE1);
    int m1 = (PORT_MODE1->IDR & PIN_MODE1) ? 1 : 0;

    return modes[m0][m1];
}
