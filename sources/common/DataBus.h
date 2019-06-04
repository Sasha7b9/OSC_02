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

    void SetModeTransmit();

    void SetModeReceive();

    void SetModeFPGA();

    Mode::E GetMode();
}
