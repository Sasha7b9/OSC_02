#pragma once


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
