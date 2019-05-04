#include "defines.h"
#include "Transceiver.h"

/*
    +------------+-------------+---+-------------+---+
    |    ����.   |    437ZIT       |    429VIT6      |
    |            |     FPGA        |     ������      |
    +------------+-------------+---+-------------+---+
    |            |  ���������� |   |  ��Ȩ����   |   |
    | REQ_SEND   | 43    PA7   | O | 8     PC14  | I |
    | ALLOW_SEND | 85    PD14  | I | 97    PE0   | O |
    | DATA       | 86    PD15  | O | 98    PE1   | I |
    | CLK        | 114   PD0   | O | 1     PE2   | I |
    | CONF_DATA  | 115   PD1   | I | 2     PE3   | 0 |
    |            |  ��Ȩ����   |   |  ���������� |   |
    | REQ_SEND   | 44    PC4   | I | 9     PC15  | O |
    | ALLOW_SEND | 58    PE7   | O | 3     PE4   | I |
    | DATA       | 59    PE8   | I | 4     PE5   | O |
    | CLK        | 60    PE9   | I | 5     PE6   | O |
    | CONF_DATA  | 63    PE10  | O | 38    PE7   | I |
    +------------+-------------+---+-------------+---+
*/

namespace Transceiver
{
    namespace Transmitter
    {
        /// ������� ������������� ����� ��� ������ ��������
        void InitPins();
        /// ��������/����� REQ_SEND
        void Write_REQ_SEND(int);
        /// ������ ���� ���������� ��������
        bool Read_ALLOW_SEND();
        /// ������ ������������� ������
        bool(*Read_CONF_DATA)() = nullptr;
        /// ���������� ��� ������ � ������������ � ����� bit ����� byte
        void WritePinBit(uint8 byte, int bit);
        /// ����������/�������� �������� ���
        void(*Write_CLK)(int) = nullptr;
        /// ���������� ��������� ���� ������
        void(*Write_DATA)(int) = nullptr;
        /// ��������� �����
        void SendByte(uint8 data);
        /// ��������� ���������� �� ��������
        void WaitPermitSend();
        /// �������� ������������� �������� ����
        void WaitPermitData();
    }

    namespace Receiver
    {
        void(*InitPins)() = nullptr;
        bool(*ReadREQ_SEND)() = nullptr;
        void(*WriteALLOW_SEND)(int) = nullptr;
        void(*WriteCONF_DATA)(int) = nullptr;
        bool(*ReadCLK)() = nullptr;
        void(*FuncRead)(uint8) = nullptr;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
    InitPins();

    Write_REQ_SEND(0);

    WaitPermitSend();

    for (uint i = 0; i < size; i++)
    {
        SendByte(data[i]);
    }

    Write_REQ_SEND(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::SendByte(uint8 data)
{
    for (int i = 0; i < 8; i++)
    {
        Write_CLK(1);
        WritePinBit(data, i);
        Write_CLK(0);
        WaitPermitData();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::WaitPermitSend()
{
    while (Read_ALLOW_SEND() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::WaitPermitData()
{
    while (Read_CONF_DATA() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 byte)
{
    Send(&byte, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 byte0, uint8 byte1)
{
    uint8 data[2] = { byte0, byte1 };
    Send(data, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::WritePinBit(uint8 byte, int bit)
{
    Write_DATA((byte >> bit) & 0x01);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{

}
