#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Memory/Sector.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include <cstdlib>


static void FillData(uint8 *dataA, uint8 *dataB, uint numPoints);
static bool CheckData(const uint8 *dataA, const uint8 *dataB, uint numPoints);
static void PrepareDS(DataSettings *ds);
/// ������ ������ � RAM ��� �������� "0"
static DataSettings *CreateDataInRAM(DataSettings *ds);


bool Test::RAM::Test()
{
    ::RAM::Init();

    Display::StartTest("���� RAM");

    int numRecord = 4096;

    for (int i = 1; i <= numRecord; i++)
    {
        static int line = -1;

        if (i % 32 == 0)
        {
            line = Display::AddMessage(String("������ %d �� %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), line);
        }

        DataSettings ds;

        PrepareDS(&ds);

        ::RAM::PrepareForNewData(&ds);

        FillData(ds.dataA, ds.dataB, ds.SizeChannel());

        DataSettings *read = ::RAM::Read(std::rand() % ::RAM::NumberDatas());

        if (!CheckData(read->dataA, read->dataB, read->SizeChannel()))
        {
            return false;
        }

        continue;
    }

    ::RAM::Init();

    return true;
}


bool Test::ROM::Data::Test()
{
    Display::StartTest("���� ROM");

    Display::AddMessage("������ ������");

    ::ROM::Data::EraseAll();

    int numRecord = 128;

    for (int i = 1; i <= numRecord; i++)
    {
        static int num = -1;

        num = Display::AddMessage(String("������ %d �� %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), num);

        uint numInROM = std::rand() % ::ROM::Data::MAX_NUM_SAVED_WAVES;

        DataSettings ds;

        ::ROM::Data::Save(numInROM, CreateDataInRAM(&ds));

        DataSettings *dsRead = nullptr;

        ::ROM::Data::Read(numInROM, &dsRead);

        if (!CheckData(dsRead->dataA, dsRead->dataB, dsRead->SizeChannel()))
        {
            return false;
        }

        continue;
    }
    
    Display::AddMessage("C����� ������");

    ::ROM::Data::EraseAll();

    return true;
}


static void FillData(uint8 *dataA, uint8 *dataB, uint numPoints)
{
    for (uint i = 0; i < numPoints; i++)
    {
        dataA[i] = static_cast<uint8>(i);
        dataB[i] = static_cast<uint8>(i);
    }
}


static bool CheckData(const uint8 *dataA, const uint8 *dataB, uint numPoints)
{
    for (uint i = 0; i < numPoints; i++)
    {
        if (dataA[i] != static_cast<uint8>(i) || dataB[i] != static_cast<uint8>(i))
        {
            return false;
        }
    }

    return true;
}


static void PrepareDS(DataSettings *ds)
{
    ds->Fill();
    ds->enableA = ds->enableB = true;
    ds->peackDet = static_cast<uint>(PeakDetMode::Disabled);
    ds->enumPoints = static_cast<uint>(std::rand() % ENumPointsFPGA::Count);
}


static DataSettings *CreateDataInRAM(DataSettings *ds)
{
    PrepareDS(ds);

    ::RAM::PrepareForNewData(ds);
    
    FillData(ds->dataA, ds->dataB, ds->SizeChannel());

    return ds;
}
