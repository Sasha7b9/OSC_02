#include "defines.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "Hardware/VCP.h"
#include "usbd_desc.h"
#include "Utils/Math.h"
#include <cstring>
#include <cstdarg>
#include <usbd_core.h>
#include <cstdio>



USBD_HandleTypeDef handleUSBD;
PCD_HandleTypeDef  handlePCD;
bool               DVCP::cableUSBisConnected = false;
bool               DVCP::connectedToUSB = false;



static bool PrevSendingComplete();



void DVCP::Init()
{
    USBD_Init(&handleUSBD, &VCP_Desc, DEVICE_FS);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
}


void *DVCP::HandleUSBD()
{
    return static_cast<void *>(&handleUSBD);
}


void *DVCP::HandlePCD()
{
    return static_cast<void *>(&handlePCD);
}


static bool PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(handleUSBD.pClassData);
    return pCDC->TxState == 0;
}


void DVCP::SendDataAsynch(const uint8 *buffer, uint size)
{
#define SIZE_BUFFER 64U
    static uint8 trBuf[SIZE_BUFFER];

    size = Math::Min(size, SIZE_BUFFER);
    while (!PrevSendingComplete())  {};
    std::memcpy(trBuf, buffer, static_cast<uint>(size));

    USBD_CDC_SetTxBuffer(&handleUSBD, trBuf, static_cast<uint16>(size));
    USBD_CDC_TransmitPacket(&handleUSBD);
}


#define SIZE_BUFFER_VCP 256     /// \todo ���� ��������� ������ ������ 512, �� �� ��207 �����
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;


void DVCP::Flush()
{
    if (sizeBuffer)
    {
        volatile USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(handleUSBD.pClassData);

        while (pCDC->TxState == 1) {}; //-V712

        USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, static_cast<uint16>(sizeBuffer));
        USBD_CDC_TransmitPacket(&handleUSBD);

        while (pCDC->TxState == 1) {}; //-V654 //-V712
    }
    sizeBuffer = 0;
}


void DVCP::SendDataSynch(const void *_buffer, uint size)
{
    if (DVCP::connectedToUSB)
    {
        char *buffer = static_cast<char *>(const_cast<void *>(_buffer));
        if (size == 0)
        {
            size = std::strlen(buffer);
        }

        volatile USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(handleUSBD.pClassData);
    
        do 
        {
            if (sizeBuffer + size > SIZE_BUFFER_VCP)
            {
                int reqBytes = SIZE_BUFFER_VCP - sizeBuffer;
                LIMITATION(reqBytes, 0, static_cast<int>(size));

                while (pCDC->TxState == 1) {}; //-V712

                std::memcpy(buffSend + sizeBuffer, static_cast<void *>(buffer), static_cast<uint>(reqBytes));
                USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, SIZE_BUFFER_VCP);
                USBD_CDC_TransmitPacket(&handleUSBD);
                size -= reqBytes;
                buffer += reqBytes;
                sizeBuffer = 0;
            }
            else
            {
                std::memcpy(buffSend + sizeBuffer, static_cast<void *>(buffer), static_cast<uint>(size));
                sizeBuffer += size;
                size = 0;
            }
        } while (size);
    }
}


void DVCP::SendStringAsynch(const char *data)
{
    SendDataAsynch(reinterpret_cast<uint8 *>(const_cast<char *>(data)), std::strlen(data));
}


void DVCP::SendStringSynch(char *data)
{
    SendDataSynch(reinterpret_cast<uint8 *>(data), std::strlen(data));
}


void DVCP::SendFormatStringAsynch(char *format, ...)
{
    if (DVCP::connectedToUSB)
    {
        static char buffer[200];
        std::va_list args;
        va_start(args, format); //-V2528
        std::vsprintf(buffer, format, args);
        va_end(args);
        std::strcat(buffer, "\r\n");
        SendDataAsynch(reinterpret_cast<uint8 *>(buffer), std::strlen(buffer));
    }
}


void DVCP::SendFormatStringSynch(char *format, ...)
{
    char buffer[200];
    std::va_list args;
    va_start(args, format); //-V2528
    std::vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\r\n");
    SendDataSynch(reinterpret_cast<uint8 *>(buffer), std::strlen(buffer));
}


void DVCP::SendByte(uint8 byte)
{
    SendDataSynch(&byte, 1);
}
