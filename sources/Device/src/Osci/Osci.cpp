#include "defines.h"
#include "device.h"
#include "Data/Reader.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ExtendedRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"


extern bool givingStart;

int Osci::addShift = 0;
void (*Osci::funcStart)() = EmptyFuncVV;


void Osci::Init()
{
    Stop();

    RAM::Init();
    ExtRAM::Init();
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
    Osci::OnPressStart();
}


void Osci::DeInit()
{
    Stop();
}


void Osci::OnPressStart()
{
    if (ModeWork::IsDir())
    {
        FPGA::OnPressStart();
    }
}


void Osci::Start()
{
    funcStart();
}


void Osci::Restart()
{
    if(FPGA::IsRunning())
    {
        Stop();
        Start();
    }
}


void Osci::Update()
{
    if(!Device::InModeOsci())
    {
        return;
    }

    if(InModeP2P() && !RAM::LastFrameIsP2P() && !TrigStartMode::IsSingle())
    {
        DataSettings *last = RAM::Get();
        if((TIME_MS - TIME_MS_DS(last) > 1000))
        {
            RAM::PrepareForNewData(true);
        }
    }

    if(FPGA::IsRunning())
    {
        UpdateFPGA();
    };

    Reader::ReadDataFromRAM();

    AutoMeasurements::SetData();
}


void Osci::Stop(bool)
{
    FPGA::isRunning = false;
}


bool Osci::IsRunning()
{
    return FPGA::IsRunning();
}


void Osci::UpdateFPGA()
{
    bool needStop = false;
    
    uint number = (Osci::InModeRandomizer()) ? TBase().RandK() : 1;

    for (uint i = 0; i < number; i++)
    {
        FPGA::ReadFlag();
    
        if (FPGA::flag.Pred() && !givingStart)
        {
            if (!Osci::InModeRandomizer() && TrigStartMode::IsAuto() && FPGA::flag.HoldOff())
            {
                FPGA::GiveStart();
                givingStart = true;
            }
            if (!FPGA::flag.TrigReady())
            {
                Trig::pulse = false;
            }
        }
        
        if (FPGA::flag.DataReady())
        {
            if (CanReadData())
            {
                Timer::PauseOnTicks(5 * 90 * 20);
    
                FPGA::ReadData();
    
                if (TrigStartMode::IsSingle())
                {
                    needStop = true;
                    Trig::pulse = false;
                }
                else
                {
                    Timer::PauseOnTicks(5 * 90 * 20);
                    Osci::Start();
                }
            }
        }
    }
    
    if(needStop)
    {
        Osci::OnPressStart();
    }
}


void Osci::ReadPointP2P()
{
    if (RAM::LastFrameIsP2P() && InModeP2P() && FPGA::IsRunning() && HAL_PIO::Read(PIN_P2P))
    {
        HAL_FSMC::SetAddrData(RD::DATA_A, RD::DATA_A + 1);
        BitSet16 dataA(HAL_FSMC::ReadData0(), HAL_FSMC::ReadData1());

        HAL_FSMC::SetAddrData(RD::DATA_B, RD::DATA_B + 1);
        BitSet16 dataB(HAL_FSMC::ReadData0(), HAL_FSMC::ReadData1());

        RAM::Get()->AddPoint(dataA, dataB);
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


void Osci::ChangedTrigStartMode()
{
    static const pFuncVV funcs[TrigStartMode::Count] = {Osci::StartAuto, Osci::StartWait, Osci::StartSingle};

    funcStart = funcs[TrigStartMode()];
}


void Osci::StartAuto()
{

}


void Osci::StartWait()
{

}


void Osci::StartSingle()
{

}


/*
    givingStart = false;
    FPGA::addrRead = 0xffff;

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::pred);
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, FPGA::post);
    HAL_FSMC::WriteToFPGA8(WR::START, 0xff);

    if (InModeP2P())
    {
        if(TrigStartMode::IsSingle())
        {
            RAM::PrepareForNewData(true);
        }
        else
        {
            DataSettings *last = RAM::Get();

            if(last == nullptr)
            {
                RAM::PrepareForNewData(true);
            }
            else if(last->isFrameP2P && !last->EqualsCurrentSettings())
            {
                RAM::PrepareForNewData(true);
            }
        }
    }

    FPGA::isRunning = true;
*/
