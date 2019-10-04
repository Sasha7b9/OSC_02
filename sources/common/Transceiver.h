#pragma once

/*
    ���������������� �������������� ����� ������������
*/



struct Transceiver
{
    /// ���������� ��������� ��������� �� �������, ������� ����� ���������� ��� ������������� ����� �� ���� ��� ��������, ����� ������� ��������� �����, ��� ����� ������������� ������,
    /// ���� �� ����� ������������ �� � ������ ������.
    static void Init();

    static bool InInteraction();

    /// ������� ������������ ������. ��������� ������, ���� ������� �������, � ������� �� ����� �������, ������������� � SetCallbackReceive.
    static bool Update();

    struct Transmitter
    {
        static void Send(const uint8 *data, uint size);
        static void Send(uint8 data);
        static void Send(uint8 byte0, uint8 byte1);
    };
};

