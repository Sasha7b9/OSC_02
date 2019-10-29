#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Compressor.h"
#include "Data/DataSettings.h"


struct Packet
{
    uint  state;    // ��������� ������:
                    // 0xFFFFFFFF - � ����� ������ �� �������������
                    // 0x00000000 - ����� ����
                    // 0xFF00FF00 - ����������� �����
    uint16 size;    // ������ ������ - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // ��� ������ :
};


#define FREE    0xFFFFFFFFU
#define ERASED  0x00000000U
#define VALID   0xFF00FF00U

#define TYPE_DATA     0U


uint Compressor::GetPackedSize(const DataSettings *ds)
{
    return sizeof(Packet) +         // Packet
        sizeof(DataSettings) +      // DataSettings
        ds->NeedMemoryForData();    // data
}


static void WriteToROM(uint *address, const void *data, int size)
{
    HAL_EEPROM::WriteBufferBytes(*address, data, size);
    *address += size;
}


void Compressor::Pack(const DataSettings *ds, Address address)
{
    Packet packet = { VALID, static_cast<uint16>(GetPackedSize(ds)), TYPE_DATA };

    uint addr = address.addressMC;

    WriteToROM(&addr, &packet, sizeof(packet));

    WriteToROM(&addr, ds, sizeof(DataSettings));

    if (ds->enableA)
    {
        WriteToROM(&addr, ds->dataA, ds->SizeChannel());
    }

    if (ds->enableB)
    {
        WriteToROM(&addr, ds->dataB, ds->SizeChannel());
    }
}


bool Compressor::UnPack(Address address, DataSettings **ds)
{
    Packet packet = *reinterpret_cast<Packet *>(address.addressMC);

    if (packet.state != VALID || packet.type != TYPE_DATA)
    {
        return false;
    }

    **ds = *reinterpret_cast<DataSettings *>(address.addressMC + sizeof(Packet));

    return true;
}
