#include "defines.h"
#include "FPGA/FPGA.h"
#include "FreqMeter/FreqMeter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Utils/Values.h"


// ������� ����� ������ �� ������ ch. ���� ������� �������, �� ��������� ��������� ����������� � ������
static bool FindSignal(Chan::E ch);

// ������� ������� �������, ��������� �� ���� ch. � ������ ������� ���������� �������� Float::ERROR
static float FindFrequency(Chan::E ch);
static float FindFrequency(Chan::E ch, Range::E range);

// ������� ������� �� ���������
static Range::E FindRange(Chan::E ch);

// ������������ TBase, ����������� ��� ����������� �������� �������
static TBase::E CalculateTBase(float frequency);

// ������� ��������������� ��������� � ����������
static void TuneFreqMeter();

// ������������ �������, ������ �� ������ ����������� �����������
static float CalculateFrequency();


void Osci::RunAutoSearch()
{
    HAL_IWDG::Disable();

    Settings old = set;

    if (!FindSignal(ChanA))
    {
        set = old;

        if (!FindSignal(ChanB))
        {
            set = old;

            DISPLAY_SHOW_WARNING("������ �� ���������");
        }
    }

    Osci::Init();

    HAL_IWDG::Enable();
}


static bool FindSignal(Chan::E ch)
{
    float frequency = FindFrequency(ch);

    if (frequency != Float::ERROR)
    {
        TBase::Set(CalculateTBase(frequency));

        //Range::Set(ch, FindRange(ch));
    }

    return (frequency != Float::ERROR);
}


static float FindFrequency(Chan::E ch)
{
    ModeCouple::Set(ch, ModeCouple::AC);
    TrigSource::Set(ch);
    TrigStartMode::Set(TrigStartMode::Wait);
    TrigLevel::Set(ch, 0);
    TBase::Set(TBase::_100ns);
    TShift::Set(0);
    RShift::Set(ch, 0);
    
    float frequency = Float::ERROR;

    for (int range = Range::_20V; range >= 0 && (frequency == Float::ERROR); range--)
    {
        frequency = FindFrequency(ch, static_cast<Range::E>(range));
    }

    return frequency;
}


static float FindFrequency(Chan::E ch, Range::E range)
{
    /*
        ������� ������ � ������� �����������
        1. ������������� ���� ������� � ���������� �����������
        2. ������ ��������, ��� ����� ������� ��.
        3. ��� ������.
        4. ���, ���� �������� ���� ������� ��� �������
    */

    Settings old = set;

    Osci::Stop();
    Range::Set(ch, range);

    Timer::PauseOnTime(500);

    TuneFreqMeter();
    Osci::Start(false);

    do
    {
        FPGA::ReadFlag();

    } while (!FPGA::Flag::PeriodReady() || !FPGA::Flag::FreqReady());

    set = old;

    return CalculateFrequency();
}


static TBase::E CalculateTBase(float frequency)
{
    struct TimeStruct
    {
        float       frequency;  // �������, ���� ������� ����� ������������� tBase
        TBase::E    tBase;
    };

    static const TimeStruct times[] =
    {
        {10.0F,  TBase::_10ms},
        {30.0F,  TBase::_5ms},
        {80.0F,  TBase::_2ms},

        {100.0F, TBase::_1ms},
        {300.0F, TBase::_500us},
        {800.0F, TBase::_200us},

        {1e3F,   TBase::_100us},
        {3e3F,   TBase::_50us},
        {8e3F,   TBase::_20us},

        {10e3F,  TBase::_10us},
        {30e3F,  TBase::_5us},
        {80e3F,  TBase::_2us},

        {100e3F, TBase::_1us},
        {300e3F, TBase::_500ns},
        {800e3F, TBase::_200ns},

        {1e6F,   TBase::_100ns},
        {3e6F,   TBase::_50ns},
        {8e6F,   TBase::_20ns},

        {10e6F,  TBase::_10ns},
        {30e6F,  TBase::_5ns},
        {250e6F, TBase::_2ns},
        {0.0F,   TBase::Count}
    };

    int i = 0;

    while (times[i].tBase != TBase::Count)
    {
        if (frequency < times[i].frequency)
        {
            return times[i].tBase;
        }
    }

    return TBase::_2ns;
}


Range::E FindRange(Chan::E)
{
    return Range::Count;
}


static void TuneFreqMeter()
{
    S_FREQ_METER_ENABLED = true;
    S_FREQ_TIME_COUNTING = FreqMeter::TimeCounting::_1s;
    S_FREQ_FREQ_CLC = FreqMeter::FreqClc::_100MHz;
    S_FREQ_NUMBER_PERIODS = FreqMeter::NumberPeriods::_1;

    FreqMeter::FPGA::LoadSettings();
    FreqMeter::FPGA::ResetCounterFreq();
    FreqMeter::FPGA::ResetCounterPeriod();

    FPGA::Flag::Clear();
}


static float CalculateFrequency()
{
    if (!FPGA::Flag::FreqOverflow() && FPGA::Flag::FreqReady())
    {
        BitSet32 counter = FreqMeter::FPGA::ReadCounterFreq();
        uint freq = counter.word;
        freq = freq;
    }

    if (!FPGA::Flag::PeriodOverflow() && FPGA::Flag::PeriodReady())
    {
        BitSet32 counter = FreqMeter::FPGA::ReadCounterPeriod();
        counter = counter;
    }

    return Float::ERROR;
}
