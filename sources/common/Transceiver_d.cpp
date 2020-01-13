#include "defines.h"
#include "Decoder_d.h"
#include "Transceiver.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>

#ifdef DEVICE
#include "Osci/Osci.h"
#include "Recorder/Recorder.h"
#endif


struct PinWR
{
    static void SetActive()
    {
        HAL_PIO::Reset(PIN_P_WR);
    }
    static void SetPassive()
    {
        HAL_PIO::Set(PIN_P_WR);
    }
};


struct PinRD
{
    static void SetAcitve();
    static void SetPassive();
};


struct PinCS
{
    static void SetActive()
    {
        HAL_PIO::Reset(PIN_P_CS);
    }
    static void SetPassive()
    {
        HAL_PIO::Set(PIN_P_CS);
    }
};


struct Panel
{
    static bool IsBusy()
    {
        return HAL_PIO::Read(PIN_P_BUSY) == 0;
    }
    static void IsDataReady();
};


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


struct Receiver
{
    /// ��������� ���� ������ � ��
    static uint8 ReadData();
};


void Transceiver::Init()
{
    HAL_PIO::Init(PIN_P_BUSY, HMode::Input, HPull::Up);
    HAL_PIO::Init(PIN_P_DATA_READY, HMode::Input, HPull::Up);
}


void Transceiver::Send(uint8 data)
{
    Send(&data, 1);
}


static void SendByte(uint8 byte)
{
    // ������������� ������ �� ���� ������

    //                                                                                ���� 0,1                                    ���� 2,3
    GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(byte) & 0x03) << 14) + ((static_cast<uint16>(byte & 0x0c)) >> 2);  // ���������� ������ � �������� ����
    //                                                                             ���� 4,5,6,7
    GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(byte) & 0xf0) << 3);

    PinWR::SetActive();             // ������������� ���� ������

    while(Panel::IsBusy()) {};      // � ���, ���� ������ �������� ���� ���������� � ��������������

    PinCS::SetActive();             // ������������� ���������

    while(!Panel::IsBusy()) {};     // ���, ���� ������ ����� ������� � ������� ��������� - ������� �����

    PinCS::SetPassive();            // ����������� ���� ������
    PinWR::SetPassive();
}


void Transceiver::Send(const uint8 *data, uint size)
{
    inInteraction = true;

    if (DataBusMode::state != DataBusMode::DeviceTransmit)        // ���� ���� ��� �� ���������������� ��� �������� -
    {
        DataBusMode::state = DataBusMode::DeviceTransmit;          // ��������������
        
        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
        
        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP

        HAL_PIO::InitOutput(PIN_P_CS, HPull::Down, 1);
        HAL_PIO::InitOutput(PIN_P_WR, HPull::Down, 1);
        HAL_PIO::InitOutput(PIN_P_RD, HPull::Down, 1);
    }


    for (uint i = 0; i < size; i++)
    {
        SendByte(*data++);
    }

    inInteraction = false;
}


bool Transceiver::Receive()
{
    return false;
}


uint8 Receiver::ReadData()
{
    uint8 result = HAL_PIO::Read(PIN_P_D7);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D6);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D5);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D4);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D3);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D2);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D1);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D0);

    return result;
}
