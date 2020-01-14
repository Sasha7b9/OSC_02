#include "defines.h"
#include "Decoder_d.h"
#include "Transceiver.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>

#ifdef DEVICE
#include "Osci/Osci.h"
#include "Recorder/Recorder.h"
#endif


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


/// ��������� ������ ������
void Set_MODE(Mode::E mode);

State::E State_READY();

struct Receiver
{
    /// ������������� FL0 �� ������
    static void Init_FL0_IN();
    /// ���������� ��������� FL0
    static State::E State_FL0();
    /// ��������� ���� ������ � ��
    static uint8 ReadData();
};


void Transceiver::Init()
{
    HAL_PIO::Init(PIN_MODE0, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_MODE1, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_READY, HMode::Input, HPull::Down);    // ������������ ��� ������ ������������� �� ������

    Set_MODE(Mode::Disabled);
}


void Receiver::Init_FL0_IN()
{
    HAL_PIO::Init(PIN_FL0, HMode::Input, HPull::Down);  // ����� �� ���� ������ ��������, ���� �� � ������ ������ ��� ��������
}


void Transceiver::Send(uint8 data)
{
    Send(&data, 1);
}


void Transceiver::Send(const uint8 *data, uint size)
{
#ifdef DEVICE
    Recorder::ReadPoint();
    Osci::ReadPointP2P();
#endif
    inInteraction = true;

    if (DataBusMode::state != DataBusMode::DeviceTransmit)        // ���� ���� ��� �� ���������������� ��� �������� -
    {
        DataBusMode::state = DataBusMode::DeviceTransmit;          // ��������������
        
        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
        
        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
    }

    for (uint i = 0; i < size; i++)
    {
        uint8 d = *data++;

        //                                                                             ���� 0,1                                 ���� 2,3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // ���������� ������ � �������� ����
        //                                                                          ���� 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3);

        HAL_PIO::Set(PIN_MODE1);                     // ���������� MODE1 � "1" - ��� ��������, ��� M0M1 == 01 � ���������� ��� ������������ �� ������ � �������� ������

        while(HAL_PIO::Read(PIN_READY) == 0) {};     // ������� ������ ������������� - "1" �� READY ����� ��������, ��� ������ ������� ������ //-V712

        HAL_PIO::Reset(PIN_MODE1);                   // ���������� MODE1 � "0" - ��� ��������, ��� ���������� � ��������� Disable

        while(HAL_PIO::Read(PIN_READY) == 1) {};         // �������, ����� ������� �� READY ������ ����� "0". //-V712
    }

    inInteraction = false;

#ifdef DEVICE
    Recorder::ReadPoint();
    Osci::ReadPointP2P();
#endif
}


bool Transceiver::Receive()
{
#ifdef DEVICE
    Osci::ReadPointP2P();
#endif

    inInteraction = true;

    DataBusMode::state = DataBusMode::DeviceReceive;

    Receiver::Init_FL0_IN();                        // �������������� FL0 �� ������

    Set_MODE(Mode::Receive);                        // �������� ������, ��� ������ ������� ������

    while (State_READY() == State::Passive)         // ������� ������ ���������� �� ������
    {
    };     

    if (Receiver::State_FL0() == State::Passive)    // ���� ������ �������� � ���, ��� ������ ���
    {
        Set_MODE(Mode::Disabled);                   // �� ��������� �������������� � �������

        return false;                               // � �������
    }

    HAL_PIO::TuneDataPinsToReceive();               // �������������� ���� ������ �� ����

    uint8 data = Receiver::ReadData();              // ������ ����
    
    DDecoder::AddData(data);                         // � ���������� ��� �� ����������

    Set_MODE(Mode::Disabled);

    inInteraction = false;

#ifdef DEVICE
    Osci::ReadPointP2P();
#endif

    return true;
}


State::E State_READY()
{
    return  HAL_PIO::Read(PIN_READY) ? State::Active : State::Passive;
}


void Set_MODE(Mode::E mode)
{
    if (mode == Mode::Send)
    {
        HAL_PIO::Reset(PIN_MODE0);
        HAL_PIO::Set(PIN_MODE1);
    }
    else if (mode == Mode::Receive)
    {
        HAL_PIO::Reset(PIN_MODE1);
        HAL_PIO::Set(PIN_MODE0);
    }
    else if (mode == Mode::Disabled)
    {
        HAL_PIO::Reset(PIN_MODE0);
        HAL_PIO::Reset(PIN_MODE1);
        /// \todo � ���� ���� ���-�� ������. ���������, ������ ��� �������� �� ��������
        //Timer::PauseOnOPS(200);
    }
    else
    {
        // ����� ������ �� ����
    }
}


State::E Receiver::State_FL0()
{
    return HAL_PIO::Read(PIN_FL0) ? State::Active : State::Passive;
}


uint8 Receiver::ReadData()
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
