#include "defines.h"
#include "Communication.h"

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
    | CONF_DATA  | 115   PD1   | O | 2     PE3   | I |
    |            |  ��Ȩ����   |   |  ���������� |   |
    | REQ_SEND   | 44    PC4   | I | 9     PC15  | O |
    | ALLOW_SEND | 58    PE7   | O | 3     PE4   | I |
    | DATA       | 59    PE8   | I | 4     PE5   | O |
    | CLK        | 60    PE9   | I | 5     PE6   | O |
    | CONF_DATA  | 63    PE10  | I | 38    PE7   | O |
    +------------+-------------+---+-------------+---+
*/


namespace Transceiver
{
    /// ������� ������������� ����� ��� ������ ��������
    void(*InitPins)() = nullptr;
    /// ������� �������� � 1 REQ_SEND
    void(*Set_REQ_SEND)() = nullptr;
    /// ������� ��������� � 0 REQ_SEND
    void(*Reset_REQ_SEND)() = nullptr;
    /// ������ ���� ���������� ��������
    int (*Read_ALLOW_SEND)() = nullptr;
    /// ������ ������������� ������
    int(*Read_CONF_DATA)() = nullptr;
    /// ���������� ��� ������ � ������������ � ����� bit ����� byte
    void(*WritePinBit)(uint8 byte, int bit) = nullptr;
    /// ���������� �������� ��� � �������
    void(*Set_CLK)() = nullptr;
    /// ���������� �������� ��� � ����
    void(*Reset_CLK)() = nullptr;
    /// ��������� �����
    void SendByte(uint8 data);
    /// ��������� ���������� �� ��������
    void WaitPermitSend();
    /// �������� ������������� �������� ����
    void WaitPermitData();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::SetCallbacks(
    void(*initSendPin)(),
    void(*initPins)(),
    void(*setREQ_SEND)(),
    void(*resetREQ_SEND)(),
    int(*readALLOW_SEND)(),
    int(*readCONF_DATA)(),
    void(*setCLK)(),
    void(*resetCLK)())
{
    initSendPin();

    InitPins = initPins;
    Set_REQ_SEND = setREQ_SEND;
    Reset_REQ_SEND = resetREQ_SEND;
    Read_ALLOW_SEND = readALLOW_SEND;
    Read_CONF_DATA = readCONF_DATA;
    Set_CLK = setCLK;
    Reset_CLK = resetCLK;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Send(uint8 *data, uint size)
{
    InitPins();

    Reset_REQ_SEND();

    WaitPermitSend();

    for (uint i = 0; i < size; i++)
    {
        SendByte(data[i]);
    }

    Set_REQ_SEND();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::SendByte(uint8 data)
{
    for (int i = 0; i < 8; i++)
    {
        Set_CLK();
        WritePinBit(data, i);
        Reset_CLK();
        WaitPermitData();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::WaitPermitSend()
{
    while (Read_ALLOW_SEND() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::WaitPermitData()
{
    while (Read_CONF_DATA() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Send(uint8 byte)
{
    Send(&byte, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Send(uint8 byte0, uint8 byte1)
{
    uint8 data[2] = { byte0, byte1 };
    Send(data, 2);
}

namespace Receiver
{
    void(*InitPins)() = nullptr;
    int(*ReadREQ_SEND)() = nullptr;
    void(*SetALLOW_SEND)() = nullptr;
    void(*ResetALLOW_SEND)() = nullptr;
    void(*SetCONF_DATA)() = nullptr;
    void(*ResetCONF_DATA)() = nullptr;
    int(*ReadCLK)() = nullptr;
    void(*FuncRead)(uint8) = nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Receiver::SetCallbacks(
    void(*initPins)(),
    int(*readREQ_SEND)(),
    void(*setALLOW_SEND)(),
    void(*resetALLOW_SEND)(),
    void(*setCONF_DATA)(),
    void(*resetCONF_DATA)(),
    int(*readCLK)(),
    void(*funcRead)(uint8))
{
    InitPins = initPins;
    ReadREQ_SEND = readREQ_SEND;
    SetALLOW_SEND = setALLOW_SEND;
    ResetALLOW_SEND = resetALLOW_SEND;
    SetCONF_DATA = setCONF_DATA;
    ResetCONF_DATA = resetCONF_DATA;
    ReadCLK = readCLK;
    FuncRead = funcRead;
}
