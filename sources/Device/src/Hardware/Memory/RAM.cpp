#include "defines.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/StorageOsci.h"
#include "Utils/Math.h"
#include <cstring>


struct Packet;


#define BEGIN reinterpret_cast<uint>(Heap::Begin())
#define END   reinterpret_cast<uint>(Heap::End())


int16 RAM::currentSignal = 0;
/// ��������� �� ����� ������ ���������� �����. �� ����� ���� ������
Packet *oldest = reinterpret_cast<Packet *>(BEGIN);
/// ��������� �� ��������� ���������� �����. �� ����� ���� ���������
Packet *newest = nullptr;

/// ������� ����� ������ ������
static void RemoveOldest();
/// ���������� ����� ��� ������ ������ � ������� � ������������ � ds
static uint AllocateMemoryForPacket(const DataSettings *ds);
/// ���������� size ���� ������ � ������ ������
static void AllocateMemoryFromBegin(uint size);
/// ���������� �� ������ dest. ���������� ����� ������� ����� ����� ������
static uint *WriteToRAM(uint *dest, const void *src, uint size)
{
    std::memcpy(dest, src, size);
    
    return (dest + size);
}


struct Packet
{
    /// ����� ���������� ������, ����� "�������". ���� addrNext == 0x00000000, ���������� ������ ���, ���� ����� ����� �����
    uint addrNewest;

    /// ��������� ������ �� ������ this. ���������� ��������� �� �����, ��������� �� ���
    Packet *Pack(const DataSettings *ds)
    {
        DataSettings data = *ds;
        data.dataA = data.dataB = nullptr;

        addrNewest = 0x0000000;                                                                         // ���������, ��� ��� ����� ��������� �����

        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));                           // �� ����� ������ ������� DataSettings

        address = WriteToRAM(address, ds, sizeof(DataSettings));                                        // ���������� DataSettings

        if (ds->enableA)                                                                                // ���������� ������ ������� ������
        {
            data.dataA = reinterpret_cast<uint8 *>(address);
            address = WriteToRAM(address, ds->dataA, ds->SizeChannel());
        }

        if (ds->dataB)                                                                                  // ���������� ������ ������� ������
        {
            data.dataB = reinterpret_cast<uint8 *>(address);
            WriteToRAM(address, ds->dataB, ds->SizeChannel());
        }

        std::memcpy(reinterpret_cast<uint *>(Address() + sizeof(Packet)), &data, sizeof(DataSettings)); // ���������� ����������������� ���������

        return reinterpret_cast<Packet *>(addrNewest);
    }

    uint Address() const
    {
        return reinterpret_cast<uint>(this);
    };
    /// ���������� ������ ������, ����������� ��� �������� ������ � ����������� � ����������� ds
    static uint NeedMemoryForPacedData(const DataSettings *ds)
    {
        return sizeof(Packet) + sizeof(DataSettings) + ds->NeedMemoryForData();
    }

    uint Size() const
    {
        return sizeof(Packet) + sizeof(DataSettings) + GetDataSettings()->NeedMemoryForData();
    }

    DataSettings *GetDataSettings() const
    {
        return reinterpret_cast<DataSettings *>(Address() + sizeof(Packet));
    }
};


void RAM::Init()
{
    oldest = reinterpret_cast<Packet *>(BEGIN);
    newest = nullptr;
}


void RAM::Save(const DataSettings *ds)
{
    uint address = AllocateMemoryForPacket(ds);         // ������� ����� ��� ������ ������ ������

    newest->addrNewest = address;                       // ��������� ��� � �������� ������ ���������� ������ ��� �����������

    newest = reinterpret_cast<Packet *>(address);       // ������������� ���� ����� � �������� ��������� ������

    newest->Pack(ds);                                   // � ����������� ������
}


bool RAM::Read(DataSettings **, uint)
{
    return false;
}


uint RAM::NumberDatas()
{
    return 0;
}


static uint AllocateMemoryForPacket(const DataSettings *ds)
{
    if (newest == nullptr)                                                  // ��� ��� �� ����� ������
    {                                                                       
        return BEGIN;                                                       
    }                                                                       

    uint addrFirst = newest->Address() + newest->Size();                    // �� ����� ������ ������ ���� ��������� ������
    uint addrLast = addrFirst + Packet::NeedMemoryForPacedData(ds);         // � ��� ��������� ���� ��������� ������
                                                                            
    if (newest > oldest)                                                    // ���������� ������� ���������� - ����� ����� ������ ����������� ����� ����� ������
    {                                                                       
        if (addrLast < END)                                                 // ���� ������� ������ ��� ������
        {
            return addrFirst;
        }
        else                                                                // ���� ������� ������ �� ������� ��� ���������� ������
        {
            AllocateMemoryFromBegin(Packet::NeedMemoryForPacedData(ds));    // �� ����������� � ������ ������ ����������� ����� ���� ��� �������� ������

            return BEGIN;
        }
    }

    if (newest < oldest)                                                    // ����� ������ ������ ����������� ����� ����� �����
    {
        while (addrLast >= reinterpret_cast<uint>(oldest))
        {
            RemoveOldest();
        }
    }

    return addrFirst;
}


static void AllocateMemoryFromBegin(uint size)
{
    while (oldest->Address() - BEGIN < size)
    {
        RemoveOldest();
    }
}


static void RemoveOldest()
{
    oldest = reinterpret_cast<Packet *>(oldest->addrNewest);
}
