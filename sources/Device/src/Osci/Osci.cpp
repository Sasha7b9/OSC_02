#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Keyboard/BufferButtons.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include <cstring>


// ��������� ��� �������� ����������, ����������� ��� ������ � ������ �������������
struct StructReadRand
{
    int step;       // ��� ����� �������
    int posFirst;   // ������� ������ ��������� �����
};


// ��������� ��� ������ �� ����������� ����� � �������������
struct RandShift
{
    // ���������� ������, ����������� ��� ������ ������ � ������� �������������.
    // ���� Tsm == 0, �� ��������� ����� �������������� �� ��� ������ ������, � ��� ����������� ���������.
    StructReadRand GetInfoForReadRand(ShiftPoint Tsm, const uint8 *address = nullptr);

    // ���������� true, ���� � ������ ������� ����� �� ����� ���� ������� � ��� � � ����� ������������ ����������
    bool Interpolated(int pos);

    static StructReadRand structRand;
};


struct Gates
{
    Gates() : minGate(0.0F), maxGate(0.0F) { }

    ShiftPoint CalculateShiftPoint();

private:
    static const int numberMeasuresForGates = 10000;
    static const uint TIME_WAIT = 3000;
    float minGate;
    float maxGate;
    // ����� �������� ��� ���������� � ��� ���������� �������� �� ���� ���������� � ������� Calculate
    MinMax2 m;
    // ����������� �������� �����
    void RecalculateGates();

    void CalculateWithoutGates(uint16 *min, uint16 *max);

    bool Calculate(uint16 value, uint16 *min, uint16 *max);
};


static RandShift randShift;
StructReadRand RandShift::structRand = { 0, 0 };

static Gates gates; // "������" �������������


int    Osci::addShift = 0;
void (*Osci::funcStop)() = EmptyFuncVV;


static void UpdateFPGA();

// � ����������� �� ��������� ����� ���������� ������ ������ ������ � ���������� ���� ������������� ���������� ������� ����� ����������
static bool ProcessFlagReady();


void Osci::Init()
{
    Stop();

    RAM::Init();
    FPGA::LoadRegUPR();
    Range::LoadBoth();
    RShift(Chan::A).Load();
    RShift(Chan::B).Load();
    TrigInput::Load();
    TrigLevel().Load();
    TrigPolarity().Load();
    TBase::Set();
    TShift::Load();
    FPGA::LoadCalibratorMode();
    LoadHoldfOff();
    HAL_PIO::Init(PIN_P2P, HMode::Input, HPull::Up);
    ChangedTrigStartMode();
    Osci::Start(true);
}


void Osci::DeInit()
{
    Stop();
}


void Osci::Start(bool)
{
    FPGA::forcedStart = false;
    addrRead = 0xffff;

    Roller::Prepare();

    FPGA::GiveStart(FPGA::pred, FPGA::post);

    FPGA::isRunning = true;
}


void Osci::Restart()
{
    if(FPGA::IsRunning())
    {
        Stop();
        Start(true);
    }

    ClearDataRand();
}


void Osci::Update()
{
    if(!Device::InModeOsci())
    {
        return;
    }

    if(FPGA::IsRunning())
    {
        UpdateFPGA();
    };

    Reader::ReadDataFromRAM();

    AutoMeasurements::SetData();
}


static void UpdateFPGA()
{
    int number = OSCI_IN_MODE_RANDOMIZER ? TBase::DeltaPoint() : 1;

    RAM::NewFrameForRandomize();

    uint lastID = 0;

    DataSettings *ds = RAM::Get();

    if(ds)
    {
        lastID = ds->id;
    }

    for(int i = 0; i < number; i++)
    {
        FPGA::ReadFlag();

        Osci::ProcessFlagPred();

        if(ProcessFlagReady())
        {
            Osci::Stop();
            break;
        }
    }

    if(OSCI_IN_MODE_RANDOMIZER)
    {
        Interpolator::Run(RAM::Get());
    }

    ds = RAM::Get();

    if(ds && ds->id != lastID)
    {
        if(ENumAverage() != ENumAverage::_1)               // ���� �������� ����������
        {
            DataSettings *last = RAM::Get(0);
            DataSettings *prev = RAM::Get(1);

            if(prev && last)
            {
                if(last->IsEquals(*prev))
                {
                    if(ENABLED_A(last))
                    {
                        AveragerOsci::Process(Chan::A, last->dataA, last->BytesInChannel());
                    }
                    if(ENABLED_B(last))
                    {
                        AveragerOsci::Process(Chan::B, last->dataB, last->BytesInChannel());
                    }
                }
            }
        }
    }
}


