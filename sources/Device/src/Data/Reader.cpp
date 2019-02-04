#include "defines.h"
#include "Data/Reader.h"
#include "Osci/Osci_Storage.h"
#include "Utils/CommonFunctions.h"
#include "Osci/Osci.h"


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
/// ��������� �� ����� ����������� ������
Osci::DataP2P *pDataP2P = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������ ����� ����������� ������
static bool ReadDataP2P();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Reader::ReadDataFromStorage()
{
    DATA_P2P = nullptr;
    DATA = nullptr;
    DS = nullptr;

    IN_A = IN_B = nullptr;

    DATA = Osci::Storage::GetData();

    if (DATA != nullptr)
    {
        DS = DATA->Settings();

        IN_A = DATA->DataA();
        IN_B = DATA->DataB();
    }

    if (Osci::InModeP2P())
    {
        return ReadDataP2P();
    }

    return (DATA != nullptr);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ReadDataP2P()
{
    if (Osci::InModeP2P())
    {
        DATA_P2P = nullptr;
        DS = nullptr;
        IN_A = IN_B = nullptr;

        DATA_P2P = Osci::Storage::GetFrameP2P();

        if (DATA_P2P)
        {
            DS = DATA_P2P->Settings();

            IN_A = DATA_P2P->DataA();
            IN_B = DATA_P2P->DataB();
        }
    }

    return (DATA_P2P != nullptr);
}
