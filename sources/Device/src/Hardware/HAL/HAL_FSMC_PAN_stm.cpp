#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


struct OutPin
{
    OutPin(HPort::E _port, uint16 _pin) : port(_port), pin(_pin) {}

    void Init()       { HAL_PIO::Init(port, pin, HMode::Output_PP, HPull::Up); SetPassive(); }
    void SetActive()  { HAL_PIO::Reset(port, pin); }
    void SetPassive() { HAL_PIO::Set(port, pin); }

    HPort::E port;
    uint16 pin;
};


struct InPin
{
    InPin(HPort::E _port, uint16 _pin) : port(_port), pin(_pin) {}

    void Init()      { HAL_PIO::Init(port, pin, HMode::Input, HPull::Up); }
    bool IsActive()  { return HAL_PIO::Read(port, pin) == 0; };
    bool IsPassive() { return HAL_PIO::Read(port, pin) == 1; };

    HPort::E port;
    uint16 pin;
};


struct DataBus
{
    /// �������������� �������������
    static void Init();
    /// ���������������� ��� ������
    static void ConfigureToRead();
    /// ��������� ���� � ���� ������
    static uint8 Read();
    /// ���������������� ��� ������
    static void ConfigureToWrite();
    /// �������� ���� � ���� ������
    static void Write(uint8 byte);
};


/// �� ��������� ��������� ����� ���� ������ ����������, ��� ������� �� ����� � ��������������
static OutPin pinCS(PIN_CS);
/// �� ������������ ��������� ����� ���� ������ ����������, ��� ������� �� ����� ������ ��������
static OutPin pinWR(PIN_WR);
/// �� ��������� ��������� ����� ���� ������ ����������, ��� ������� �� ����� ������ ������
static OutPin pinRD(PIN_RD);

/// �������� ���������� ����� ���� ������ �������� � ���������� � ��������������
static InPin pinReadyPAN(PIN_PAN_READY);
/// �������� ��������� ����� ���� �������� � ���, ��� ������ ����� ������ ��� ��������
static InPin pinDataPAN(PIN_PAN_DATA);

/// true ��������, ��� ���� ��������� � �������� ������ � ������� � ������ �� ������� FSMC � ������� � ������ ���������
static bool interactionWithPanel = false;

/// �������� ���� ���� � ������
static void SendByteToPanel(uint8 byte);


void HAL_FSMC::InitPanel()
{
    pinReadyPAN.Init();
    pinDataPAN.Init();
    pinCS.Init();

    DataBus::Init();
}


void HAL_FSMC::ConfigureToPanel(bool forWrite)
{
    mode = Mode::Panel;

    pinWR.Init();
    pinRD.Init();

    if(forWrite)
    {
        DataBus::ConfigureToWrite();
    }
    else
    {
        DataBus::ConfigureToRead();
    }
}


bool HAL_FSMC::Receive()
{
    interactionWithPanel = true;

    if(mode != Mode::Panel)
    {
        ConfigureToPanel(false);
    }


    interactionWithPanel = false;

    return false;
}


void HAL_FSMC::SendToPanel(uint8 byte)
{
    SendToPanel(&byte, 1);
}


void HAL_FSMC::SendToPanel(uint8 byte0, uint8 byte1)
{
    uint8 buffer[2] = { byte0, byte1 };
    
    SendToPanel(buffer, 2);
}


void HAL_FSMC::SendToPanel(uint8 *data, uint size)
{
    interactionWithPanel = true;

    if(mode != Mode::Panel)
    {
        ConfigureToPanel(true);
    }

    for(uint i = 0; i < size; i++)
    {
        SendByteToPanel(*data++);
    }

    interactionWithPanel = false;
}


static void SendByteToPanel(uint8 byte)
{
    DataBus::Write(byte);               // ���������� ������ �� ����

    pinWR.SetActive();                  // ��� ������ ������

    while(pinReadyPAN.IsPassive()) {}   // � ������� ������ ������ � ���, ��� ��� ��������

    pinCS.SetActive();                  // ��� ������� ����, ��� ������ ���������� � ����� �� ���������

    while(pinReadyPAN.IsActive()) {}    // ������������ PIN_PAN_READY � ���������� ��������� ��������, ��� ������ ������� ������ � ������������ ��

    pinWR.SetPassive();                 // \ ������������� WR � CS � ���������� ��������� - ������������ ���� ������ �������
    pinCS.SetPassive();                 // /
}


bool HAL_FSMC::InteractionWithPanel()
{
    return interactionWithPanel;
}


void DataBus::Init()
{
    ConfigureToRead();
}


void DataBus::ConfigureToRead()
{
    GPIO_InitTypeDef gpio;

    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;

    gpio.Pin = GPIO_PIN_0  |        // D2
               GPIO_PIN_1  |        // D3
               GPIO_PIN_14 |        // D0
               GPIO_PIN_15;         // D1
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Pin = GPIO_PIN_7  |        // D4
               GPIO_PIN_8  |        // D5
               GPIO_PIN_9  |        // D6
               GPIO_PIN_10;         // D7
    HAL_GPIO_Init(GPIOE, &gpio);
}


void DataBus::ConfigureToWrite()
{
    GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
    GPIOD->MODER |= 0x50000005U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP

    GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
    GPIOE->MODER |= 0x00154000U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
}


void DataBus::Write(uint8 d)
{
    //                                                                             ���� 0,1                                 ���� 2,3
    GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // ���������� ������ � �������� ����
    //                                                                          ���� 4,5,6,7
    GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3);
}


uint8 DataBus::Read()
{
    uint8 result = HAL_PIO::Read(PIN_D7);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D6);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D5);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D4);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D3);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D2);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D1);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D0);

    return result;
}