static bool ProcessFlagReady()
{
    bool needStop = false;

    if(FPGA::flag.DataReady())
    {
        if(Osci::CanReadData())
        {
            Timer::PauseOnTicks(5 * 90 * 20);

            Osci::ReadData();

            if(TrigStartMode::IsSingle())
            {
                needStop = true;
                Trig::pulse = false;
            }
            else
            {
                Timer::PauseOnTicks(5 * 90 * 20);
                Osci::Start(false);
            }
        }
    }

    return needStop;
}


void Osci::Stop()
{
    funcStop();
}


bool Osci::IsRunning()
{
    return FPGA::IsRunning();
}


void Osci::ProcessFlagPred()
{
    if(FPGA::flag.Pred() && !FPGA::forcedStart)
    {
        if(!OSCI_IN_MODE_RANDOMIZER && TrigStartMode::IsAuto() && FPGA::flag.HoldOff())
        {
            FPGA::ForcedStart();
        }
    }
}


bool Osci::CanReadData()
{
    if (ENumSignalsInSec::Is25())
    {
        return true;
    }
    
    static uint timePrevRead = 0;

    if (TIME_MS > timePrevRead + ENumSignalsInSec::TimeBetweenFramesMS())
    {
        timePrevRead = TIME_MS;

        return true;
    }

    return false;
}


void Osci::OnChangedPoints()
{
    FPGA::LoadRegUPR();
    FPGA::Reset();
    DisplayOsci::PainterData::ChangeTPos();
    FPGA::Reset();
    TShift::Set(set.time.shift);
    FPGA::Reset();
    RAM::Init();
}


void Osci::OnPressStart()
{
    return IsRunning() ? Stop() : Start(true);
}


void Osci::ChangedTBase()
{
    SetFunctionsStartStop();
}


void Osci::ChangedTrigStartMode()
{
    Stop();

    SetFunctionsStartStop();

    if(!TrigStartMode::IsSingle())
    {
        Start(true);
    }

    // ���� ��������� � ������ �������������
    if(OSCI_IN_MODE_RANDOMIZER)
    {
        // � ������������� �� ��������� ����� �������, �� ���� ��������� ��������� ��� �������, ����� ������������ ��� ����������� � ����� 
        // ������������� �������������� ��� ������
        if(TrigStartMode::IsSingle())
        {
            set.time.sampleTypeOld = SampleType();
            SampleType().Set(SampleType::Real);
        }
        else if(TrigStartMode::IsAuto())    // ����� ����������� ����� ����������
        {
            SampleType().Set(set.time.sampleTypeOld);
        }
    }
}


void Osci::SetFunctionsStartStop()
{
    static const pFuncVV stop[2][TrigStartMode::Count] =
    {
        //  Auto        Wait         Single
        { StopNormal, StopNormal,  StopNormal    },        // Normal mode
        { StopNormal, StopWaitP2P, StopSingleP2P }         // P2P mode
    };

    int index = OSCI_IN_MODE_P2P ? 1 : 0;

    //funcStart = start[index][TrigStartMode()];

    funcStop = stop[index][TrigStartMode()];
}


void Osci::StopNormal()
{
    FPGA::isRunning = false;
}


void Osci::StopWaitP2P()
{

}


void Osci::StopSingleP2P()
{

}


