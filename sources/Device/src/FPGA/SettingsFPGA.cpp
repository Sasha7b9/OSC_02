#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Display/DisplayOsci.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


void TrigSource::Load()
{
    TrigInput::Load();
}


void TrigInput::Load()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // �� 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // ��
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // ��
    };

    GPIO::WritePin(FPin::A1S, _GET_BIT(datas[static_cast<int>(set.trig.input)][static_cast<int>(set.trig.source)], 2));
    GPIO::WritePin(FPin::A0S, _GET_BIT(datas[static_cast<int>(set.trig.input)][static_cast<int>(set.trig.source)], 1));
    GPIO::WritePin(FPin::LFS, _GET_BIT(datas[static_cast<int>(set.trig.input)][static_cast<int>(set.trig.source)], 0));
}


void RShift::Load(Chan::E ch)
{
    set.disp.lastAffectedChannel = ch;

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    uint16 shift = SET_RSHIFT(ch);

    int8 add = set.dbg.addRShift[static_cast<int>(ch)][static_cast<int>(set.ch[static_cast<int>(ch)].range)];

    shift += add;

    if (Chan(ch).IsA() && Device::State::InModeTester())
    {
        shift = (uint16)((int)shift - Tester::DeltaRShiftA());
    }

    GPIO::WriteRegisters(FPin::SPI3_CS1, (uint16)(mask[static_cast<int>(ch)] | (shift << 2)));

    Osci::Restart();
}


void FPGA::LoadCalibratorMode()
{
    FPGA::LoadRegUPR();
}


void TShift::LoadReal()
{
    FPGA::post = (uint16)(set.time.shift - TShift::Min());
    int Pred = (int)FPGA_NUM_POINTS - (int)FPGA::post;

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = (uint16)Pred;

    FPGA::post = (uint16)(~(FPGA::post + 1));
    FPGA::pred = (uint16)(~(FPGA::pred + 3));

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::post);
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, FPGA::pred);
}


static int GetK()
{
    return (-TShift::Min()) % Osci::Kr[static_cast<int>(set.time.base)];
}


void TShift::LoadRandomize()
{
    int k = Osci::Kr[static_cast<int>(set.time.base)];

    FPGA::post = (uint16)((set.time.shift - TShift::Min() - GetK()) / k);

    int Pred = (int)FPGA_NUM_POINTS / k - (int)FPGA::post;

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = (uint16)Pred;

    FPGA::post = (uint16)(~(FPGA::post + 1));
    FPGA::pred = (uint16)(~(FPGA::pred));

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::pred);
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, FPGA::post);

    Osci::addShift = (set.time.shift) % k;

    if (Osci::addShift < 0)
    {
        Osci::addShift = k + Osci::addShift;
    }
}


void TShift::Load()
{
    if (Osci::InModeRandomizer())
    {
        LoadRandomize();
    }
    else
    {
        LoadReal();
    }
}


void TShift::Change(int delta)
{
    if (Device::State::InModeRecorder())
    {
        return;
    }

    TShift::Set(set.time.shift + delta);
}


void Range::Change(Chan::E ch, int delta)
{
    if (Recorder::IsRunning())
    {
        return;
    }

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&set.ch[static_cast<int>(ch)].range), (uint8)(Range::Size - 1)); // -V206
    }
    else
    {
        ::Math::LimitationDecrease<uint8>((uint8 *)(&set.ch[static_cast<int>(ch)].range), 0);  // -V206
    }
    Range::LoadBoth();

    DisplayOsci::SetFlagRedraw();
}


void Range::Set(Chan::E ch, E range)
{
    set.ch[static_cast<int>(ch)].range = range;
    LoadBoth();
}


void RShift::Change(Chan::E ch, int delta)
{
    ::Math::AdditionThisLimitation<uint16>(&SET_RSHIFT(ch), STEP_RSHIFT * delta, RShift::MIN, RShift::MAX);

    Load(ch);
}


