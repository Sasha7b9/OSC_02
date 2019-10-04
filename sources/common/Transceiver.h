#pragma once

/*
    ���������������� �������������� ����� ������������
*/



struct Transceiver
{
    /// ���������� ��������� ��������� �� �������, ������� ����� ���������� ��� ������������� ����� �� ���� ��� ��������, ����� ������� ��������� �����, ��� ����� ������������� ������,
    /// ���� �� ����� ������������ �� � ������ ������.
    static void Init();

    /// ������� ������������ ������. ��������� ������, ���� ������� �������, � ������� �� ����� �������, ������������� � SetCallbackReceive.
    static bool Update();

    struct Transmitter
    {
        static void Send(const uint8 *data, uint size);
        static void Send(uint8 data);
        static void Send(uint8 byte0, uint8 byte1);
    };

    static bool inInteraction;
};

/// �����, � ������� ������ ��������� ����
struct DataBusMode
{
    enum E
    {
        DeviceTransmit, ///< �������� � ������
        DeviceReceive,  ///< ������ �� ������
        FPGA            ///< ����� � FPGA
    };

    static DataBusMode::E state;
};
