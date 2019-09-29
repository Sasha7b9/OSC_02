#include "defines.h"
#include "device.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "FPGA/FPGA_Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include <cstring>
#include <stdlib.h>

#include "Recorder/Recorder.h"
#include "Osci/Osci_Storage.h"
#include "Data/Reader.h"
#include "Osci/Osci_Averager.h"


using namespace HAL::ADDRESSES::FPGA;
using namespace FPGA::HAL;
using namespace FPGA::Settings;
using namespace Osci::Settings;

using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16 FPGA::valueADC = 0;

uint16 FPGA::post = (uint16)~(512);
uint16 FPGA::pred = (uint16)~(512);

uint8 dataRand[Chan::Size][FPGA::MAX_NUM_POINTS];    ///< ����� ����� ������ �������������

bool          FPGA::isRunning = false;
uint          FPGA::timeStart = 0;
StateWorkFPGA FPGA::fpgaStateWork = StateWorkFPGA_Stop;

/// True, ���� ��� ������
bool givingStart = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class DataAccessor
    {
    public:
        static uint8 *DataA(Osci::Data *data)
        {
            return data->dataA;
        }
        static uint8 *DataB(Osci::Data *data)
        {
            return data->dataB;
        }
    };
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::GiveStart()
{
    uint8 value = (uint8)(((uint8)set.trig.polarity) % 2);

    uint8 stop = 0;
    if (Device::State::InModeRecorder())    // � ������ ������������
    {
        stop = (1 << BIT_TRIG_ENABLED);     // ������������� ������� ����, ��� ������� ������ ������ ����������
    }

    FSMC::WriteToFPGA8(WR::TRIG, (uint8)(value++ | stop));
    FSMC::WriteToFPGA8(WR::TRIG, (uint8)((value % 2) | stop));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FPGA::ReadLastRecord(Chan::E ch)
{
    static uint16 address = 0;

    if (Chan(ch).IsA())
    {
        address = (uint16)(FSMC::ReadFromFPGA(RD::LAST_RECORD_LO) + ((FSMC::ReadFromFPGA(RD::LAST_RECORD_HI)) << 8));
    }

    return address;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::OnPressStart()
{
    if (Device::State::InModeRecorder())
    {
        Recorder::OnPressStart();
    }
    else
    {
        if (IsRunning())
        {
            Osci::Stop();
        }
        else
        {
            Osci::Start();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::ForTester::Start() // -V2506
{
    // � ��� ���������� �������. �� ���������� ������� ������ ����������� �� ����� 2.5 ��
    // 2.5�� / 12��� = 0.2 ��/��� = 10���/���

    SET_TBASE = TBase::_500us;

    TBase::Load();
    
    FSMC::WriteToFPGA16(WR::POST_LO, (uint16)(~(400)));
    FSMC::WriteToFPGA16(WR::PRED_LO, (uint16)(~(1)));
    FSMC::WriteToFPGA8(WR::START, 0xff);

    GiveStart();

    return true;

//    uint start = TIME_US;
//    HAL::ReadFlag();
//
//    while (!HAL::GetFlag::PRED())
//    {
//        HAL::ReadFlag();
//        if(TIME_US - start > 250)     /// \todo ��������� �������. ���� ������� ���, ����� ����� �������� ���� ���������. ���� ����������, �� ����� ������� ������
//        {
//            return false;
//        }
//    }
//
//    GiveStart();
//
//    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::ForTester::Read(uint16 *dataA, uint8 *dataB) // -V2506
{
    uint start = TIME_MS;
    HAL::flag = 0;
    while (!HAL::GetFlag::DATA_READY())    // ��� ����� ���������� ������
    {
        HAL::ReadFlag();

        if(TIME_MS - start > 20)        /// \todo ��������� �������. ���� ������� ���, ����� ����� �������� ���� ���������. ���� ����������, �� �����
        {                               /// ������� ������
            return false;
        }
    }

    uint16 aRead = (uint16)(ReadLastRecord(Chan::A) - TESTER_NUM_POINTS);

    FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // ��������� �����, � �������� ����� ������ ������
    FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // � ��� ������� ����, ����� ������ ���������� � ����

    uint8 *addrA = RD::DATA_A; // -V566
    addrA++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataA++ = *addrA;
    }

    FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // ��������� �����, � ��������� ����� ������ ������
    FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // � ��� ������� ����, ����� ������ ���������� � ����

    uint8 *addrB = RD::DATA_B; // -V566
    addrB++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataB++ = *addrB;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Reset()
{
    TShift::Load();

    HAL::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::BytesInChannel()
{
    Osci::Settings::Memory::ENumPointsFPGA points(set.mem.enumPoints);
    return points.BytesInChannel(SET_PEAKDET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetValueADC(uint16 value)
{
    valueADC = value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::IsRunning()
{
    return isRunning;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ClearDataRand()
{
    std::memset(dataRand, 0, FPGA::MAX_NUM_POINTS * 2 * sizeof(uint8));  // -V512
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadData()
{
    Osci::Data *data = Osci::Storage::PrepareForNewData();

    if (ReadDataChanenl(Chan::A, DataAccessor::DataA(data)))
    {
        if (ReadDataChanenl(Chan::B, DataAccessor::DataB(data)))
        {
        }
        else
        {
            return;
        }
    }

    if (set.disp.ENumAverage != ENumAverage::_1)               // ���� �������� ����������
    {
        Osci::Data *last = Osci::Storage::GetData(0);
        Osci::Data *prev = Osci::Storage::GetData(1);

        if (prev && last)
        {
            const DataSettings *setLast = last->Settings();
            const DataSettings *setPrev = prev->Settings();

            if (setLast->Equals(*setPrev))
            {
                if (ENABLED_A(setLast))
                {
                    Osci::Averager::Process(Chan::A, last->DataA(), setLast->SizeChannel());
                }
                if (ENABLED_B(setLast))
                {
                    Osci::Averager::Process(Chan::B, last->DataB(), setLast->SizeChannel());
                }
            }
        }
    }
}
