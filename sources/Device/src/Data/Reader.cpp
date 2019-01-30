#include "defines.h"
#include "Data/Reader.h"
#include "Osci/Osci_Storage.h"


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

    return pData;
}
