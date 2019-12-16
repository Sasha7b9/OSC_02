#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
#include "Utils/Math.h"
#include <cstring>
#include <cstdlib>


#define BEGIN reinterpret_cast<uint>(Heap::Begin())
#define END   reinterpret_cast<uint>(Heap::End())


int16 RAM::currentSignal = 0;
Packet *RAM::oldest = reinterpret_cast<Packet *>(BEGIN);
Packet *RAM::newest = nullptr;


/// ���������� �� ������ dest. ���������� ����� ������� ����� ����� ������
static uint *WriteToRAM(uint *dest, const void *src, uint size)
{
    uint8 *address = reinterpret_cast<uint8 *>(dest);

    std::memcpy(address, src, size);
    
    return reinterpret_cast<uint *>(address + size);
}


struct Packet
{
    /// ����� ���������� ������, ����� "�������". ���� addrNext == 0x00000000, ���������� ������ ���, ���� ����� ����� �����
    uint addrNewest;

    /// ��������� ������ �� ������ this. ���������� ��������� �� �����, ��������� �� ���
    void Pack(const DataSettings *ds)
    {
        DataSettings data = *ds;
        data.dataA = nullptr;
        data.dataB = nullptr;

        addrNewest = 0x0000000;                                                                         // ���������, ��� ��� ����� ��������� �����

        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));                           // �� ����� ������ ������� DataSettings

        address = WriteToRAM(address, ds, sizeof(DataSettings));                                        // ���������� DataSettings

        if (ds->enableA)                                                                                // ���������� ������ ������� ������
        {
            data.dataA = reinterpret_cast<uint8 *>(address);
            address = WriteToRAM(address, ds->dataA, ds->BytesInChannel());
        }

        if (ds->dataB)                                                                                  // ���������� ������ ������� ������
        {
            data.dataB = reinterpret_cast<uint8 *>(address);
            WriteToRAM(address, ds->dataB, ds->BytesInChannel());
        }

        std::memcpy(reinterpret_cast<uint *>(Address() + sizeof(Packet)), &data, sizeof(DataSettings)); // ���������� ����������������� ���������
    }
    /// ����������� ����� ��� ���������� ������ � ������������ � ����������� ds
    void Prepare(DataSettings *ds)
    {
        uint bytesInChannel = ds->BytesInChannel();

        addrNewest = 0x00000000;
        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));

        ds->dataA = nullptr;
        ds->dataB = nullptr;
        uint8 *addrData = reinterpret_cast<uint8 *>(reinterpret_cast<uint8 *>(address) + sizeof(DataSettings));

        if (ds->enableA)
        {
            ds->dataA = addrData;
            std::memset(addrData, VALUE::NONE, bytesInChannel);
            addrData += bytesInChannel;
        }

        if (ds->enableB)
        {
            ds->dataB = addrData;
            std::memset(addrData, VALUE::NONE, bytesInChannel);
        }

        WriteToRAM(address, ds, sizeof(DataSettings));
    }

    uint Address() const
    {
        return reinterpret_cast<uint>(this);
    };
    /// ���������� ����� ������� ���������� �� ������� �����
    uint End() const
    {
        return Address() + Size();
    }
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
        uint address = Address() + sizeof(Packet);

        return reinterpret_cast<DataSettings *>(address);
    }
};


void RAM::Init()
{
    oldest = reinterpret_cast<Packet *>(BEGIN);
    newest = nullptr;
}


void RAM::PrepareForNewData(DataSettings *ds)
{
    ds->Fill();

    ds->id = NumberDatas() ? Get()->id + 1 : 0;

    uint address = AllocateMemoryForPacket(ds);         // ������� ����� ��� ������ ������ ������

    if (newest)
    {
        newest->addrNewest = address;                   // ��������� ��� � �������� ������ ���������� ������ ��� �����������
    }

    newest = reinterpret_cast<Packet *>(address);       // ������������� ���� ����� � �������� ��������� ������

    newest->Prepare(ds);                                   // � ����������� ������
}


DataSettings *RAM::Get(uint numFromEnd)
{
    uint number = NumberDatas();
    
    if (numFromEnd + 1 > number)
    {
        return nullptr;
    }

    uint counter = number - 1 - numFromEnd;

    Packet *packet = oldest;

    while (counter > 0)
    {
        packet = reinterpret_cast<Packet *>(packet->addrNewest);
        counter--;
    }

    return packet->GetDataSettings();
}


uint RAM::NumberDatas()
{
    if (newest == nullptr)
    {
        return 0;
    }

    if (oldest == nullptr)
    {
        return 1;
    }

    uint result = 0;

    Packet *packet = oldest;

    while (packet != nullptr)
    {
        result++;
        packet = reinterpret_cast<Packet *>(packet->addrNewest);
    }

    return result;
}

uint RAM::AllocateMemoryForPacket(const DataSettings *ds)
{
    if (newest == nullptr)                                                  // ��� ��� �� ����� ������
    {
        return BEGIN;
    }

    uint addrFirst = newest->End();                                         // �� ����� ������ ������ ���� ��������� ������
    uint addrLast = addrFirst + Packet::NeedMemoryForPacedData(ds);         // � ��� ��������� ���� ��������� ������

    if (addrLast > END)
    {
        addrLast = BEGIN + Packet::NeedMemoryForPacedData(ds);
        while (reinterpret_cast<uint>(oldest) < addrLast)
        {
            RemoveOldest();
        }
        return BEGIN;
    }
                                                                            
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
            if (oldest->Address() == BEGIN)
            {
                break;
            }
        }
    }

    return addrFirst;
}


void RAM::AllocateMemoryFromBegin(uint size)
{
    while (oldest->Address() - BEGIN < size)
    {
        RemoveOldest();
    }
}


void RAM::RemoveOldest()
{
    oldest = reinterpret_cast<Packet *>(oldest->addrNewest);
}
