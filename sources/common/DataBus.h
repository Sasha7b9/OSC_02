#pragma once


namespace DataBus
{
    /// �����, � ������� ������ ��������� ����
    struct Mode
    {
        enum E
        {
            DeviceTransmit, ///< �������� � ������
            DeviceReceive,  ///< ������ �� ������
            FPGA            ///< ����� � FPGA
        };
    };

    extern Mode::E mode;
}
