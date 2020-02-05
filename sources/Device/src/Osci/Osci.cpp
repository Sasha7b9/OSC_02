#include "defines.h"
#include "log.h"
#include "device.h"
#include "Osci/DataSettings.h"
#include "Hardware/Memory/Reader.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Utils/Values.h"


int    Osci::addShift = 0;
void (*Osci::funcStart)(bool) = EmptyFuncVB;
void (*Osci::funcStop)() = EmptyFuncVV;


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
    TShift().Load();
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


void Osci::Start(bool button)
{
    uint prev = RAM::NumberDatas();

    funcStart(button);

    if(prev != RAM::NumberDatas())
    {
        LOG_ERROR("���������� %d %d", prev, RAM::NumberDatas());
    }
}


void Osci::Restart()
{
    if(FPGA::IsRunning())
    {
        Stop();
        Start(true);
    }
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


void Osci::Stop()
{
    uint prev = RAM::NumberDatas();
    funcStop();

    if(prev != RAM::NumberDatas())
    {
        LOG_ERROR("%d %d", prev, RAM::NumberDatas());
    }
}


bool Osci::IsRunning()
{
    return FPGA::IsRunning();
}


void Osci::UpdateFPGA()
{
    uint number = (Osci::InModeRandomizer()) ? TBase().RandK() : 1;

    for (uint i = 0; i < number; i++)
    {
        FPGA::ReadFlag();
    
        ProcessFlagPred();

        if(ProcessFlagReady())
        {
            Osci::Stop();
            break;
        }
    }
}


void Osci::ProcessFlagPred()
{
    if(FPGA::flag.Pred() && !FPGA::forcedStart)
    {
        if(!FPGA::flag.TrigReady())
        {
            Trig::pulse = false;
        }

        if(InModeP2P() && !FrameP2P::IsCorrect())
        {
            return;
        }

        if(!InModeRandomizer() && TrigStartMode::IsAuto() && FPGA::flag.HoldOff())
        {
            FPGA::ForcedStart();
        }
    }
}


bool Osci::ProcessFlagReady()
{
    bool needStop = false;

    if(FPGA::flag.DataReady())
    {
        if(CanReadData())
        {
            Timer::PauseOnTicks(5 * 90 * 20);

            FPGA::ReadData();

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


void Osci::ReadPointP2P()
{
    if (FrameP2P::IsCorrect() && FPGA::IsRunning() && HAL_PIO::Read(PIN_P2P))
    {
        HAL_BUS::FPGA::SetAddrData(RD::DATA_A, RD::DATA_A + 1);
        BitSet16 dataA(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());

        HAL_BUS::FPGA::SetAddrData(RD::DATA_B, RD::DATA_B + 1);
        BitSet16 dataB(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());

        FrameP2P::AddPoint(dataA, dataB);
    }
}


bool Osci::InModeP2P()
{
    return (TBase() >= TBase::MIN_P2P);
}


bool Osci::InModeRandomizer()
{
    return (TBase() <= TBase::_50ns);
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
    TShift::Set(TShift());
    FPGA::Reset();
    RAM::Init();
}


Osci::StructReadRand Osci::GetInfoForReadRand(int Tsm, const uint8 *address)
{
    static StructReadRand result = { 0, 0 };

    if (Tsm != NULL_TSHIFT)
    {
        result.step = TBase().RandK();

        int index = Tsm - addShift;

        while (index < 0)
        {
            index += result.step;
            volatile uint8 d = *address;
            d = d;
        }

        result.posFirst = static_cast<uint>(index);
    }

    return result;
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
    if(Osci::InModeRandomizer())
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
        else
        {
            // ��� ��������
        }
    }
}


void Osci::SetFunctionsStartStop()
{
    static const pFuncVB start[2][TrigStartMode::Count] =
    {
        //  Auto         Wait          Single
        { StartNormal, StartNormal,  StartNormal    },     // Normal mode
        { StartP2P,    StartWaitP2P, StartSingleP2P }      // P2P mode
    };

    static const pFuncVV stop[2][TrigStartMode::Count] =
    {
        //  Auto        Wait         Single
        { StopNormal, StopNormal,  StopNormal    },        // Normal mode
        { StopNormal, StopWaitP2P, StopSingleP2P }         // P2P mode
    };

    int index = InModeP2P() ? 1 : 0;

    funcStart = start[index][TrigStartMode()];

    funcStop = stop[index][TrigStartMode()];
}


void Osci::StartNormal(bool)
{
    FPGA::forcedStart = false;
    FPGA::addrRead = 0xffff;

    FrameP2P::Prepare();

    uint prev = RAM::NumberDatas();

    FPGA::GiveStart(FPGA::pred, FPGA::post);

    if(prev != RAM::NumberDatas())
    {
        LOG_ERROR("%d %d", prev, RAM::NumberDatas());
    }

    FPGA::isRunning = true;
}


void Osci::StartP2P(bool button)
{
    uint prev = RAM::NumberDatas();

    FPGA::forcedStart = false;
    FPGA::addrRead = 0xffff;

    FrameP2P::Prepare();

    if(button || TrigStartMode::IsWait())
    {
        FPGA::GiveStart(FPGA::pred, FPGA::post);
        FrameP2P::Prepare();
    }

    FPGA::isRunning = true;

    if(prev != RAM::NumberDatas())
    {
        LOG_ERROR("%d %d", prev, RAM::NumberDatas());
    }
}


void Osci::StopNormal()
{
    FPGA::isRunning = false;
}


void Osci::StartWaitP2P(bool)
{

}


void Osci::StopWaitP2P()
{

}


void Osci::StartSingleP2P(bool)
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
    else
    {
        // ��� ��������
    }

    HAL_PIO::Set(portCS, pinCS);
}
