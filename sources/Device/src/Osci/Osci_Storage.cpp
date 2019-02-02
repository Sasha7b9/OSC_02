#include "defines.h"
#include "Osci/Osci_Storage.h"
#include "FPGA/FPGA.h"
#include "Data/Heap.h"
#include "Settings/Settings.h"
#include <cstring>


#include "Hardware/Timer.h"


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
    static uint8 *GetMemoryForData(uint /*size*/)
    {
        last = (Data *)Heap::Begin();
        return (uint8 *)last;
    }

    static uint8 *GetMemoryForDataP2P(uint /*size*/)
    {
        return (uint8 *)GetDataP2P();
    }

    static Data *GetData(int /* fromEnd */)
    {
        return (Data *)last;
    }

    static DataP2P *GetDataP2P()
    {
        return (DataP2P *)(((uint)Heap::End() + (uint)Heap::Begin()) / 2);
    }

private:
    /// ��������� �� ������ ���������� ������ (����� ������)
    static Data *first;
    /// ��������� �� ��������� ���������� ������ (����� ������)
    static Data *last;
};

Data *HeapWorker::first = nullptr;
Data *HeapWorker::last = nullptr;

};


/// ���������� ����������� ��� ������� �� Heap �����
static uint CalculateNeedMemory();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Data *Storage::PrepareForNewData()
{
    if (SET_DISABLED_BOTH)
    {
        return nullptr;
    }

    uint bytesForSave = CalculateNeedMemory();

    Data *data = (Data *)HeapWorker::GetMemoryForData(bytesForSave);

    data->Create();

    DataAccessor::FillNewData(data);

    return data;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::PrepareNewFrameP2P()
{
    uint bytesForSave = CalculateNeedMemory();

    DataP2P *data = (DataP2P *)HeapWorker::GetMemoryForDataP2P(bytesForSave);

    data->Create();

    DataAccessor::FillNewData(data);

    data->timeStart = TIME_MS;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint CalculateNeedMemory()
{
    uint bytesForSave = sizeof(Data);

    uint bytesInChannel = FPGA::BytesInChannel();

    if (SET_ENABLED_A)
    {
        bytesForSave += bytesInChannel;
    }
    if (SET_ENABLED_B)
    {
        bytesForSave += bytesInChannel;
    }

    return bytesForSave;
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

    pointer = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const DataSettings *Data::Settings()
{
    return &settings;
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
    data.dataA[pointer] = a.byte1;
    data.dataB[pointer] = b.byte1;

    readingPoints++;

    pointer++;

    if (pointer == data.settings.PointsInChannel())
    {
        pointer = 0;
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
void DataP2P::Logging() const
{
    LOG_WRITE("%d ����� ������� ��������� %d", readingPoints, pointer);
}
