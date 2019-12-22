#pragma once
#include "defines.h"
#include <usbd_def.h>


#define VCP_FLUSH()                       DVCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size) DVCP::SendDataSynch(buffer, size)
#define CONNECTED_TO_USB                  DVCP::connectedToUSB
#define CABLE_USB_IS_CONNECTED            DVCP::cableUSBisConnected


class DVCP
{
public:
    static const int DEVICE_FS = 0;

    /// �������������
    static void Init();

    static void SendDataAsynch(const uint8 *data, uint size);

    static void SendDataSynch(const void *data, uint size = 0);
    /// ��������� ������ ��� ������������ ����
    static void SendStringAsynch(const char *data);
    /// ��������� ������ ��� ������������ ����
    static void SendStringSynch(char *data);
    /// ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringAsynch(char *format, ...);
    /// ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringSynch(char *format, ...);

    static void SendByte(uint8 data);

    static void Flush();

    static void *HandleUSBD();

    static void *HandlePCD();

    static bool connectedToUSB;

    static bool cableUSBisConnected;
};
