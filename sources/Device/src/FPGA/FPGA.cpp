#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include <cstring>


uint16 FPGA::valueADC = 0;
uint16 FPGA::post = static_cast<uint16>(~(512));
uint16 FPGA::pred = static_cast<uint16>(~(512));

uint8 dataRand[Chan::Count][ENumPointsFPGA::MAX_NUM];    ///< ����� ����� ������ �������������

bool          FPGA::isRunning = false;
uint          FPGA::timeStart = 0;
StateWorkFPGA FPGA::fpgaStateWork = StateWorkFPGA_Stop;

/// True, ���� ��� ������
bool givingStart = false;


void FPGA::GiveStart()
{
    uint8 value = static_cast<uint8>((static_cast<uint8>(set.trig.polarity)) % 2);

    uint8 stop = 0;
    if (Device::State::InModeRecorder())    // � ������ ������������
    {
        stop = (1 << BIT_TRIG_ENABLED);     // ������������� ������� ����, ��� ������� ������ ������ ����������
    }

    HAL_FSMC::WriteToFPGA8(WR::TRIG, static_cast<uint8>(value++ | stop));
    HAL_FSMC::WriteToFPGA8(WR::TRIG, static_cast<uint8>((value % 2) | stop));
}


uint16 FPGA::ReadLastRecord(Chan::E ch)
{
    static uint16 address = 0;

    if (Chan(ch).IsA())
    {
        address = static_cast<uint16>(HAL_FSMC::ReadFromFPGA(RD::LAST_RECORD_LO) + ((HAL_FSMC::ReadFromFPGA(RD::LAST_RECORD_HI)) << 8));
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
    std::memset(dataRand, 0, ENumPointsFPGA::MAX_NUM * 2 * sizeof(uint8));  // -V512
}


void FPGA::ReadData()
{
    DataSettings ds;
    ds.Fill();
    RAM::PrepareForNewData(&ds);

    if (ReadDataChanenl(Chan::A, ds.dataA, ds.SizeChannel()))
    {
        if (ReadDataChanenl(Chan::B, ds.dataB, ds.SizeChannel()))
        {
        }
        else
        {
            return;
        }
    }

    if (set.disp.ENumAverage != ENumAverage::_1)               // ���� �������� ����������
    {
        DataSettings *last = RAM::Read(0);
        DataSettings *prev = RAM::Read(1);

        if (prev && last)
        {
            if (last->Equals(*prev))
            {
                if (ENABLED_A(last))
                {
                    AveragerOsci::Process(Chan::A, last->dataA, last->SizeChannel());
                }
                if (ENABLED_B(last))
                {
                    AveragerOsci::Process(Chan::B, last->dataB, last->SizeChannel());
                }
            }
        }
    }
}
