#include "defines.h"
#include "Osci/Osci_Storage.h"
#include "FPGA/FPGA.h"
#include "Data/Heap.h"
#include "Settings/Settings.h"
#include <cstring>


#include "Hardware/Timer.h"
#include "Utils/Buffer.h"


/*
    ������ � Heap ����� ��������� ���������������. ������ ��������� ������ �������� �� �����������. ����� ����������� ����� ������, ��������� ������ � ������ � ��������� ������� �� �� �����.
    ������ �������� ���:
    settings channel1 channel2
    ��� ���� ������ ������ �� ������� ����� ������������� � � ���� ������, ���� ����������� ������ ������ 1, �� ����������� ���� ����� ������ ������ 2.
*/


using namespace Osci;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{

uint Data::allDatas = 0;

class DataAccessor
{
public:
    /// ��������� ����� ���������� ������ ������� ����������
    static void FillNewData(Data *data)
    {
        uint bytesInChannel = FPGA::BytesInChannel();

        data->settings.Fill();

        uint8 *addrData = (uint8 *)data + sizeof(Data);     // ����� ������ ������

        if (SET_ENABLED_A)
        {
            data->dataA = addrData;
        }
        if (SET_ENABLED_B)
        {
            data->dataB = addrData;
            if (SET_ENABLED_A)
            {
                data->dataB += bytesInChannel;
            }
        }
    }

    /// ��������� Data ����������
    static void FillNewData(DataP2P *data)
    {
        FillNewData(&data->data);
    }

private:
    /// �������� �������� ���������  �� shift, ���� �� �� ����� ����
    static void ShiftPointerIfNotZero(uint8 **pointer, int shift)
    {
        if (*pointer)
        {
            *pointer += shift;
        }
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ����� ��� ������ � Heap
class HeapWorker
{
public:

    /// ���������� ��������� �� ������ ����������� size, ���� ������� ������� � �������
    static uint8 *Allocate(uint size)
    {
        if (oldest == nullptr)
        {
            return BeginMemory();
        }

        uint freeMemory = 0;

        if (newest >= oldest)           // ������ ����������� ��������� - �������� �� ������� �������
        {
            freeMemory = EndMemory() - (uint8 *)newest - newest->FullSize();
        }
        else
        {
            freeMemory = (uint8 *)oldest - (uint8 *)newest - newest->FullSize();
        }

        return (freeMemory >= size) ? ((uint8 *)newest + newest->FullSize()) : nullptr;
    }
    /// �������� ������������ ������
    static void FreeOldest()
    {
        oldest = oldest->next;
        oldest->prev = nullptr;
    }

    /// ���������� �����, ��������� �� ��������� ��������� ��� ���������� ������
    static uint8 *EndMemory()
    {
        return (uint8 *)GetMemoryForDataP2P();
    }

    /// ���������� ������ ��������� ����� ��� �������� ������
    static uint8 *BeginMemory()
    {
        return (uint8 *)Heap::Begin();
    }

    /// �������� ����� ��� ������ ������, ����� ������� ������ ��� �������� ������ � ����������� DataSettings
    static Data *GetMemoryForData(const DataSettings *ds)
    {
        if (oldest == nullptr)
        {
            newest = oldest = (Data *)BeginMemory();
            newest->prev = newest->next = oldest->next = oldest->prev = nullptr;
            return oldest;
        }

        uint needMemory = sizeof(Data) + ds->NeedMemoryForData();       // ��������� ��������� ���������� ������

        uint8 *address = Allocate(needMemory);

        while (address == nullptr)
        {
            FreeOldest();                           // ������� ������ ������ �� ��� ���, 
            address = Allocate(needMemory);         // ���� �� ����������� ���������� ����� ��� �������� �����
        }

        Data *data = (Data *)address;

        newest->next = data;
        newest = data;

        return newest;
    }

    static Data *GetData(int fromEnd)
    {
        Data *data = newest;

        while (fromEnd > 0 && data != nullptr)
        {
            data = newest->prev;
            fromEnd--;
        }

        return data;
    }

    static DataP2P *GetMemoryForDataP2P()
    {
        return GetDataP2P();
    }

    static DataP2P *GetDataP2P()
    {
        DataSettings ds;
        ds.Fill();
        uint size = sizeof(DataP2P) + ds.SizeChannel() * 2;

        return (DataP2P *)(((uint)Heap::End() - size));
    }

    static void Reset()
    {
        oldest = newest = nullptr;
    }

private:
    /// ��������� �� ������ ���������� ������ (����� ������)
    static Data *oldest;
    /// ��������� �� ��������� ���������� ������ (����� ������)
    static Data *newest;
};

Data *HeapWorker::oldest = nullptr;
Data *HeapWorker::newest = nullptr;

};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Data *Storage::PrepareForNewData()
{
    if (SET_DISABLED_BOTH)
    {
        return nullptr;
    }

    DataSettings ds;
    ds.Fill();

    Data *data = (Data *)HeapWorker::GetMemoryForData(&ds);

    data->Create();

    DataAccessor::FillNewData(data);

    return data;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::PrepareNewFrameP2P()
{
    DataP2P *data = HeapWorker::GetMemoryForDataP2P();

    data->Create();

    DataAccessor::FillNewData(data);

    data->timeStart = TIME_MS;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Data *Storage::GetData(int fromEnd)
{
    return HeapWorker::GetData(fromEnd);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DataP2P *Storage::GetFrameP2P()
{
    return HeapWorker::GetDataP2P();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Data::Create()
{
    num = allDatas++;
    dataA = dataB = nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::Create()
{
    data.Create();
    data.allDatas--;
    
    readingPoints = 0;

    pointerToByte = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const DataSettings *Data::Settings()
{
    return &settings;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const DataSettings *DataP2P::Settings()
{
    return &data.settings;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *Data::DataA()
{
    return dataA;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *Data::DataB()
{
    return dataB;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Data::FullSize() const
{
    uint result = sizeof(Data);

    if (ENABLED_A(&settings))
    {
        result += settings.SizeChannel();
    }
    if (ENABLED_B(&settings))
    {
        result += settings.SizeChannel();
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *Data::GetData(Chan::E ch)
{
    return (ch == Chan::A) ? dataA : dataB;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *DataP2P::DataA()
{
    return data.dataA;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *DataP2P::DataB()
{
    return data.dataB;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float DataP2P::TimePointMS(uint numPoint) const
{
    static const float timePoint[TBase::Size] =
    {
        /* 2 ��    */ 0,
        /* 5 ��    */ 0,
        /* 10 ��   */ 0,
        /* 20 ��   */ 0,
        /* 50 ��   */ 0,
        /* 100 ��  */ 0,
        /* 200 ��  */ 0,
        /* 500 ��  */ 0,
        /* 1 ���   */ 0,
        /* 2 ���   */ 0,
        /* 5 ���   */ 0,
        /* 10 ���  */ 0,
        /* 20 ���  */ 0,
        /* 50 ���  */ 0,
        /* 100 ��� */ 0.005F,
        /* 200 ��� */ 1.0e-2F,
        /* 500 ��� */ 2.0e-25F,
        /* 1 ��    */ 5.0e-2F,
        /* 2 ��    */ 1.0e-2F,
        /* 5 ��    */ 2.5e-1F,
        /* 10 ��   */ 5.0e-1F,
        /* 20 ��   */ 1.0e+0F,
        /* 50 ��   */ 2.5e+0F,         
        /* 100 ��  */ 5.0e+0F,         
        /* 200 ��  */ 1.0e+1F,
        /* 500 ��  */ 2.5e+1F, 
        /* 1 �     */ 5.0e+1F,
        /* 2 �     */ 1.0e+2F,
        /* 5 �     */ 2.5e+2F,
        /* 10 �    */ 5.0e+2F
    };
    
    float k = timePoint[TBASE(&data.settings)] * numPoint;
    
    return timeStart + k;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::AddPoints(uint timeMS, BitSet16 a, BitSet16 b)
{
    if (SET_PEAKDET_EN)
    {
        data.dataA[pointerToByte] = a.byte0;
        data.dataB[pointerToByte] = b.byte0;

        pointerToByte++;
    }
    data.dataA[pointerToByte] = a.byte1;
    data.dataB[pointerToByte] = b.byte1;

    readingPoints++;

    pointerToByte++;

    if (pointerToByte == data.settings.SizeChannel())
    {
        pointerToByte = 0;
    }

    if (NeedAdditionPoints(timeMS))
    {
        AddPoints(timeMS, a, b);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DataP2P::NeedAdditionPoints(uint timeMS) const
{
    float timePoint = TimePointMS(readingPoints);

    return (timePoint < timeMS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::FillBufferForDraw(Chan::E ch, Buffer *buffer)
{
    if (PEAKDET_DISABLED(&data.settings))
    {
        FillBufferForPeakDetDisabled(ch, buffer);
    }
    else
    {
        FillBufferForPeakDetEnabled(ch, buffer);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::FillBufferForPeakDetDisabled(Chan::E ch, Buffer *buffer)
{
    static const uint NUM_BYTES = 281;

    PrepareBuffer(buffer, NUM_BYTES);

    if (readingPoints <= NUM_BYTES)
    {
        std::memcpy(buffer->data, data.GetData(ch), readingPoints);
    }
    else
    {
        for (int i = 0; i < NUM_BYTES; i++)
        {
            buffer->data[NUM_BYTES - i] = ByteFromEnd(ch, i);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::FillBufferForPeakDetEnabled(Chan::E ch, Buffer *buffer)
{
    static const uint NUM_BYTES = 281 * 2;
    uint readingBytes = (uint)ReadingBytes();

    PrepareBuffer(buffer, NUM_BYTES);

    if (readingBytes <= NUM_BYTES)
    {
        std::memcpy(buffer->data, data.GetData(ch), readingBytes);
    }
    else
    {
        for (int i = 0; i < NUM_BYTES; i++)
        {
            buffer->data[NUM_BYTES - i] = ByteFromEnd(ch, i);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::PrepareBuffer(Buffer *buffer, uint size)
{
    buffer->Realloc(size);
    std::memset(buffer->data, FPGA::VALUE::NONE, size);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 DataP2P::ByteFromEnd(Chan::E ch, int fromEnd)
{
    if (fromEnd > data.settings.SizeChannel())      // ���� ��������� ��������, ������� ��� �������� ���������
    {
        return FPGA::VALUE::NONE;
    }

    int index = pointerToByte;        // index ����� ��������� �� ������� ������������� ��������

    while (fromEnd != 0)
    {
        fromEnd--;
        index--;
        if (index < 0)
        {
            index = data.settings.SizeChannel() - 1;
        }
    }

    return data.GetData(ch)[index];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint DataP2P::ReadingBytes() const
{
    if (PEAKDET_DISABLED(&data.settings))
    {
        return readingPoints;
    }

    return readingPoints * 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Storage::NumElementsInStorage()
{
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Storage::Clear()
{
    HeapWorker::Reset();
}
