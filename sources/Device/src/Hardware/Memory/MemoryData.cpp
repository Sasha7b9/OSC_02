#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Packet.h"
#include "Hardware/Memory/Memory.h"


static const int NUM_SECTORS = 5;

/// ��� ������� ������������� ��� �������� ������
static const Sector *sectors[5] =
{
    &SECTOR(Sector::_19_DATA_1),
    &SECTOR(Sector::_20_DATA_2),
    &SECTOR(Sector::_21_DATA_3),
    &SECTOR(Sector::_22_DATA_4),
    &SECTOR(Sector::_23_DATA_5)
};


/// ����� �������� ������ - ���������������� �� �� �������� ����� �������, ����� �������� ��������� ���������� ��� ������ ��� �������
static void Compress();
/// ���������� ���������� ��������� ��������� ��� ������ ��������
static int NumberFreeSectors();
/// ���������� ��������� �� ������ ��������� ������
static const Sector *GetFirstFreeSector();
/// ���������� ��������� �� �������� ������� ������ - ��� ������ ������ �������� ������ ����� �����
static const Sector *GetMostWornSector();
/// ���������� ���������� ����, ���������� ������� ��������
static int GetNumberWornBytes(const Sector *sector);
/// ����������� ������ �� ������� src � ������ dest
//static void CopyData(const Sector *sectorDest, const Sector *sectorSrc);
/// ����������� ������ � ��������� �����
static void CopyDataToFreeSpace(const Sector *src);
/// ���������� ������ �� ������� src � ������ dest
//static void CopyDataToSector(const Sector *dest, const Sector *src);
/// ���������� ��������� �� ������ ����� � ������� �� ������� sector � nullptr, ���� ������ � ������� ���
static const Packet *GetFirstPacketWithData(const Sector *sector);
/// ���������� ��������� �� ������ ����� � ������� �� packet, ��� nullptr, ���� ������� � ������� � ������� ������ ���
//static const Packet *GetNextPacketWithData(const Sector *sector, const Packet *packet);
/// ���������� true, ���� � ������ �� ���� �������� �� ������ ������
static bool SectorIsFree(const Sector *sector);



/// ��������� true � �� �������� ������� existData[], ������� ������������ ������������ � ������ ������� ���������� ������
static void FillInfoFromSector(const Sector *sector, bool existData[MAX_NUM_SAVED_WAVES])
{
    Packet *packet = reinterpret_cast<Packet *>(sector->address);

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {
            DataSettings *ds;
            if (packet->UnPack(&ds))
            {
                existData[ds->numROM] = true;
            }
        }
        packet = packet->Next();
    }
}


void FlashMemory::Data::GetInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        existData[i] = false;
    }

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        FillInfoFromSector(sectors[i], existData);
    }
}


static bool GetData(const Sector *, int num, DataSettings **, uint8 **, uint8 **)
{
    Packet *packet = reinterpret_cast<Packet *>(ADDR_SECTOR(num));

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {

        }

        packet = packet->Next();
    }

    return false;
}


bool FlashMemory::Data::Get(int num, DataSettings **ds, uint8 **dataA, uint8 **dataB)
{
    *ds = nullptr;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        if (GetData(sectors[i], num, ds, dataA, dataB))
        {
            return true;
        }
    }

    return false;
}


void FlashMemory::Data::Delete(int /*num*/)
{
}


void FlashMemory::Data::Save(int, const DataSettings *ds, uint8 *dataA, uint8 *dataB)
{
    Compress();

    const_cast<DataSettings *>(ds)->dataA = dataA;
    const_cast<DataSettings *>(ds)->dataB = dataB;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const Sector *sector = sectors[i];

        if (sector->NotExistPackets())          // ���������� �������, � ������� ��� ��� �������
        {
            continue;
        }

        if (sector->WritePacket(ds))
        {
            return;
        }
    }

    // ���� ����� � �������� ������� �������� ���, ����� ���������� � ���������

    const Sector *sector = GetFirstFreeSector();

    sector->WritePacket(ds);
}


void FlashMemory::Data::DeleteAll()
{
}


static void Compress()
{
    while (NumberFreeSectors() < 2)
    {
        const Sector *src = GetMostWornSector();

        CopyDataToFreeSpace(src);

        src->Erase();
    }
}


static int NumberFreeSectors()
{
    int result = 0;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        Packet *packet = reinterpret_cast<Packet *>(sectors[i]->address);

        if (packet->IsFree())
        {
            result++;
        }
    }

    return result;
}


static const Sector *GetFirstFreeSector()
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        Packet *packet = reinterpret_cast<Packet *>(sectors[i]->address);

        if (packet->IsFree())
        {
            return sectors[i];
        }
    }

    return nullptr;
}


static const Sector *GetMostWornSector()
{
    int numWornBytes = 0;               /// ���������� ������ ���� 

    const Sector *sector = nullptr;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        int numBytes = GetNumberWornBytes(sectors[i]);

        if (numBytes > numWornBytes)
        {
            numWornBytes = numBytes;
            sector = sectors[i];
        }
    }

    return sector;
}


static int GetNumberWornBytes(const Sector *sector)
{
    int result = 0;

    Packet *packet = reinterpret_cast<Packet *>(sector->address);

    while (packet && !packet->IsFree())
    {
        if (packet->IsErased())
        {
            result += packet->Size();
        }

        packet = packet->Next();
    }

    return result;
}


//static void CopyData(const Sector *sectorDest, const Sector *sectorSrc)
//{
//    Packet *src = reinterpret_cast<Packet *>(sectorSrc->address);
//
//    Packet *dest = reinterpret_cast<Packet *>(sectorDest->address);
//
//    while (src && !src->IsFree())
//    {
//        if(!src->IsErased())
//        {
//            Compressor::Copy(dest, src);
//            dest = dest->Next();
//        }
//
//        src = src->Next();
//    }
//}


static void CopyDataToFreeSpace(const Sector *sectorSrc)
{
    /*
        ���� ���� ��������� ����� � �������� ��������, ����� ����.
        ����� ����� � ������ �������.
    */

    const Packet *src = GetFirstPacketWithData(sectorSrc);

    if (!src)
    {
        return;
    }

    for (int i = 0; i < NUM_SECTORS; i++)                           // ������� ���������� ������ � ��� ������� �������
    {
        const Sector *sector = sectors[i];

        if (sector == sectorSrc || SectorIsFree(sector))
        {
            continue;
        }

        while (src)
        {
            if (!src->WriteToSector(sector))
            {
                break;
            }

            src = src->Next();
        }
    }
}


//static void CopyDataToSector(const Sector *, const Sector *sectorSrc)
//{
//    const Packet *src = GetFirstPacketWithData(sectorSrc);
//
//    if (!src)
//    {
//        return;
//    }
//
//    while (src)
//    {
//
//    }
//}


static const Packet *GetFirstPacketWithData(const Sector *sector)
{
    const Packet *result = reinterpret_cast<const Packet *>(sector->address);

    while (result && !result->IsFree())
    {
        if (result->IsValid())
        {
            return result;
        }

        result = result->Next();
    }

    return nullptr;
}


static bool SectorIsFree(const Sector *sector)
{
    Packet *packet = reinterpret_cast<Packet *>(sector->address);

    return packet->IsFree();
}
