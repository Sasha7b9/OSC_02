#include "defines.h"
#include "common/Decoder_p.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include <stm32f4xx_hal.h>


// ��� ���������� � ������ � ������
#define PORT_READY_WR  GPIOC
#define PIN_READY_WR   GPIO_PIN_14
#define READY_WR       PORT_READY_WR, PIN_READY_WR

// ��� ���������� � ������ �� ������
#define PORT_READY_RD  GPIOC
#define PIN_READY_RD   GPIO_PIN_15
#define READY_RD       PORT_READY_RD, PIN_READY_RD

// ���������
#define PORT_CS        GPIOC
#define PIN_CS         GPIO_PIN_13
#define CS             PORT_CS, PIN_CS

// ������ ������ � ������ (������� �� 0 � 1)
#define PORT_WR        GPIOD
#define PIN_WR         GPIO_PIN_5
#define WR             PORT_WR, PIN_WR

// ������ ������ �� ������ (������� �� 0 � 1)
#define PORT_RD        GPIOD
#define PIN_RD         GPIO_PIN_4
#define RD             PORT_RD, PIN_RD


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

    bool IsActive()    { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_RESET; }

    bool IsPassive()   { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_SET; }

    void WaitPassive() { while(IsActive()) { } }

    void WaitActive()  { while(IsPassive()) { } }

    GPIO_TypeDef *gpio;
    uint16 pin;
};


/// �� ���� ������ ����� ���������� ������� ���������� � ������������ � ������� �������������
static OutPin pinReadyWR(READY_WR);
/// ����� ����� ���������� ������� ���������� ������ ��� �������� � ����������
static OutPin pinReadyRD(READY_RD);
/// �� ����� ������� �� �������� �� �������� ���������� ������/������
static InPin  pinCS(CS);
/// ������� ����, ��� �������� �� ������������ �������� ������ � ������
static InPin  pinWR(WR);
/// ������� ����, ��� �������� �� ������������ �������� ������ �� ������
static InPin  pinRD(RD);

static Queue<uint8> queueData;


void HAL_BUS::Init()
{
    pinReadyWR.Init();
    pinReadyRD.Init();

    pinCS.Init();
    pinRD.Init();
    pinWR.Init();

    GPIOE->MODER &= 0xffff0000U;                        // ������������� �� �� ������
}


void HAL_BUS::SendToDevice(uint8 *data, uint size)
{
    while(size > 0)
    {
        queueData.Push(*data++);
        size--;
    }
}


void HAL_BUS::Update()
{
    if(queueData.Size())
    {
        pinReadyRD.SetActive();                                 // ������������� ������� ����, ��� ������� ������

        while(queueData.Size())                                 // ���� ���� ������ ��� ��������
        {
            while(pinCS.IsPassive() || pinRD.IsPassive())  { }  // ���, ���� �� ��������� CS � RD � �������� ���������, ������������ � ���, ��� �� �������� ������� ������

            GPIOE->MODER &= 0xffff0000U;                        // ������������� �� �� ������
            GPIOE->MODER |= 0x00005555U;

            GPIOE->ODR = (GPIOD->ODR & 0xffff0000) + static_cast<uint16>(queueData.Front());    // ������������� ������ �� ��

            while(pinCS.IsActive() && pinRD.IsActive())    { }  // ��� �� �� ������� ��������� ������

            GPIOE->MODER &= 0xffff0000U;                        // ������������� �� �� ������
        }

        pinReadyRD.SetPassive();
    }

    pinReadyWR.SetActive();

    if(pinCS.IsActive() && pinWR.IsActive())
    {
        while(pinCS.IsActive() && pinWR.IsActive())       { }   // ��� ���� �� �������� ������� ��������� ������

        uint8 data = static_cast<uint8>(GPIOE->IDR);            // � ��������� ������

        pinReadyWR.SetPassive();

        PDecoder::AddData(data);                   
    }
}
