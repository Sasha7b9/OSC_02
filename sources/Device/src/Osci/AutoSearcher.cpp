#include "defines.h"
#include "log.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Utils/Math.h"


static void DisplayUpdate();

// ������ ������ �� ������ ch. ���������� true, ���� ������ ������ � ��������� ������� � tBase, range
static bool FindSignal(Chan::E ch, TBase::E *tBase, Range::E *range);

// ���������������� ������. ���� onlyReduce - ������ �����
static void ScaleChannel(Chan::E ch, bool onlyReduce);

// ������� ������� �� ������ ch
static bool FindFrequency(Chan::E ch, float *outFreq, Range::E *outRange);
static bool FindFrequencyForRanges(Chan::E ch, uint timeWaitMS, float *outFreq, Range::E *outRange);
static bool FindFrequencyForRange(Chan::E ch, Range::E range, uint timeWaitMS, float *outFreq);

// ������� �������� ������������� � ������� timeWaitMS ����������� � ���������� true, ���� ������������� ������
static bool WaitSync(uint timeWaitMS);

// ������������ TBase, ����������� ��� ����������� �������� �������
static TBase::E CalculateTBase(float frequency);


namespace AutoFPGA
{
    static const int SIZE = 300;
    void Start();
    void ReadData(Chan::E ch, uint8 *data);

    // ��������� ������ � ���������, ��� ��� �� ������� �� ������� ������������ � �������������. ���� false - ����� �� �����
    float ReadAndCalculateMinMax(Chan::E ch);
}


namespace FrequencyMeter
{
    // ����� ������� �� �������� �������
    void TuneForFind();
    
    namespace State
    {
        // ��������� ��������� ����������� ����� ������ �����������
        void Store();

        // ������������ ��������� ����������� ����� ����� ���������
        void Restore();

        bool enabled = S_FREQ_METER_ENABLED;
        FreqMeter::TimeCounting::E counting = S_FREQ_TIME_COUNTING;
        FreqMeter::FreqClc::E clc = S_FREQ_FREQ_CLC;
        FreqMeter::NumberPeriods::E periods = S_FREQ_NUMBER_PERIODS;
    }
}


void Osci::RunAutoSearch()
{
    Settings old = set;

    TBase::E tBase = S_TIME_BASE;
    Range::E rangeA = S_RANGE_A;
    Range::E rangeB = S_RANGE_B;

    if (FindSignal(ChanA, &tBase, &rangeA))
    {
        set = old;
        S_TIME_BASE = tBase;
        S_RANGE_A = rangeA;
        TrigSource::Set(ChanA);
        RShift::Set(ChanA, 0);
        TrigLevel::Set(ChanA, 0);
        TrigStartMode::Set(TrigStartMode::Wait);
        ModeCouple::Set(ChanA, ModeCouple::AC);

        ScaleChannel(ChanA, false);
        ScaleChannel(ChanB, true);

        Osci::Init();
    }
    else if (FindSignal(ChanB, &tBase, &rangeB))
    {
        set = old;
        S_TIME_BASE = tBase;
        S_RANGE_B = rangeB;
        TrigSource::Set(ChanB);
        RShift::Set(ChanB, 0);
        TrigLevel::Set(ChanB, 0);
        TrigStartMode::Set(TrigStartMode::Wait);
        ModeCouple::Set(ChanB, ModeCouple::AC);

        ScaleChannel(ChanA, true);
        ScaleChannel(ChanB, false);

        Osci::Init();
    }
    else
    {
        DISPLAY_SHOW_WARNING("������ �� ���������");

        set = old;
        Osci::Init();
    }

    FreqMeter::Init();
}


static bool FindSignal(Chan::E ch, TBase::E *tBase, Range::E *outRange)
{
    float frequency = 0.0F;

    if (FindFrequency(ch, &frequency, outRange))
    {
        *tBase = CalculateTBase(frequency);

        return true;
    }

    return false;
}


