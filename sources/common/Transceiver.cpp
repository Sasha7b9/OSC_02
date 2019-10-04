#include "defines.h"
#include "Transceiver.h"


DataBusMode::E DataBusMode::state = DataBusMode::FPGA;


//namespace Transmitter
//{
//    void Init();
//    /// ������������� ����� ��� ������ ��������.
//    void InitPinsSend();
//}

namespace Receiver
{
    void Init();
    /// ������������� ����� ��� ������ �����.
    void InitPinsRecieve();
}

/// ������������ � true �������� ��������, ��� ��� ����� � �������
bool inInteraction = false;



void Transceiver::Transmitter::Send(uint8 byte0, uint8 byte1)
{
    uint8 data[2] = { byte0, byte1 };
    Send(data, 2);
}


bool Transceiver::InInteraction()
{
    return inInteraction;
}
