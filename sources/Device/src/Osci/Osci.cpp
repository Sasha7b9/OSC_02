#include "defines.h"
#include "device.h"
#include "Data/Reader.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"
#include <cstring>

#include "Osci/Display/PainterData.h"


using namespace FPGA;
using namespace ::HAL;
using namespace ::HAL::ADDRESSES::FPGA;
using namespace Osci::Settings;


extern bool givingStart;

extern uint16 addrRead;

extern uint8 dataRand[Chan::Size][FPGA::MAX_NUM_POINTS];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
volatile static int numberMeasuresForGates = 1000;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������ ����� � ������������� � ������ address
void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data);

static int CalculateShift();

static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);
/// ������ ����� � ���������� ������
static void ReadPointP2P();
/// ���������� true, ���� ��� ����� ������ ������
static bool CanReadData();

namespace Osci
{
    static void UpdateFPGA();

    int addShift = 0;
                   //  2�� 5�� 10�� 20�� 50��
    const int Kr[TBase::Size] = { 50, 20, 10,  5,   2 };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Init()
{
    Stop();

    FPGA::HAL::LoadRegUPR();
    Settings::Range::LoadBoth();
    Settings::RShift::Load(Chan::A);
    Settings::RShift::Load(Chan::B);
    Settings::Trig::Input::Load();
    Settings::Trig::Level::Load();
    Settings::TBase::Load();
    Settings::TShift::Load();
    FPGA::Settings::LoadCalibratorMode();
    Settings::LoadHoldfOff();
    FPGA::HAL::Interrupt::P2P::Init(ReadPointP2P);
    FPGA::OnPressStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::DeInit()
{
    Stop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Start()
{
    givingStart = false;
    addrRead = 0xffff;

    FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::pred);
    FSMC::WriteToFPGA16(WR::POST_LO, FPGA::post);
    FSMC::WriteToFPGA8(WR::START, 0xff);

    FPGA::timeStart = TIME_MS;

    FPGA::isRunning = true;

    if (InModeP2P())
    {
        Storage::PrepareNewFrameP2P();
        FPGA::HAL::Interrupt::P2P::Enable();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Stop(bool)
{
    FPGA::HAL::Interrupt::P2P::Disable();

    isRunning = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Restart()
{
    if (FPGA::IsRunning())
    {
        Stop();
        Start();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Update()
{
    if (!Device::State::InModeOsci())
    {
        return;
    }

    if (FPGA::IsRunning())
    {
        UpdateFPGA();
    };

    Reader::ReadDataFromStorage();

    Measurements::SetData();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Osci::UpdateFPGA()
{
    int number = (Osci::InModeRandomizer()) ? Kr[SET_TBASE] : 1;

    for (int i = 0; i < number; i++)
    {
        FPGA::HAL::ReadFlag();

        if (FPGA::HAL::GetFlag::PRED() && !givingStart)
        {
            if (!Osci::InModeRandomizer() && START_MODE_IS_AUTO && FPGA::HAL::GetFlag::HOLD_OFF())
            {
                GiveStart();
                givingStart = true;
            }
            if (!FPGA::HAL::GetFlag::TRIG_READY())
            {
                Trig::pulse = false;
            }
        }

        if (FPGA::HAL::GetFlag::DATA_READY())
        {
            if (CanReadData())
            {
                Timer::PauseOnTicks(5 * 90 * 20);

                ReadData();
                if (START_MODE_IS_SINGLE)
                {
                    OnPressStart();
                    Trig::pulse = false;
                }
                else
                {
                    Start();
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data) // -V2506
{
    int Tsm = CalculateShift();

    if (Tsm == NULL_TSHIFT)
    {
        return;
    }

    int step = Osci::Kr[SET_TBASE];

    int index = Tsm - Osci::addShift;

    uint8 *dataRead = &dataRand[ch][index];

    while (index < 0)
    {
        volatile uint8 d = *address;
        d = d;
        index += step;
        dataRead += step;
    }

    uint8 *last = &dataRand[ch][FPGA_NUM_POINTS];

    while (dataRead < last)
    {
        *dataRead = *address;

        dataRead += step;
    }

    std::memcpy(data, &dataRand[ch][0], FPGA_NUM_POINTS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateShift()
{
    uint16 min = 0;
    uint16 max = 0;

    if (!CalculateGate(valueADC, &min, &max))
    {
        return NULL_TSHIFT;
    }

    int deltaMAX = set.dbg_enum_gate_max * 10;
    int deltaMIN = set.dbg_enum_gate_min * 10;

    if (valueADC > max - deltaMAX || valueADC < min + deltaMIN)
    {
        return NULL_TSHIFT;
    }

    if (Osci::InModeRandomizer())
    {

        float tin = (float)(valueADC - min + deltaMIN) / (max - deltaMAX - (min + deltaMIN));
        int retValue = (int)(tin * Osci::Kr[SET_TBASE]);

        return retValue;
    }

    return NULL_TSHIFT;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{
    static float minGate = 0.0F;
    static float maxGate = 0.0F;

    if (rand < 500 || rand > 4000)
    {
        return false;
    }

    static int numElements = 0;
    static uint16 min = 0xffff;
    static uint16 max = 0;

    numElements++;

    bool retValue = true;

    if (rand < min)
    {
        min = rand;
    }
    if (rand > max)
    {
        max = rand;
    }

    if (minGate == 0.0F)    // -V550
    {
        *eMin = min;
        *eMax = max;
        if (numElements < numberMeasuresForGates)
        {
            return true;
        }
        minGate = min;
        maxGate = max;
        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    if (numElements >= numberMeasuresForGates)
    {
        minGate = 0.8F * minGate + min * 0.2F;
        maxGate = 0.8F * maxGate + max * 0.2F;

        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    *eMin = (uint16)(minGate);      // -V519 // -V2004
    *eMax = (uint16)(maxGate - 50); // -V519 // -V2004

    LOG_WRITE("������ %d %d", *eMin, *eMax);

    if (rand < *eMin || rand > *eMax)
    {
        return false;
    }

    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadPointP2P()
{
    /// ����� ������ �����. ���� == 0, �� ����� ����� ���������� ��� ���������� �����, ����� - �� time
    static uint time = 0;

    if(::HAL::FSMC::InterchangeWithPanel())
    {
        time = TIME_MS;
        ::HAL::FSMC::RunFunctionAfterInteractionWitchPanel(ReadPointP2P);
    }
    else
    {
        if (time == 0)
        {
            time = TIME_MS;
        }

        BitSet16 dataA(FSMC::ReadFromFPGA(RD::DATA_A), FSMC::ReadFromFPGA(RD::DATA_A + 1));
        BitSet16 dataB(FSMC::ReadFromFPGA(RD::DATA_B), FSMC::ReadFromFPGA(RD::DATA_B + 1));

        Osci::Storage::GetFrameP2P()->AddPoints(time, dataA, dataB);

        time = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Osci::InModeP2P()
{
    return (SET_TBASE >= TBase::MIN_P2P);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Osci::InModeRandomizer()
{
    return (SET_TBASE <= TBase::_50ns);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool CanReadData()
{
    if (ENUM_SIGNALS_IN_SEC.value == Display::ENumSignalsInSec::_25)
    {
        return true;
    }
    
    static uint timePrevRead = 0;

    if (TIME_MS > timePrevRead + ENUM_SIGNALS_IN_SEC.TimeBetweenFramesMS())
    {
        timePrevRead = TIME_MS;

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::OnChangedPoints()
{
    FPGA::HAL::LoadRegUPR();
    FPGA::Reset();
    Osci::Display::PainterData::ChangeTPos();
    FPGA::Reset();
    TShift::Set(SET_TSHIFT);
    FPGA::Reset();
}
