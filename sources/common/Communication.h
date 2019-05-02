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
        int(*ReadALLOW_SEND)(),
        int(*ReadCONF_DATA)(),
        void(*SetCLK)(),
        void(*ResetCLK)()
    );
    /// �������� size ����, ������� � ������ data
    void Send(uint8 *data, uint size);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Receiver
{
    void SetCallbacks(
        void(*InitPins)(),
        int(*ReadREQ_SEND)(),
        void(*SetALLOW_SEND)(),
        void(*ResetALLOW_SEND)(),
        void(*SetCONF_DATA)(),
        void(*ResetCONF_DATA)(),
        int(*ReadCLK)(),
        void(*FuncRead)(uint8)
    );
    /// ������� ������������ ������. ��������� ������, ���� ������� �������, � ������� �� ����� �������, ������������� � SetCallbackReceive.
    void Update();
};

