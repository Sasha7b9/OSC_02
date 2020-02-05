#include "defines.h"
#include "common/Decoder_d.h"
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

    void Init()        { HAL_PIO::Init(port, pin, HMode::Input, HPull::Up); }
    bool IsActive()    { return HAL_PIO::Read(port, pin) == 0; };
    bool IsPassive()   { return HAL_PIO::Read(port, pin) == 1; };
    void WaitActive()  { while(IsPassive()) { } }
    void WaitPassive() { while(IsActive()) { } }

    HPort::E port;
    uint16 pin;
};


struct DataBus
{
    /// �������������� �������������
    static void Init();
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

void HAL_BUS::Panel::Init()
{
    pinReadyPAN.Init();
    pinDataPAN.Init();
    pinCS.Init();

    DataBus::Init();
}


bool HAL_BUS::Panel::Receive()
{
    //if(pinReadyPAN.IsPassive() || pinDataPAN.IsPassive())
    if((GPIOA->IDR & GPIO_PIN_7) || (GPIOC->IDR & GPIO_PIN_4))
    {
        return false;
    }
   
    interactionWithPanel = true;
    
    if(mode != Mode::PanelRead)
    {
        mode = Mode::PanelRead;

        //pinWR.Init();
        //pinRD.Init();
        GPIOD->MODER &= 0xfffff0ffU;
        GPIOD->MODER |= 0x00000500U;
        GPIOD->BSRR = (GPIO_PIN_5 | GPIO_PIN_4);

        // ������������� �� �� ������

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3

        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
    }
    
    //pinRD.SetActive();
    GPIOD->BSRR = GPIO_PIN_4 << 16;

    //pinCS.SetActive();
    GPIOG->BSRR = GPIO_PIN_12 << 16;
    
    //pinReadyPAN.WaitPassive();
    //while(pinReadyPAN.IsActive())

    volatile uint val = GPIOA->IDR;

    while((val & GPIO_PIN_7) == 0)
    {
        val = GPIOA->IDR;
    }
    
    //                                                 4,5,6,7              2,3                          0,1
    uint8 data = static_cast<uint8>((GPIOE->IDR >> 3) & 0xF0 | (GPIOD->IDR << 2) & 0x0C | (GPIOD->IDR >> 14));

    DDecoder::AddData(data);
    
    //pinRD.SetPassive();
    GPIOD->BSRR = GPIO_PIN_4;

    //pinCS.SetPassive();
    GPIOG->BSRR = GPIO_PIN_12;
    
    interactionWithPanel = false;

    return true;
}


void HAL_BUS::Panel::Send(uint8 byte)
{
    Send(&byte, 1);
}


void HAL_BUS::Panel::Send(uint8 byte0, uint8 byte1)
{
    uint8 buffer[2] = { byte0, byte1 };
    
    Send(buffer, 2);
}

#ifdef __cplusplus
extern "C" {
#endif

extern void CycleSend(void);
    
#ifdef __cplusplus
}
#endif

void HAL_BUS::Panel::Send(uint8 *data, uint size)
{
    interactionWithPanel = true;

    if(mode != Mode::PanelWrite)
    {
        mode = Mode::PanelWrite;

        //pinWR.Init();
        //pinRD.Init();
        GPIOD->MODER &= 0xfffff0ffU;
        GPIOD->MODER |= 0x00000500U;
        GPIOD->BSRR = (GPIO_PIN_5 | GPIO_PIN_4);

        // ������������� �� �� ������

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP

        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
    }

    for(uint i = 0; i < size; i++)
    {
        uint8 d = *data++;

        //DataBus::Write(byte);               // ���������� ������ �� ����
        //                                                                             ���� 0,1                                 ���� 2,3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // ���������� ������ � �������� ����
        //                                                                          ���� 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3);

        //pinWR.SetActive();                  // ��� ������ ������
        // HAL_PIO::Reset(PIN_WR);
        //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
        
        //   GPIOD->BSRR = GPIO_PIN_5 << 16;
        
        CycleSend();
        
        //while(pinReadyPAN.IsPassive()) {}   // � ������� ������ ������ � ���, ��� ��� ��������
        //while(HAL_PIO::Read(PIN_PAN_READY) == 1) {}
        //while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET) {}
        //while(GPIOA->IDR & GPIO_PIN_7)
        //{
        //}

        //pinCS.SetActive();                  // ��� ������� ����, ��� ������ ���������� � ����� �� ���������
        //HAL_PIO::Reset(PIN_CS);
        //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
        //GPIOG->BSRR = GPIO_PIN_12 << 16;

        //while(pinReadyPAN.IsActive()) {}    // ������������ PIN_PAN_READY � ���������� ��������� ��������, ��� ������ ������� ������ � ������������ ��
        //while(HAL_PIO::Read(PIN_PAN_READY) == 0) {}
        //while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET) {}
        //while((GPIOA->IDR & GPIO_PIN_7) == 0)
        //{
        //}

        //pinWR.SetPassive();                 // \ ������������� WR � CS � ���������� ��������� - ������������ ���� ������ �������
        //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
        //GPIOD->BSRR = GPIO_PIN_5;

        //pinCS.SetPassive();                 // /
        //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
        GPIOG->BSRR = GPIO_PIN_12;
    }

    interactionWithPanel = false;
}


bool HAL_BUS::Panel::InInteraction()
{
    return interactionWithPanel;
}


void DataBus::Init()
{
    // ������������� �� �� ������

    GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3

    GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
}