void Osci::InputController::Init()
{
    HAL_PIO::Init(PIN_SPI3_SCK, HMode::Output_PP, HPull::Down);     // SPI3_SCK
    HAL_PIO::Init(PIN_SPI3_DAT, HMode::Output_PP, HPull::Down);     // SPI3_DAT
    HAL_PIO::Init(PIN_SPI3_CS1, HMode::Output_PP, HPull::Down);     // SPI3_CS1
    HAL_PIO::Init(PIN_SPI3_CS2, HMode::Output_PP, HPull::Down);     // SPI3_CS2
    HAL_PIO::Init(PIN_A1, HMode::Output_PP, HPull::Down);           // A1
    HAL_PIO::Init(PIN_A2, HMode::Output_PP, HPull::Down);           // A2
    HAL_PIO::Init(PIN_A3, HMode::Output_PP, HPull::Down);           // A3
    HAL_PIO::Init(PIN_A4, HMode::Output_PP, HPull::Down);           // A4
    HAL_PIO::Init(PIN_LF1, HMode::Output_PP, HPull::Down);          // LF1 ����������� ������
    HAL_PIO::Init(PIN_LF2, HMode::Output_PP, HPull::Down);          // LF2 ����������� ������
    HAL_PIO::Init(PIN_LF3, HMode::Output_PP, HPull::Down);          // A1S
    HAL_PIO::Init(PIN_A0S, HMode::Output_PP, HPull::Down);          // A0S
    HAL_PIO::Init(PIN_LFS, HMode::Output_PP, HPull::Down);          // LFS
}