void RShift::Set(Chan::E ch, uint16 rShift)
{
    ::Math::Limitation<uint16>(&rShift, MIN, MAX);
    SET_RSHIFT(ch) = rShift;
    Load(ch);
}


void TrigPolarity::Load()
{
    FPGA::GiveStart();
}


int TShift::Min()
{
#define k 0
#define mul 2

    static const struct StructENumPoints
    {
        int m[3];
        StructENumPoints(int m0, int m1, int m2) { m[0] = m0; m[1] = m1; m[2] = m2; }
    }
    m[ENumPointsFPGA::Size] =
    {
        StructENumPoints(-256 * mul + k,  -128 * mul + k, 0 * mul + k),  // 512
        StructENumPoints(-512 * mul + k,  -256 * mul + k, 0 * mul + k),  // 1024
        StructENumPoints(-1024 * mul + k,  -512 * mul + k, 0 * mul + k),  // 2048
        StructENumPoints(-2048 * mul + k, -1024 * mul + k, 0 * mul + k),  // 4096
        StructENumPoints(-4096 * mul + k, -2048 * mul + k, 0 * mul + k)   // 8192
    };

    return m[(int)set.mem.enumPoints].m[(int)set.time.TPos];
}


int TShift::Zero()
{
    return 0;
}


int TShift::Max()
{
    return 60000;
}

void TShift::Set(int tShift)
{
    LIMITATION(tShift, Min(), Max()); //-V2516

    set.time.shift = tShift;

    TShift::Load();

    Osci::Restart();

    DisplayOsci::SetFlagRedraw();
}


String TShift::ToString(TBase::E tBase)
{
    return Time(MathFPGA::TShift2Abs(set.time.shift, tBase)).ToString(true);
}


String RShift::ToString(uint16 rShiftRel, Range::E range, int8 _divider)
{
    float rShiftVal = MathFPGA::RShift2Abs(rShiftRel, range) * Divider((uint)_divider).ToAbs();
    return Voltage(rShiftVal).ToString(true);
}


pString TBase::Name() const
{
    static pString names[TBase::Size] =
    {
        "2��",
        "5��",
        "10��",
        "20��",
        "50��",
        "0.1���",
        "0.2���",
        "0.5���",
        "1���",
        "2���",
        "5���",
        "10���",
        "20���",
        "50���",
        "0.1��",
        "0.2��",
        "0.5��",
        "1��",
        "2��",
        "5��",
        "10��",
        "20��",
        "50��",
        "0.1�",
        "0.2�",
        "0.5�",
        "1�",
        "2�",
        "5�",
        "10�"
    };

    return names[static_cast<int>(value)];
}


pString Range::Name() const
{
    static const struct StructRange
    {
        const char * name;
        StructRange(pString nRU)
        {
            name = nRU;
        }
    }
    names[Range::Size] =
    {
        StructRange("2��"),
        StructRange("5��"),
        StructRange("10��"),
        StructRange("20��"),
        StructRange("50��"),
        StructRange("0.1�"),
        StructRange("0.2�"),
        StructRange("0.5�"),
        StructRange("1�"),
        StructRange("2�"),
        StructRange("5�"),
        StructRange("10�"),
        StructRange("20�")
    };

    return names[static_cast<int>(value)].name;
};


void ModeCouple::Set(Chan::E ch, ModeCouple::E modeCoupe)
{
    set.ch[(int)ch].couple = modeCoupe;
    Range::LoadBoth();
}


pString ModeCouple::UGO() const
{
    static const pString couple[] = { "\x92", "\x91", "\x90" };
    return couple[(int)value];
}


void Bandwidth::Load()
{
    Chan::E ch = GetChannel();
    static const FPin::E pinsLF[2] = { FPin::LF1, FPin::LF2 };

    GPIO::WritePin(pinsLF[(int)ch], (set.ch[(int)ch].bandwidth.value == Bandwidth::_20MHz));
}


Chan::E Bandwidth::GetChannel() const
{
    return (&set.ch[Chan::A].bandwidth.value == &this->value) ? Chan::A : Chan::B;
}
