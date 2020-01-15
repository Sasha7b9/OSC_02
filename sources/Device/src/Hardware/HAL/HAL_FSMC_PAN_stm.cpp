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
    /// ��������� ���� � ���� ������
    static uint8 Read();
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
static void SendByte(uint8 byte);


void HAL_FSMC::InitPanel()
{
    pinReadyPAN.Init();
    pinDataPAN.Init();
    pinCS.Init();

    DataBus::Init();
}


void HAL_FSMC::ConfigureToReadPanel()
{
    mode = Mode::PanelRead;

    pinWR.Init();
    pinRD.Init();

    // ������������� �� �� ������

    GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3

    GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
}


void HAL_FSMC::ConfigureToWritePanel()
{
    mode = Mode::PanelWrite;

    pinWR.Init();
    pinRD.Init();

    // ������������� �� �� ������

    GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
    GPIOD->MODER |= 0x50000005U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP

    GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
    GPIOE->MODER |= 0x00154000U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
}


bool HAL_FSMC::Receive()
{
    if(pinReadyPAN.IsPassive() || pinDataPAN.IsPassive())
    {
        return false;
    }
   
    interactionWithPanel = true;
    
    if(mode != Mode::PanelRead)
    {
        ConfigureToReadPanel();
    }
    
    pinRD.SetActive();
    pinCS.SetActive();
    
    pinReadyPAN.WaitPassive();
    
    uint8 data = DataBus::Read();

    DDecoder::AddData(data);
    
    pinRD.SetPassive();
    pinCS.SetPassive();
    
    interactionWithPanel = false;

    return true;
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
    while(Receive())
    {

    }

    interactionWithPanel = true;

    if(mode != Mode::PanelWrite)
    {
        ConfigureToWritePanel();
    }

    for(uint i = 0; i < size; i++)
    {
        SendByte(*data++);
    }

    interactionWithPanel = false;
}


static void SendByte(uint8 d)
{
    //DataBus::Write(byte);               // ���������� ������ �� ����
        //                                                                             ���� 0,1                                 ���� 2,3
    GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // ���������� ������ � �������� ����
    //                                                                          ���� 4,5,6,7
    GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3);

    //pinWR.SetActive();                  // ��� ������ ������
    // HAL_PIO::Reset(PIN_WR);
    //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
    GPIOD->BSRR = GPIO_PIN_5 << 16;

    //while(pinReadyPAN.IsPassive()) {}   // � ������� ������ ������ � ���, ��� ��� ��������
    //while(HAL_PIO::Read(PIN_PAN_READY) == 1) {}
    //while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET) {}
    while(GPIOA->IDR & GPIO_PIN_7) {}

    //pinCS.SetActive();                  // ��� ������� ����, ��� ������ ���������� � ����� �� ���������
    //HAL_PIO::Reset(PIN_CS);
    //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
    GPIOG->BSRR = GPIO_PIN_12 << 16;

    //while(pinReadyPAN.IsActive()) {}    // ������������ PIN_PAN_READY � ���������� ��������� ��������, ��� ������ ������� ������ � ������������ ��
    //while(HAL_PIO::Read(PIN_PAN_READY) == 0) {}
    //while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET) {}
    while((GPIOA->IDR & GPIO_PIN_7) == 0) {}

    //pinWR.SetPassive();                 // \ ������������� WR � CS � ���������� ��������� - ������������ ���� ������ �������
    //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
    GPIOD->BSRR = GPIO_PIN_5;

    //pinCS.SetPassive();                 // /
    //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
    GPIOG->BSRR = GPIO_PIN_12;
}


bool HAL_FSMC::InteractionWithPanel()
{
    return interactionWithPanel;
}


void DataBus::Init()
{
    // ������������� �� �� ������

    GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3

    GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
}


uint8 DataBus::Read()
{
    uint16 d = GPIOD->IDR;
    uint16 e = GPIOE->IDR;

    uint8 result = HAL_PIO::Read(PIN_D7);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D6);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D5);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_D4);
    result <<= 4;

    result |= (d << 2) & 0x0C;

    result |= (d >> 14);

    return result;
}
