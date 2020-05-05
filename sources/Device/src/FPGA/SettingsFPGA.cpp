#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


void TrigStartMode::Set(TrigStartMode::E v)
{
    set.trig.startMode = v;
    Osci::ChangedTrigStartMode();
}


void TrigSource::Set(Chan::E ch)
{
    if (ch != Chan::Count)
    {
        set.trig.source = ch;
    }

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

    HAL_PIO::Write(PIN_LF3, _GET_BIT(datas[set.trig.input][set.trig.source], 2)); //-V525
    HAL_PIO::Write(PIN_A0S, _GET_BIT(datas[set.trig.input][set.trig.source], 1));
    HAL_PIO::Write(PIN_LFS, _GET_BIT(datas[set.trig.input][set.trig.source], 0));
}


static bool NeedLoadRShift(Chan::E ch)
{
    bool result = true;

    static Range::E prevRanges[Chan::Count] = { Range::Count, Range::Count };
    static int16 prevShift[Chan::Count] = { -1000, -1000 };

    if((prevShift[ch] == set.ch[ch].rShift) && (prevRanges[ch] == set.ch[ch].range))
    {
        result = false;
    }

    prevRanges[ch] = set.ch[ch].range;
    prevShift[ch] = set.ch[ch].rShift;

    return result;
}


void RShift::Load(Chan::E ch)
{
    if(!NeedLoadRShift(ch))
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    int16 shift = set.ch[ch].rShift + HARDWARE_ZERO;

    shift += setNRST.exShift[ch][set.ch[ch].range];

    if ((ch == ChanA) && Device::InModeTester())
    {
        shift -= Tester::DeltaRShiftA();
    }

    Osci::InputController::Write(PIN_SPI3_CS1, static_cast<uint16>(mask[ch] | (shift << 2)));

    Osci::Restart();

    if(OSCI_IN_MODE_RANDOMIZER)
    {
        DataSettings *ds = RAM::Get();

        if(ds)
        {
            ds->Fill();
        }
    }
}


void FPGA::LoadCalibratorMode()
{
    FPGA::LoadRegUPR();
}


void TShift::LoadReal()
{
    FPGA::post = static_cast<uint16>(set.time.shift - TShift().Min());
    int Pred = static_cast<int>(ENumPointsFPGA::PointsInChannel()) - static_cast<int>(FPGA::post);

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = static_cast<uint16>(Pred);

    FPGA::post = static_cast<uint16>(~(FPGA::post + 1));
    FPGA::pred = static_cast<uint16>(~(FPGA::pred + 3));

    HAL_BUS::FPGA::Write16(WR::PRED_LO, FPGA::post);
    HAL_BUS::FPGA::Write16(WR::POST_LO, FPGA::pred);
}


static int GetK()
{
    return (-TShift::Min()) % TBase::DeltaPoint();
}


void TShift::LoadRandomize()
{
    int k = TBase::DeltaPoint();

    FPGA::post = static_cast<uint16>((set.time.shift - TShift().Min() - GetK()) / k);

    if(set.time.shift - static_cast<int>(GetK()) < TShift().Min())
    {
        FPGA::post = 0;
    }

    int Pred = static_cast<int>(ENumPointsFPGA::PointsInChannel()) / static_cast<int>(k) - static_cast<int>(FPGA::post);

    if (Pred < 5)
    {
        Pred = 5;
    }

    FPGA::pred = static_cast<uint16>(Pred);

//    LOG_WRITE("pred = %d, post = %d", FPGA::pred, FPGA::post + 1);

    if(set.dbg.showRandPredPost)
    {
        LOG_WRITE("pred = %d, post = %d", FPGA::pred, FPGA::post);
    }

    FPGA::post = static_cast<uint16>(~(FPGA::post + 5));
    FPGA::pred = static_cast<uint16>(~(FPGA::pred));

    HAL_BUS::FPGA::Write16(WR::PRED_LO, FPGA::pred);
    HAL_BUS::FPGA::Write16(WR::POST_LO, FPGA::post);

    Osci::addShift = static_cast<int>(set.time.shift % k);

    if (Osci::addShift < 0)
    {
        Osci::addShift = static_cast<int>(k + Osci::addShift);
    }
}


