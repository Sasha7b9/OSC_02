#pragma once
#include "Hardware/Memory/Memory.h"


#define STATE_FREE    0xFFFFFFFFU
#define STATE_ERASED  0x00000000U
#define STATE_VALID   0xFF00FF00U

#define TYPE_DATA     0U


struct Packet
{
    uint  state;    // ��������� ������:
                    // 0xFFFFFFFF - � ����� ������ �� �������������
                    // 0x00000000 - ����� ����
                    // 0xFF00FF00 - ����������� �����
    uint16 size;    // ������ ������ - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // ��� ������

    bool IsData() const { return (state == STATE_VALID) && (type == TYPE_DATA); }
    bool IsFree() const { return (state == STATE_FREE); }
    bool IsErased() const { return (state == STATE_ERASED); }
    bool IsValid() const { return (state == STATE_VALID); }
    Packet *Next() const;
    bool UnPack(DataSettings **ds) const;
    int Size() const;
    /// ������ ������� ������ ������ � ������ sector. � ������ ������� ���������� false
    bool WriteToSector(const Sector *sector) const;
};


struct Compressor
{
    /// ���������� ���������� ����, ����������� ��� �������� ������
    static uint GetPackedSize(const DataSettings *ds);
    /// ����������� ������ � ���������� ds �� ������ address
    static void Pack(const DataSettings *ds, uint address);
    /// ����������� ����� �� src � dest
    static void Copy(Packet *dest, const Packet *src);
};
