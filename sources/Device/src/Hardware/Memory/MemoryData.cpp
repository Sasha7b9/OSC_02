#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
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


void FlashMemory::Data::GetInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        existData[i] = false;
    }

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        sectors[i]->GetDataInfo(existData);
    }
}


bool FlashMemory::Data::Read(uint numInROM, DataSettings **ds)
{
    *ds = nullptr;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        if (sectors[i]->ReadData(numInROM, ds))
        {
            return true;
        }
    }

    return false;
}


void FlashMemory::Data::Delete(uint numInROM)
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        if (sectors[i]->DeleteData(numInROM))
        {
            return;
        }
    }
}


void FlashMemory::Data::Save(uint numInROM, const DataSettings *ds)
{
    Compress();

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const Sector *sector = sectors[i];

        if (sector->ExistPackets())                 // ����� ������ � ������, � ������� ��� ������������� ������
        {
            if (sector->WriteData(numInROM, ds))
            {
                return;
            }
        }
    }

    // ���� ����� � �������� ������� �������� ���, ����� ���������� � ���������

    GetFirstFreeSector()->WriteData(numInROM, ds);
}


void FlashMemory::Data::Save(uint numInROM, const DataSettings *ds, uint8 *dataA, uint8 *dataB)
{
    const_cast<DataSettings *>(ds)->dataA = dataA;
    const_cast<DataSettings *>(ds)->dataB = dataB;

    Save(numInROM, ds);
}


void FlashMemory::Data::DeleteAll()
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        sectors[i]->Erase();
    }
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
        const Packet *packet = sectors[i]->FirstPacket();

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
        const Packet *packet = sectors[i]->FirstPacket();

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

    const Packet *packet = sector->FirstPacket();

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
//    Packet *src = sectorSrc->FirstPacket();
//
//    Packet *dest = sectorDest->FirstPacket();
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

        if (sector == sectorSrc || !sector->ExistPackets())
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
    const Packet *result = sector->FirstPacket();

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
