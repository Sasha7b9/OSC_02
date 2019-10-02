#pragma once

/*
    ���������������� �������������� ����� ������������
*/



namespace Transceiver
{
    /// ���������� ��������� ��������� �� �������, ������� ����� ���������� ��� ������������� ����� �� ���� ��� ��������, ����� ������� ��������� �����, ��� ����� ������������� ������,
    /// ���� �� ����� ������������ �� � ������ ������.
    void Init();

    bool InInteraction();

    /// ������� ������������ ������. ��������� ������, ���� ������� �������, � ������� �� ����� �������, ������������� � SetCallbackReceive.
    bool Update();

    ////
    namespace Transmitter
    {
        void Send(const uint8 *data, uint size);
        void Send(uint8 data);
        void Send(uint8 byte0, uint8 byte1);
    };
};