void Osci::InputController::Write(HPort::E portCS, uint16 pinCS, uint16 value)
{
    HAL_PIO::Reset(portCS, pinCS);

    if(portCS == PORT_SPI3_CS1)
    {
        for(int i = 15; i >= 0; --i)
        {
            HAL_PIO::Write(PIN_SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            HAL_PIO::Set(PIN_SPI3_SCK);
            HAL_PIO::Reset(PIN_SPI3_SCK);
        }
    }
    else if(portCS == PORT_SPI3_CS2)
    {
        for(int i = 0; i < 16; ++i)
        {
            HAL_PIO::Write(PIN_SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            HAL_PIO::Set(PIN_SPI3_SCK);
            HAL_PIO::Reset(PIN_SPI3_SCK);
        }
    }

    HAL_PIO::Set(portCS, pinCS);
}


void Osci::ClearDataRand()
{
    if(OSCI_IN_MODE_RANDOMIZER)
    {
        DataSettings *ds = RAM::Get();
        ds->Fill();

        std::memset(ds->Data(Chan::A), VALUE::NONE, ds->PointsInChannel());
        std::memset(ds->Data(Chan::B), VALUE::NONE, ds->PointsInChannel());

        std::memset(IntRAM::DataRand(Chan::A), VALUE::NONE, ds->PointsInChannel());
    }
}


void Osci::ReadData()
{
    Osci::Stop();

    DataSettings *ds = RAM::PrepareForNewData();

    if(ReadDataChannel(Chan::A, ds->dataA))
    {
        if(ReadDataChannel(Chan::B, ds->dataB))
        {
        }
    }
}


struct StructLog
{
    uint8 pos[10];
    StructLog()
    {
        std::memset(pos, 0, 10);
    }
    void Log()
    {
        LOG_WRITE("%d%d%d%d%d%d%d%d%d%d", pos[0], pos[1], pos[2], pos[3], pos[4], pos[5], pos[6], pos[7], pos[8], pos[9]);
    }
};


bool Osci::ReadDataChannelRand(uint8 *addr, uint8 *data)
{
    ShiftPoint Tsm = gates.CalculateShiftPoint();

    if(Tsm.type == ShiftPoint::FAIL)
    {
        return false;
    }

    StructReadRand infoRead = randShift.GetInfoForReadRand(Tsm, addr);

    static StructLog sl;

    if(infoRead.posFirst < 10)
    {
        sl.pos[infoRead.posFirst] = 1;
    }

    sl.Log();

    int step = infoRead.step;

    uint8 *dataRead = data + infoRead.posFirst;

    uint8 *interpolated = IntRAM::DataRand(Chan::A) + infoRead.posFirst;

    uint8 *last = data + ENumPointsFPGA::PointsInChannel();

    HAL_BUS::FPGA::SetAddrData(addr);

    if(ENumAverage() > 1)
    {
        uint8 *dataPointer = &data[infoRead.posFirst];              // ��������� � ���������� �������

        while(dataRead < last)
        {
            *dataRead = HAL_BUS::FPGA::ReadA0();
            *dataPointer = *dataRead;
            *interpolated = *dataRead;
            

            dataRead += step;
            dataPointer += step;
            interpolated += step;
        }
    }
    else
    {
        while(dataRead < last)
        {
            *dataRead = HAL_BUS::FPGA::ReadA0();
            *interpolated = *dataRead;

            dataRead += step;
            interpolated += step;

        }
    }

    return true;
}


ShiftPoint Gates::CalculateShiftPoint()
{
    ShiftPoint result(ShiftPoint::READED, 0);

    uint16 min = 0;
    uint16 max = 0;

    if(!Calculate(Osci::valueADC, &min, &max))
    {
        result.type = ShiftPoint::FAIL;
        return result;
    }

    if(((Osci::valueADC > max - setNRST.enumGameMax * 10) || (Osci::valueADC < min + setNRST.enumGameMin * 10)) && set.time.base > TBase::_5ns)
    {
        result.type = ShiftPoint::FAIL;
        return result;
    }

    float tin = static_cast<float>(Osci::valueADC - min) / (max - min);

    result.shift = static_cast<int>(tin * TBase::DeltaPoint());

    if(result.shift < 0)
    {
        result.shift = 0;
    }
    else if(result.shift >= TBase::DeltaPoint())
    {
        result.shift = TBase::DeltaPoint() - 1;
    }

    if(((Osci::valueADC > max - setNRST.enumGameMax * 10) || (Osci::valueADC < min + setNRST.enumGameMin * 10)) && set.time.base < TBase::_10ns)
    {
        result.type = ShiftPoint::FAIL;
    }

    return result;
}


StructReadRand RandShift::GetInfoForReadRand(ShiftPoint Tsm, const uint8 *address)
{
    if(Tsm.type != ShiftPoint::FAIL)
    {
        structRand.step = TBase::DeltaPoint();

        int index = Tsm.shift - Osci::addShift - structRand.step;

        while(index < 0)
        {
            index += structRand.step;
            volatile uint8 d = *address;
            d = d;
        }

        structRand.posFirst = index;
    }

    return structRand;
}


bool Gates::Calculate(uint16 value, uint16 *min, uint16 *max) 
{
    if(value < 250 || value > 4000)
    {
        return false;
    }

    m.Add(value);

    if((TIME_MS > TIME_WAIT) && (BufferButtons::TimeAfterControlMS() < TIME_WAIT))
    {
        CalculateWithoutGates(min, max);

        return true;
    }

    if(minGate == 0.0F) //-V550
    {
        *min = m.Min();
        *max = m.Max();
        if(m.Count() < numberMeasuresForGates)
        {
            return true;
        }
        minGate = m.Min();
        maxGate = m.Max();
        m.Reset();
    }

    if(m.Count() >= numberMeasuresForGates)
    {
        RecalculateGates();

        m.Reset();
    }

    *min = static_cast<uint16>(minGate);
    *max = static_cast<uint16>(maxGate);

    return (value >= *min) && (value <= *max);
}


void Gates::RecalculateGates()
{
    minGate = 0.8F * minGate + m.Min() * 0.2F;
    maxGate = 0.8F * maxGate + m.Max() * 0.2F;

    if(set.dbg.showRandGate)
    {
        static uint timePrev = 0;
        LOG_WRITE("����� ������ %d %d  ����� %d", static_cast<uint16>(minGate), static_cast<uint16>(maxGate), (TIME_MS - timePrev) / 1000);
        timePrev = TIME_MS;
    }
}


void Gates::CalculateWithoutGates(uint16 *min, uint16 *max)
{
    if(minGate == 0.0F) //-V550
    {
        *min = m.Min();
        *max = m.Max();
    }
    else
    {
        *min = static_cast<uint16>(minGate);
        *max = static_cast<uint16>(maxGate);
    }
}
