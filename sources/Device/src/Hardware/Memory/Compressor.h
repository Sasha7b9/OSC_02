#pragma once
#include "Hardware/Memory/Memory.h"


struct Packet
{
    uint  state;    // ��������� ������:
                    // 0xFFFFFFFF - � ����� ������ �� �������������
                    // 0x00000000 - ����� ����
                    // 0xFF00FF00 - ����������� �����
    uint16 size;    // ������ ������ - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // ��� ������ :
};


#define STATE_FREE    0xFFFFFFFFU
#define STATE_ERASED  0x00000000U
#define STATE_VALID   0xFF00FF00U

#define TYPE_DATA     0U


struct Compressor
{
    /// ���������� ����� ���������� ������ � �������� ������������ ����������������.
    static Packet *NextPacket(Packet *packet);
    /// ���������� ���������� ����, ����������� ��� �������� ������
    static uint GetPackedSize(const DataSettings *ds);
    /// ����������� ������ � ���������� ds �� ������ address
    static void Pack(const DataSettings *ds, uint address);
    /// ���������� ��������� ds �� ������������ ������ �� ������ address
    static bool UnPack(uint address, DataSettings **ds);
};