static bool FindFrequency(Chan::E ch, float *outFreq, Range::E *outRange)
{
    Osci::Stop();
    ModeCouple::Set(ch, ModeCouple::AC);
    TrigSource::Set(ch);
    TrigStartMode::Set(TrigStartMode::Wait);
    TrigLevel::Set(ch, 0);
    TBase::Set(TBase::_100ns);
    TShift::Set(0);
    RShift::Set(ch, 0);
    S_TRIG_INPUT = TrigInput::Full;
    TrigInput::Load();

    if (FindFrequencyForRanges(ch, 150, outFreq, outRange))
    {
        return true;
    }

    if (FindFrequencyForRanges(ch, 1200, outFreq, outRange))
    {
        return true;
    }

    return false;
}


static bool FindFrequencyForRanges(Chan::E ch, uint timeWaitMS, float *outFreq, Range::E *outRange)
{
    bool result = false;

    FrequencyMeter::State::Store();

    FrequencyMeter::TuneForFind();

    for (int range = static_cast<int>(Range::_20V); range > 0; range--)
    {
        float frequency1 = 0.0F;

        if (FindFrequencyForRange(ch, static_cast<Range::E>(range), timeWaitMS, &frequency1))
        {
            float frequency2 = 0.0F;

            if (FindFrequencyForRange(ch, static_cast<Range::E>(range - 1), timeWaitMS, &frequency2))
            {
                if (Math::FloatsIsEquals(frequency1, frequency2, 0.05F))
                {
                    *outFreq = (frequency1 + frequency2) / 2.0F;
                    *outRange = static_cast<Range::E>(range - 1);
                    result = true;
                    break;
                }
            }
        }
    }

    FrequencyMeter::State::Restore();

    return result;
}


static bool FindFrequencyForRange(Chan::E ch, Range::E range, uint timeWaitMS, float *outFreq)
{
    DisplayUpdate();

    Range::Set(ch, range);

    Timer::PauseOnTime(50);

    FPGA::Flag::Clear();

    Osci::Start(false);

    if (WaitSync(timeWaitMS))
    {
        do
        {
            DisplayUpdate();
            FPGA::Flag::Read(false);
        } while (!FPGA::Flag::FreqReady());

        BitSet32 counterFreq = FreqMeter::FPGA::ReadCounterFreq();

        if (counterFreq.word > 10)
        {
            *outFreq = counterFreq.word * 10.0F;
        }
        else
        {
            while (!FPGA::Flag::PeriodReady())
            {
                DisplayUpdate();
                FPGA::Flag::Read(false);
            }

            BitSet32 counterPeriod = FreqMeter::FPGA::ReadCounterPeriod();

            *outFreq = 1.0F / (counterPeriod.word * 10e-9F);
        }

        return true;
    }

    return false;
}


void FrequencyMeter::TuneForFind()
{
    S_FREQ_METER_ENABLED = true;
    S_FREQ_TIME_COUNTING = FreqMeter::TimeCounting::_100ms;
    S_FREQ_FREQ_CLC = FreqMeter::FreqClc::_100MHz;
    S_FREQ_NUMBER_PERIODS = FreqMeter::NumberPeriods::_1;

    FreqMeter::FPGA::LoadSettings();
}


static bool WaitSync(uint timeWaitMS)
{
    uint start = TIME_MS;           // ����� ������ ��������

    while (!FPGA::Flag::TrigReady())
    {
        if (TIME_MS - start > timeWaitMS)
        {
            return false;
        }

        FPGA::Flag::Read();

        DisplayUpdate();
    }

    return true;
}


