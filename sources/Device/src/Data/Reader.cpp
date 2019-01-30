#include "defines.h"
#include "Data/Reader.h"
#include "Osci/Osci_Storage.h"
#include "Utils/CommonFunctions.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ��� ������������ CCM-������
static uint8 ccm[4][16 * 1024] __attribute__((section("CCM_DATA")));


/// ��� ��������� �� ��������� ������
const uint8 *dataIN[2] = { nullptr, nullptr };
/// ��� ��������� �� ������, ������������� � ������� � ������
uint8 *dataOUT[2] = { ccm[0], ccm[1] };
/// ��������� �� ��������� ��������� ������
const DataSettings *pDS = nullptr;
/// ��������� �� ��������� ������
Osci::Data *pData = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Osci::Data *Reader::ReadDataFromStorage()
{
    pData = nullptr;
    pDS = nullptr;

    dataIN[0] = dataIN[1] = nullptr;

    pData = Osci::Storage::GetData();

    if (pData != nullptr)
    {
        pDS = pData->Settings();

        dataIN[0] = pData->DataA();
        dataIN[1] = pData->DataB();
    }

    //LOG_WRITE("%x %x %x %x", pData, pDS, dataIN[0], dataIN[1]);
    //
    //CF::LogBufferU8_DEC(dataIN[0], 10);

    return pData;
}
