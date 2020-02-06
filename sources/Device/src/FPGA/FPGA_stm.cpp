#include "defines.h"
#include "log.h"
#include "FPGA/AD9286.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include <cstring>


extern uint8 dataRand[Chan::Count][ENumPointsFPGA::MAX_NUM];

uint16 FPGA::addrRead = 0xffff;

volatile static int numberMeasuresForGates = 10000;



void FPGA::Init()
{
    forcedStart = false;

    Osci::InputController::Init();

    HAL_PIO::Set(PIN_SPI3_CS1);
    HAL_PIO::Set(PIN_SPI3_CS2);

    HAL_PIO::Reset(PIN_SPI3_SCK);
    HAL_PIO::Reset(PIN_SPI3_DAT);

    AD9286::Init();

    HAL_ADC3::Init();
}


static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{
    static float minGate = 0.0F;
    static float maxGate = 0.0F;

    if (rand < 500 || rand > 4000)
    {
        return false;
    }

    static int numElements = 0;

    static Min2 min;
    static Max2 max;

    numElements++;

    bool retValue = true;

    min.Add(rand);
    max.Add(rand);

    if (minGate == 0.0F)    // -V550 //-V2550 //-V550
    {
        *eMin = min.Get();
        *eMax = max.Get();
        if (numElements < numberMeasuresForGates)
        {
            return true;
        }
        minGate = min.Get();
        maxGate = max.Get();
        numElements = 0;
        min.Reset();
        max.Reset();
    }

    if (numElements >= numberMeasuresForGates)
    {
        minGate = 0.8F * minGate + min.Get() * 0.2F;
        maxGate = 0.8F * maxGate + max.Get() * 0.2F;

        numElements = 0;
        min.Reset();
        max.Reset();

        LOG_WRITE("����� ������ %d %d", static_cast<uint16>(minGate), static_cast<uint16>(maxGate));
    }

    *eMin = static_cast<uint16>(minGate);   // -V519 // -V2004
    *eMax = static_cast<uint16>(maxGate);   // -V519 // -V2004

    if (rand < *eMin || rand > *eMax)
    {
        return false;
    }

    return retValue;
}


int FPGA::CalculateShift()
{
    uint16 min = 0;
    uint16 max = 0;

    if (!CalculateGate(valueADC, &min, &max))
    {
        return NULL_TSHIFT;
    }

    int deltaMAX = set.dbg.nrst.enum_gate_max * 10;
    int deltaMIN = set.dbg.nrst.enum_gate_min * 10;

    if (FPGA::valueADC > max - deltaMAX || valueADC < min + deltaMIN)
    {
        return NULL_TSHIFT;
    }

    if (Osci::InModeRandomizer())
    {
        float tin = static_cast<float>(valueADC - min + deltaMIN) / (max - deltaMAX - (min + deltaMIN));
        int retValue = static_cast<int>(tin * TBase().RandK());

        return retValue;
    }

    return NULL_TSHIFT;
}


bool FPGA::ReadDataChannelRand(Chan::E ch, uint8 *addr, uint8 *data)
{
    static uint startFrame = 0;
    static uint timeForFrame = 0;

    uint start = Timer::TimeUS();

    int Tsm = CalculateShift();

    if (Tsm == NULL_TSHIFT)
    {
        std::memcpy(data, &dataRand[ch][0], ENumPointsFPGA::PointsInChannel());
        return false;
    }

    Osci::StructReadRand infoRead = Osci::GetInfoForReadRand(Tsm, addr);

    uint step = infoRead.step;

    uint8 *dataRead = &dataRand[ch][infoRead.posFirst];

    uint8 *last = &dataRand[ch][ENumPointsFPGA::PointsInChannel()];

    HAL_BUS::FPGA::SetAddrData(addr);

    uint p1 = 0xFFFFFFFF, p2 = 0xFFFFFFFF;

    if (ENumAverage() > 1)
    {
        uint8 *dataPointer = &data[infoRead.posFirst];              // ��������� � ���������� �������

        while (dataRead < last)
        {
            *dataRead = HAL_BUS::FPGA::ReadA0();
            *dataPointer = *dataRead;

            dataRead += step;
            dataPointer += step;
        }
    }
    else
    {
        Debug::StartProfiling();

        while (dataRead < last)
        {
            *dataRead = HAL_BUS::FPGA::ReadA0();
            dataRead += step;
        }

        p1 = Debug::PointProfiling();

        std::memcpy(data, &dataRand[ch][0], ENumPointsFPGA::PointsInChannel());

        p2 = Debug::PointProfiling();
    }

    timeForFrame += Timer::TimeUS() - start;

    if(Timer::TimeMS() >= startFrame + 1000)
    {
        LOG_WRITE("����� ������ �� ������� - %d mc", timeForFrame / 1000);
        timeForFrame = 0;
        startFrame = Timer::TimeMS();
        LOG_WRITE(" ");
        LOG_WRITE("%d %d", p1, p2);
    }

    return true;
}


bool FPGA::ReadDataChannel(Chan::E ch, uint8 *data)
{
    uint numPoints = ENumPointsFPGA::PointsInChannel();

    if (addrRead == 0xffff)
    {
        uint k = 1;

        if (Osci::InModeRandomizer())
        {
            k = TBase().RandK();
        }

        addrRead = static_cast<uint16>(ReadLastRecord(ch) - static_cast<int>(numPoints) / k);
    }

    HAL_BUS::FPGA::Write16(WR::PRED_LO, static_cast<uint16>(addrRead));
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);


    uint8 *a0 = Chan(ch).IsA() ? RD::DATA_A : RD::DATA_B;  // -V566
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    if (Osci::InModeRandomizer())
    {
        return ReadDataChannelRand(ch, a1, data);
    }
    else
    {
        uint8 *p = data;

        *p = HAL_BUS::FPGA::ReadA0();    // ������ ����� ������-�� ������������ ��������. ������ ���������� �.
        *p = HAL_BUS::FPGA::ReadA1();    // -V519

        if (PeakDetMode().IsEnabled())
        {
            for (uint i = 0; i < numPoints; i++)
            {
                *p++ = HAL_BUS::FPGA::ReadA0();
                *p++ = HAL_BUS::FPGA::ReadA1();
            }
        }
        else
        {
            float stretch = HAL_BUS::FPGA::GetStretch(a1);

            Buffer buffer(numPoints);

            for(uint i = 0; i < numPoints; i++)
            {
                int delta = VALUE::AVE - static_cast<int>(*a1);

                uint8 result = static_cast<uint8>(VALUE::AVE - static_cast<int>(delta * stretch));

                if(result < VALUE::MIN)      { p[i] = VALUE::MIN; }
                else if(result > VALUE::MAX) { p[i] = VALUE::MAX; }
                else                         { p[i] = result;     }
            }
        }
    }

    return true;
}
