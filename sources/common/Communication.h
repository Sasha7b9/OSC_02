#pragma once

/*
    ���������������� �������������� ����� ������������
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Transceiver
{
    void SetCallbacks(
        void(*InitSendPin)(),
        void(*InitPins)(),
        void(*SetREQ_SEND)(),
        void(*ResetREQ_SEND)(),
        bool(*ReadALLOW_SEND)(),
        bool(*ReadCONF_DATA)(),
        void(*SetCLK)(),
        void(*ResetCLK)(),
        void(*SetDATA)(),
        void(*ResetDATA)()
    );
    /// �������� size ����, ������� � ������ data
    void Send(uint8 *data, uint size);
    void Send(uint8 data);
    void Send(uint8 byte0, uint8 byte1);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Receiver
{
    void SetCallbacks(
        void(*InitPins)(),
        bool(*ReadREQ_SEND)(),
        void(*SetALLOW_SEND)(),
        void(*ResetALLOW_SEND)(),
        void(*SetCONF_DATA)(),
        void(*ResetCONF_DATA)(),
        bool(*ReadCLK)(),
        void(*FuncRead)(uint8)
    );
    /// ������� ������������ ������. ��������� ������, ���� ������� �������, � ������� �� ����� �������, ������������� � SetCallbackReceive.
    void Update();
};

