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
        void(*WriteREQ_SEND)(int),
        bool(*ReadALLOW_SEND)(),
        bool(*ReadCONF_DATA)(),
        void(*WriteCLK)(int),
        void(*WriteDATA)(int)
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