static void DisplayUpdate()
{
    Painter::BeginScene(Color::BACK);

    Text("����� �������").DrawInCenterRect(0, 0, 320, 200, Color::FILL);

    int length = static_cast<int>((TIME_MS / 250) % 20);

    int dX = 5;

    int width = dX * length;

    for (int i = 0; i < length; i++)
    {
        Text(".").Draw(320 / 2 - width / 2 + i * dX, 120);
    }

    Painter::EndScene();
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
        {2.0F,   TBase::_200ms},
        {3.0F,   TBase::_100ms},
        {5.0F,   TBase::_50ms},
        {30.0F,  TBase::_20ms},
        {60.0F,  TBase::_5ms},
        {80.0F,  TBase::_2ms},

        {300.0F, TBase::_1ms},
        {600.0F, TBase::_500us},
        {800.0F, TBase::_200us},

        {3e3F,   TBase::_100us},
        {6e3F,   TBase::_50us},
        {8e3F,   TBase::_20us},

        {30e3F,  TBase::_10us},
        {60e3F,  TBase::_5us},
        {80e3F,  TBase::_2us},

        {300e3F, TBase::_1us},
        {600e3F, TBase::_500ns},
        {800e3F, TBase::_200ns},

        {3e6F,   TBase::_100ns},
        {6e6F,   TBase::_50ns},
        {15e6F,  TBase::_20ns},

        {30e6F,  TBase::_10ns},
        {60e6F,  TBase::_5ns},
        {0.0F,   TBase::Count}
    };

    for (int i = 0; times[i].tBase != TBase::Count; i++)
    {
        if (frequency < times[i].frequency)
        {
            return times[i].tBase;
        }
    }

    return TBase::_2ns;
}


void FrequencyMeter::State::Store()
{
    enabled = S_FREQ_METER_ENABLED;
    counting = S_FREQ_TIME_COUNTING;
    clc = S_FREQ_FREQ_CLC;
    periods = S_FREQ_NUMBER_PERIODS;
}


void FrequencyMeter::State::Restore()
{
    S_FREQ_METER_ENABLED = enabled;
    S_FREQ_TIME_COUNTING = counting;
    S_FREQ_FREQ_CLC = clc;
    S_FREQ_NUMBER_PERIODS = periods;

    FreqMeter::Init();
}


static void ScaleChannel(Chan::E ch, bool onlyReduce)
{
    float delta = AutoFPGA::ReadAndCalculateMinMax(ch);

    while (delta > 0.8F)
    {
        DisplayUpdate();
        Range::Change(ch, 1);
        Timer::PauseOnTime(100);
        delta = AutoFPGA::ReadAndCalculateMinMax(ch);
    }

    Range::E range = S_RANGE(ch);
    
    if (!onlyReduce)
    {
        while (delta < 0.4F)
        {
            DisplayUpdate();
            Range::Change(ch, -1);
            Osci::Init();
            range = S_RANGE(ch);
            range = range;
            Timer::PauseOnTime(100);
            delta = AutoFPGA::ReadAndCalculateMinMax(ch);
        }
    }
    
    range = S_RANGE(ch);
    range = range;
    
    delta = delta;
}


float AutoFPGA::ReadAndCalculateMinMax(Chan::E ch)
{
    Buffer buffer(AutoFPGA::SIZE);

    AutoFPGA::Start();

    int counter = 0;

    do
    {
        DisplayUpdate();
        FPGA::Flag::Read();
        counter++;

    } while (!FPGA::Flag::DataReady());

    counter = counter;

    AutoFPGA::ReadData(ch, buffer.data);

    uint8 max = Math::MaxFromArray(buffer.data, 0, 300);
    uint8 min = Math::MinFromArray(buffer.data, 0, 300);

    float delta = static_cast<float>(max - min) / static_cast<float>(VALUE::MAX - VALUE::MIN);

    return delta;
}


void AutoFPGA::Start()
{
    FPGA::GiveStart(static_cast<uint16>(~(1)), static_cast<uint16>(~(SIZE + 5)));

    for (int i = 0; i < 20; i++)
    {
        FPGA::Flag::Read();
        if (FPGA::Flag::Pred())
        {
            FPGA::ForcedStart();
        }
    }
}


void AutoFPGA::ReadData(Chan::E ch, uint8 *data)
{
    uint16 addrRead = (uint16)(Osci::ReadLastRecord(ch) - SIZE);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, addrRead);
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);

    uint8 *a0 = (ch == ChanA) ? RD::DATA_A : RD::DATA_B;
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    uint8 *p = data;
    *p = HAL_BUS::FPGA::ReadA1();

    for (int i = 0; i < SIZE; i++)
    {
        *p = HAL_BUS::FPGA::ReadA1();
        p++;
    }
}
