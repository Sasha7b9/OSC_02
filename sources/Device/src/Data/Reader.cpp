#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Reader.h"
#include "Hardware/Memory/MemoryRAM.h"
#include "Osci/Osci.h"
#include "Osci/StorageOsci.h"
#include "Settings/Settings.h"


/// ��� ������������ CCM-������
static uint8 ccm[4][16 * 1024] __attribute__((section("CCM_DATA")));
/// ��� ��������� �� ��������� ������
const uint8 *dataIN[2] = { nullptr, nullptr };
/// ��� ��������� �� ������, ������������� � ������� � ������
uint8 *dataOUT[2] = { ccm[0], ccm[1] };
/// 
uint16 *ave[2] = { reinterpret_cast<uint16 *>(ccm[2]), reinterpret_cast<uint16 *>(ccm[3]) };
/// ��������� �� ��������� ��������� ������
const DataSettings *pDS = nullptr;
/// ��������� �� ��������� ������
DataOsci *pData = nullptr;
/// ��������� �� ����� ����������� ������
DataOsciP2P *pDataP2P = nullptr;


/// ����� ������ �������������, ���� ���������� �������������� ����� ������
static void FindTrigLevelIfNeed();



void Reader::ReadDataFromStorage()
{
    DATA_P2P = nullptr;
    DATA = nullptr;
    DS = nullptr;

    IN_A = IN_B = nullptr;

    DATA = StorageOsci::GetData((set.mem.modeWork == ModeWork::RAM) ? RAM::currentSignal : 0);

    if (DATA != nullptr)
    {
        DS = DATA->Settings();

        IN_A = DATA->DataA();
        IN_B = DATA->DataB();

        FindTrigLevelIfNeed();
    }

    if ((set.mem.modeWork == ModeWork::Dir) && Osci::InModeP2P())
    {
        ReadDataP2P();
    }
}


void Reader::ReadDataP2P()
{
    if (Osci::InModeP2P())
    {
        DATA_P2P = StorageOsci::GetFrameP2P();

        if (DATA_P2P)
        {
            if (DS)
            {
                if ((set.trig.startMode == TrigStartMode::Wait) && DS->Equals(*DATA_P2P->Settings()))
                {
                    DATA_P2P = nullptr;
                }
                else
                {
                    DS = DATA_P2P->Settings();
                }
            }
        }
    }
}


static void FindTrigLevelIfNeed()
{
    if (set.trig.modeFind == TrigModeFind::Auto)
    {
        TrigLevel::Find();
    }
}
