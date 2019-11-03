#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Compressor.h"
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


void FlashMemory::Data::Save(int /*num*/, const DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
}


void FlashMemory::Data::DeleteAll()
{
}
