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
    void SetLow()     { HAL_PIO::Reset(port, pin); }
    void SetHi()      { HAL_PIO::Set(port, pin); }

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


// ������������ � "0" ����� ���� �������� ������ �������� ��� ����� ������������������� ����
static OutPin pinCS(PIN_CS);
// "0" ��������, ��� ��������� ������ �� ������, "1" - ������ � ������
static OutPin pinType(PIN_TYPE);
// "0" - ��� ������� �������� ���������, "1" - ��������
static OutPin pinNibble(PIN_NIBBLE);
// "0" - �������� ������������� ������������� �� ������� ���
static OutPin pinConfCPU(PIN_CONF_CPU);

// ������������� � "0" ������ ��������, ��� ������ �������������/����������
static InPin pinConfPanel(PIN_CONF_PANEL);
// "0" �� ���� ������ ��������, ��� ������ ������ � ����� ������
static InPin pinReadReady(PIN_READ_READY);
// "1" �� ���� ������ ��������, ��� � ������ ���� ������ ��� ��������
static InPin pinSendReady(PIN_SEND_READY);

/// true ��������, ��� ���� ��������� � �������� ������ � ������� � ������ �� ������� FSMC � ������� � ������ ���������
static bool interactionWithPanel = false;

void HAL_BUS::InitPanel()
{
    pinReadReady.Init();
    pinSendReady.Init();
    pinCS.Init();
}


void HAL_BUS::Panel::Receive()
{
    if(pinSendReady.IsPassive())
    {
        return;
    }

    interactionWithPanel = true;

    if(mode != Mode::PanelRead)
    {
        mode = Mode::PanelRead;

        pinType.Init();
        pinConfPanel.Init();
        pinNibble.Init();
        pinConfCPU.Init();

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
    }

    while(pinSendReady.IsActive())
    {
        pinType.SetLow();                   // ��� �������� - ������
        pinNibble.SetLow();                 // ����� ������ ������� ��������
        pinCS.SetActive();                  // ��� ������� ������ ������

        while(pinConfPanel.IsPassive()) {}   // ������� ������������� ����, ��� ������ ��������� ������.

        uint8 nibble0 = static_cast<uint8>((GPIOD->IDR << 2) & 0x0c | (GPIOD->IDR >> 14));

        pinCS.SetPassive();                 // ����������� ������� ����, ��� ������������� ��������

        while(pinConfPanel.IsActive()) {}   // � �������, ���� ������ ���������� ���

        pinNibble.SetHi();
        pinCS.SetActive();

        while(pinConfPanel.IsPassive()) {}  // �������, ���� ������ �������� ������. � ���� ����� ������ ������� ������� pinSendReady, ���� ������ ��� �������� ������ ���

        uint8 nibble1 = static_cast<uint8>((GPIOD->IDR << 2) & 0x0c | (GPIOD->IDR >> 14));

        pinCS.SetPassive();

        DDecoder::AddData(static_cast<uint8>(nibble0 | (nibble1 << 4)));
    }

    interactionWithPanel = false;
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

void HAL_BUS::Panel::Send(uint8 *data, uint size)
{
    interactionWithPanel = true;

    if(mode != Mode::PanelWrite)
    {
        mode = Mode::PanelWrite;

        pinType.Init();
        pinConfPanel.Init();
        pinNibble.Init();
        pinConfCPU.Init();

        GPIOD->MODER &= 0x0ffffff0U;    // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;    // ������������� ��� ���� ����� ����� GPIO_MODE_OUTPUT_PP
    }

    while(size)
    {
        uint8 n0 = static_cast<uint8>((*data) & 0x0F);
        uint8 n1 = static_cast<uint8>(((*data) & 0xF0) >> 4);
        data++;

        while(pinReadReady.IsPassive()) {}      // ��� ���� ������ ����������� ��� ������

        pinType.SetHi();                        // ��� �������������� - ������ � ������
        pinNibble.SetLow();                     // ��� ���������� - ������� ��������

        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<uint16>(n0) & 0x03) << 14) + ((static_cast<uint16>(n0 & 0x0c)) >> 2);

        pinCS.SetActive();                      // ��� ������ �������, ��� ������ ���������� �� ����

        while(pinConfPanel.IsPassive()) {}      // �������, ���� ������ ������� ������ � ������� �� ����

        pinCS.SetPassive();

        pinNibble.SetHi();                      // ��� ���������� - ������� ��������

        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<uint16>(n1) & 0x03) << 14) + ((static_cast<uint16>(n1 & 0x0c)) >> 2);

        pinCS.SetActive();                      // ��� ������ �������, �� ������ �������� ��������� �� ����

        while(pinConfPanel.IsPassive()) {}      // �������, ����� ������ ������� ������ � ������� �� ����

        pinCS.SetPassive();
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
