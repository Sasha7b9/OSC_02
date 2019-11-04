#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
#include "Data/DataSettings.h"


uint Packet::GetPackedSize(const DataSettings *ds)
{
    return sizeof(Packet) +         // Packet
        sizeof(DataSettings) +      // DataSettings
        ds->NeedMemoryForData();    // data
}


bool Packet::UnPack(DataSettings **ds) const
{
    if (!IsValid() || !IsData())
    {
        return false;
    }

    uint8 *pointer = const_cast<uint8 *>(reinterpret_cast<const uint8 *>(this) + sizeof(Packet));

    *ds = reinterpret_cast<DataSettings *>(pointer);

    return true;
}


Packet *Packet::Next() const
{
    if (IsFree())
    {
        return nullptr;
    }

    return reinterpret_cast<Packet *>(reinterpret_cast<uint8 *>(const_cast<Packet *>(this)) + size);
}


int Packet::Size() const
{
    if (IsFree())
    {
        return 0;
    }

    return size;
}


void Compressor::Copy(Packet *dest, const Packet *src)
{
    FlashMemory::Write(reinterpret_cast<uint>(dest), const_cast<const Packet *>(src), src->Size());
}


bool Packet::WriteToSector(const Sector *sector) const
{
    const Packet *dest = sector->FirstPacket();

    while (dest && !dest->IsFree())
    {
        dest = dest->Next();
    }

    uint addressWrite = reinterpret_cast<uint>(dest);      // �� ����� ������ ����� ������������� ������ ������

    uint lastAddress = addressWrite + size;                   // � ��� ��������� ����� ������

    if (lastAddress > sector->End())
    {
        return false;
    }

    FlashMemory::Write(addressWrite, this, size);

    return true;
}

uint Sector::End() const
{
    return address + size;
}


bool Sector::ExistPackets() const
{
    return !FirstPacket()->IsFree();
}


static void WriteToROM(uint *address, const void *data, int size)
{
    HAL_FLASH::WriteBufferBytes(*address, data, size);
    *address += size;
}


static void TranslateAddressToROM(const DataSettings *ds, const Packet *packet)
{
    uint8 *addressData = reinterpret_cast<uint8 *>(packet->Address() + sizeof(Packet) + sizeof(DataSettings)); // �� ����� ������ ����� �������� ������ ������� �� ������������ �������

    if (ds->enableA)
    {
        const_cast<DataSettings *>(ds)->dataA = addressData;
        addressData += ds->SizeChannel();
    }

    if (ds->enableB)
    {
        const_cast<DataSettings *>(ds)->dataB = addressData;
    }
}


const Packet *Sector::WriteData(int numInROM, const DataSettings *ds) const
{
    const Packet *packet = FirstFreePacket();

    if (packet == nullptr)
    {
        return nullptr;
    }

    uint recordAddress = packet->Address();

    if (packet->Address() + Packet::GetPackedSize(ds) > End() - 10)      // ���� ����� ��� ������ ������� �� ������� �������. ��� ����� - � ����� ������� ������ �������� ������ 4 �����,
    {                                                                    // ���������� 0xFFFFFFFF �� ��������� ������ �������� �� ����� �� ������� �������.
        return nullptr;
    }

    Packet record = { STATE_VALID, static_cast<uint16>(Packet::GetPackedSize(ds)), TYPE_DATA };

    WriteToROM(&recordAddress, &record, sizeof(record));

    const_cast<DataSettings *>(ds)->numInROM = numInROM;

    uint8 *addressDataA = ds->dataA;   // �� ���� ������� �������� ������, ���������� ������. �������� �� ����� ���, ��� � ds ����� �������� ����� - �� ������� ������ ����� ��������� � ROM.
    uint8 *addressDataB = ds->dataB;   // �� ���� ������� �������� ������, ���������� ������. �������� �� ����� ���, ��� � ds ����� �������� ����� - �� ������� ������ ����� ��������� � ROM.

    TranslateAddressToROM(ds, packet);

    WriteToROM(&recordAddress, ds, sizeof(DataSettings));

    if (ds->enableA)
    {
        WriteToROM(&recordAddress, addressDataA, ds->SizeChannel());
    }

    if (ds->enableB)
    {
        WriteToROM(&recordAddress, addressDataB, ds->SizeChannel());
    }

    return packet;
}


const Packet *Sector::ReadData(int numInROM, DataSettings **ds) const
{
    const Packet *packet = FirstPacket();

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {
            DataSettings *settings;

            packet->UnPack(&settings);

            if(settings->numInROM == numInROM)
            {
                *ds = settings;

                return packet;
            }
        }

        packet = packet->Next();
    }

    return nullptr;
}


const Packet *Sector::FirstPacket() const
{
    return reinterpret_cast<const Packet *>(address);
}


const Packet *Sector::FirstFreePacket() const
{
    const Packet *packet = FirstPacket();

    while (packet)
    {
        if (packet->IsFree())
        {
            return packet;
        }
        packet = packet->Next();
    }

    return nullptr;
}


void Sector::GetDataInfo(bool existData[FlashMemory::Data::MAX_NUM_SAVED_WAVES]) const
{
    const Packet *packet = FirstPacket();

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {
            DataSettings *ds;
            if (packet->UnPack(&ds))
            {
                existData[ds->numInROM] = true;
            }
        }
        packet = packet->Next();
    }
}