void TShift::Load()
{
    if (OSCI_IN_MODE_RANDOMIZER)
    {
        LoadRandomize();
    }
    else
    {
        LoadReal();
    }
}


void TShift::Change(const int delta)
{
    if (Device::InModeRecorder())
    {
        return;
    }

    TShift::Set(set.time.shift + delta);
}


// ���������� range ��� ������ ������-����������
static void LimitForTester(Range::E *range)
{
    if(*range > Range::_5V)
    {
        *range = Range::_5V;
    }
    else if(*range < Range::_200mV)
    {
        *range = Range::_200mV;
    }
}


void Range::Change(Chan::E ch, int16 delta)
{
    if (Recorder::IsRunning())
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>(reinterpret_cast<uint8 *>(&set.ch[ch].range), static_cast<uint8>(Range::Count - 1)); // -V206
    }
    else
    {
        ::Math::LimitationDecrease<uint8>(reinterpret_cast<uint8 *>(&set.ch[ch].range), 0);  // -V206
    }

    if(Device::InModeTester())
    {
        LimitForTester(&set.ch[ch].range);
    }

    Range::LoadBoth();

    DisplayOsci::SetFlagRedraw();
}


void RShift::Set(Chan::E ch, int16 rShift)
{
    ::Math::Limitation(&rShift, MIN, MAX);
    set.ch[ch].rShift = rShift;
    Load(ch);
}


void RShift::Change(Chan::E ch, int16 delta)
{
    ::Math::AdditionThisLimitation<int16>(&set.ch[ch].rShift, STEP * delta, MIN, MAX);

    RShift::Load(ch);
}


void TrigPolarity::Load()
{
    FPGA::ForcedStart();
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
    m[ENumPointsFPGA::Count] =
    {
        StructENumPoints(-256 * mul + k,  -128 * mul + k, 0 * mul + k),  // 512
        StructENumPoints(-512 * mul + k,  -256 * mul + k, 0 * mul + k),  // 1024
        StructENumPoints(-1024 * mul + k,  -512 * mul + k, 0 * mul + k),  // 2048
        StructENumPoints(-2048 * mul + k, -1024 * mul + k, 0 * mul + k),  // 4096
        StructENumPoints(-4096 * mul + k, -2048 * mul + k, 0 * mul + k)   // 8192
    };

    return m[ENumPointsFPGA()].m[set.time.tPos];
}


int TShift::Max()
{
    return 60000;
}


String TShift::ToString(const TBase::E _base)
{
    TBase::E tBase = _base;

    if(tBase == TBase::Count)
    {
        tBase = set.time.base;
    }

    return Time(TShift::ToAbs(set.time.shift, tBase)).ToString(true);
}


String RShift::ToString(int16 rShiftRel, Range::E range, int8 _divider)
{
    float rShiftVal = ToAbs(rShiftRel, range) * Divider::ToAbs(static_cast<Divider::E>(_divider));
    return Voltage(rShiftVal).ToString(true);
}


pString TBase::Name(TBase::E tBase)
{
    static pString names[TBase::Count] =
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

    return names[tBase];
}


pString Range::Name(Chan::E ch)
{
    static const struct StructRange
    {
        const char * name;
        StructRange(pString nRU)
        {
            name = nRU;
        }
    }
    names[Range::Count] =
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

    return names[set.ch[ch].range].name;
};


pString ModeCouple::UGO(ModeCouple::E v)
{
    static const pString couple[] = { "\x92", "\x91", "\x90" };
    return couple[v];
}


ModeCouple::E &ModeCouple::Ref(Chan::E ch)
{
    return set.ch[ch].couple;
}


void Bandwidth::Load(Chan::E ch)
{
    static const StructPIN pinsLF[2] = { {PIN_LF1}, {PIN_LF2} };

    HAL_PIO::Write(pinsLF[ch], (set.ch[ch].bandwidth == Bandwidth::_20MHz));
}


void ModeCouple::Set(Chan::E ch, ModeCouple::E couple)
{
    set.ch[ch].couple = couple;
    Range::LoadBoth();
}
