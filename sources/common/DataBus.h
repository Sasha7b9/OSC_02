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
        } value;
        Mode(E v) : value(v) {};

        bool IsFPGA() const { return value == Mode::FPGA; }

        bool IsDeviceTransmit() const { return value == Mode::DeviceTransmit; }
    };

    void SetModeTransmit();

    void SetModeReceive();

    void SetModeFPGA();

    Mode GetMode();
}
