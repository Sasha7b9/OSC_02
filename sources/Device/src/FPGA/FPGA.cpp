#include "defines.h"
#include "device.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Osci/StorageOsci.h"
#include "Data/Reader.h"
#include <cstring>
#include <stdlib.h>


using namespace Address;


uint16 FPGA::valueADC = 0;
uint16 FPGA::post = (uint16)~(512);
uint16 FPGA::pred = (uint16)~(512);

uint8 dataRand[Chan::Size][FPGA::MAX_NUM_POINTS];    ///< ����� ����� ������ �������������

bool          FPGA::isRunning = false;
uint          FPGA::timeStart = 0;
StateWorkFPGA FPGA::fpgaStateWork = StateWorkFPGA_Stop;

/// True, ���� ��� ������
bool givingStart = false;


void FPGA::GiveStart()
{
    uint8 value = (uint8)(((uint8)set.trig.polarity) % 2);

    uint8 stop = 0;
    if (Device::State::InModeRecorder())    // � ������ ������������
    {
        stop = (1 << BIT_TRIG_ENABLED);     // ������������� ������� ����, ��� ������� ������ ������ ����������
    }

    HAL_FSMC::WriteToFPGA8(WR::TRIG, (uint8)(value++ | stop));
    HAL_FSMC::WriteToFPGA8(WR::TRIG, (uint8)((value % 2) | stop));
}


uint16 FPGA::ReadLastRecord(Chan::E ch)
{
    static uint16 address = 0;

    if (Chan(ch).IsA())
    {
        address = (uint16)(HAL_FSMC::ReadFromFPGA(RD::LAST_RECORD_LO) + ((HAL_FSMC::ReadFromFPGA(RD::LAST_RECORD_HI)) << 8));
    }

    return address;
}


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


bool FPGA::ForTester::Start() // -V2506
{
    // � ��� ���������� �������. �� ���������� ������� ������ ����������� �� ����� 2.5 ��
    // 2.5�� / 12��� = 0.2 ��/��� = 10���/���

    set.time.base = TBase::_500us;

    TBase::Load();
    
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, (uint16)(~(400)));
    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, (uint16)(~(1)));
    HAL_FSMC::WriteToFPGA8(WR::START, 0xff);

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


bool FPGA::ForTester::Read(uint16 *dataA, uint8 *dataB) // -V2506
{
    uint start = TIME_MS;
    FPGA::flag = 0;
    while (!GetFlag::DATA_READY())    // ��� ����� ���������� ������
    {
        FPGA::ReadFlag();

        if(TIME_MS - start > 20)        /// \todo ��������� �������. ���� ������� ���, ����� ����� �������� ���� ���������. ���� ����������, �� �����
        {                               /// ������� ������
            return false;
        }
    }

    uint16 aRead = (uint16)(ReadLastRecord(Chan::A) - TESTER_NUM_POINTS);

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // ��������� �����, � �������� ����� ������ ������
    HAL_FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // � ��� ������� ����, ����� ������ ���������� � ����

    uint8 *addrA = RD::DATA_A; // -V566
    addrA++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataA++ = *addrA;
    }

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // ��������� �����, � ��������� ����� ������ ������
    HAL_FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // � ��� ������� ����, ����� ������ ���������� � ����

    uint8 *addrB = RD::DATA_B; // -V566
    addrB++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataB++ = *addrB;
    }

    return true;
}


void FPGA::Reset()
{
    TShift::Load();

    FPGA::LoadRegUPR();
}


uint FPGA::BytesInChannel()
{
    ENumPointsFPGA points(set.mem.enumPoints);
    return points.BytesInChannel(set.time.peakDet);
}


void FPGA::SetValueADC(uint16 value)
{
    valueADC = value;
}


bool FPGA::IsRunning()
{
    return isRunning;
}


void FPGA::ClearDataRand()
{
    std::memset(dataRand, 0, FPGA::MAX_NUM_POINTS * 2 * sizeof(uint8));  // -V512
}


void FPGA::ReadData()
{
    DataOsci *data = StorageOsci::PrepareForNewData();

    if (ReadDataChanenl(Chan::A, data->dataA))
    {
        if (ReadDataChanenl(Chan::B, data->dataB))
        {
        }
        else
        {
            return;
        }
    }

    if (set.disp.ENumAverage != ENumAverage::_1)               // ���� �������� ����������
    {
        DataOsci *last = StorageOsci::GetData(0);
        DataOsci *prev = StorageOsci::GetData(1);

        if (prev && last)
        {
            const DataSettings *setLast = last->Settings();
            const DataSettings *setPrev = prev->Settings();

            if (setLast->Equals(*setPrev))
            {
                if (ENABLED_A(setLast))
                {
                    AveragerOsci::Process(Chan::A, last->DataA(), setLast->SizeChannel());
                }
                if (ENABLED_B(setLast))
                {
                    AveragerOsci::Process(Chan::B, last->DataB(), setLast->SizeChannel());
                }
            }
        }
    }
}
