#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Reader.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Osci/Osci.h"
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

FrameP2P *frameP2P = nullptr;


void Reader::ReadDataFromRAM()
{
    IN_A = nullptr;
    IN_B = nullptr;
    DS = nullptr;

    DS = RAM::Get(ModeWork::IsRAM() ? static_cast<uint>(RAM::currentSignal) : 0U);

    if (DS)
    {
        if (DS->enableA)
        {
            IN_A = DS->dataA;
        }
        if (DS->enableB)
        {
            IN_B = DS->dataB;
        }

        FindTrigLevelIfNeed();
    }
}


void Reader::ReadDataFromROM()
{
    IN_A = nullptr;
    IN_B = nullptr;
    DS = nullptr;

    DS = ROM::Data::Read(NUM_ROM_SIGNAL);

    if (DS)
    {
        if (DS->enableA)
        {
            IN_A = DS->dataA;
        }
        if (DS->enableB)
        {
            IN_B = DS->dataB;
        }
    }
}


void Reader::FindTrigLevelIfNeed()
{
    if (TrigModeFind::IsAuto())
    {
        TrigLevel().Find();
    }
}
