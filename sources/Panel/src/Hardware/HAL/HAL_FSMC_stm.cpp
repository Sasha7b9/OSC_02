#include "defines.h"
#include "common/Decoder_p.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


#define PORT_READY  GPIOC
#define PIN_READY   GPIO_PIN_14
#define READY       PORT_READY, PIN_READY

#define PORT_DATA   GPIOC
#define PIN_DATA    GPIO_PIN_15
#define DATA        PORT_DATA, PIN_DATA

#define PORT_CS     GPIOC
#define PIN_CS      GPIO_PIN_13
#define CS          PORT_CS, PIN_CS

#define PORT_WR     GPIOD
#define PIN_WR      GPIO_PIN_5
#define WR          PORT_WR, PIN_WR

#define PORT_RD     GPIOD
#define PIN_RD      GPIO_PIN_4
#define RD          PORT_RD, PIN_RD


struct OutPin
{
    OutPin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) {};

    void Init()
    {
        GPIO_InitTypeDef is = { pin, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP };

        HAL_GPIO_Init(gpio, &is);

        SetPassive();
    }

    void SetActive()  { HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET); }

    void SetPassive() { HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET); }

    GPIO_TypeDef *gpio;
    uint16 pin;
};


struct InPin
{
    InPin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) {};

    void Init()
    {
        GPIO_InitTypeDef is = { pin, GPIO_MODE_INPUT, GPIO_PULLUP };

        HAL_GPIO_Init(gpio, &is);
    }

    bool IsActive()  { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_RESET; }

    bool IsPassive() { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_SET; }

    GPIO_TypeDef *gpio;
    uint16 pin;
};


/// �� ���� ������ ����� ���������� ������� ���������� � ������������ � ������� �������������
static OutPin pinReady(READY);
/// ����� ����� ���������� ������� ���������� ������ ��� �������� � ����������
static OutPin pinData(DATA);
/// �� ����� ������� �� �������� �� �������� ���������� ������/������
static InPin  pinCS(CS);
/// ������� ����, ��� �������� �� ������������ �������� ������ � ������
static InPin  pinWR(WR);
/// ������� ����, ��� �������� �� ������������ �������� ������ �� ������
static InPin  pinRD(RD);


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


void HAL_FSMC::Init()
{
    pinReady.Init();
    pinReady.SetActive();

    pinData.Init();
    pinData.SetPassive();

    pinCS.Init();
    pinRD.Init();
    pinWR.Init();

    DataBus::Init();
}

void HAL_FSMC::SendToPanel(uint8 *, uint)
{

}


bool HAL_FSMC::Receive()
{
    if(pinCS.IsPassive())   // ���� CS ��������� - ������� �� �� ����� ��������
    {
        return false;
    }
        
    if(pinWR.IsActive())
    {
        uint8 data = DataBus::Read();

        pinReady.SetPassive();

        PDecoder::AddData(data);

        pinReady.SetActive();

        return true;
    }

    return false;
}


void DataBus::Init()
{
    ConfigureToRead();
}


void DataBus::ConfigureToRead()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio);
}


void DataBus::ConfigureToWrite()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio);
}


uint8 DataBus::Read()
{
    return (uint8)GPIOE->IDR;
}


void DataBus::Write(uint8 data)
{
    static const uint16 pins[8] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7 };

    for(int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(GPIOE, pins[i], static_cast<GPIO_PinState>(data & 0x01));
        data >>= 1;
    }
}
