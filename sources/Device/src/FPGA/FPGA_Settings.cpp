#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "device.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Font/Font.h"
#include "FPGA.h"
#include "FPGA_HAL.h"
#include "FPGA_Settings.h"
#include "FPGA_Math.h"
#include "FPGATypes.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#endif


using namespace Display::Primitives;
using namespace FPGA::HAL::GPIO;
using namespace FPGA::SET;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FPGA::SET::TShift::value = 0;

namespace Osci
{
    namespace Settings
    {
        namespace Trig
        {
            /// ������������� � true �������� ��������, ��� ����� �������� ������ ��������������
            bool pulse = false;
            ///< ����� �� �������� �������������� ����� ������ �������� ������ �������������.
            static bool needDraw = false;
            /// ��������� ����������� ������ ������������� ������ �������
            static void DisableDrawing();
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8 ValueForRange(Chan::E ch);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Settings::Trig::Source::Load()
{
    Input::Load();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::Trig::Input::Load()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // �� 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // ��
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // ��
    };

    WritePin(Pin::A1S, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 2));
    WritePin(Pin::A0S, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 1));
    WritePin(Pin::LFS, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 0));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::Range::LoadBoth()
{
    uint16 val = (uint16)(ValueForRange(Chan::B) + (ValueForRange(Chan::A) << 8));

    WriteRegisters(Pin::SPI3_CS2, val);

    PAUSE_ON_MS(10);                // �������� �����, ����� ���������� ���� ������ ����������

    WriteRegisters(Pin::SPI3_CS2, 0);    // ���������� ����, ����� ���� �� ���������� �������

    DEF_STRUCT(StructRange, uint8) vals[Osci::Settings::Range::Number] =
    {
        StructRange(BIN_U8(00000000)),  // 2mV      // -V2501
        StructRange(BIN_U8(00000001)),  // 5mV      // -V2501
        StructRange(BIN_U8(00000010)),  // 10mV     // -V2501
        StructRange(BIN_U8(00000011)),  // 20mV     // -V2501
        StructRange(BIN_U8(00000001)),  // 50mV     // -V2501
        StructRange(BIN_U8(00000010)),  // 100mV    // -V2501
        StructRange(BIN_U8(00000011)),  // 200mV    // -V2501
        StructRange(BIN_U8(00000001)),  // 500mV    // -V2501
        StructRange(BIN_U8(00000010)),  // 1V       // -V2501
        StructRange(BIN_U8(00000011)),  // 2V       // -V2501
        StructRange(BIN_U8(00000001)),  // 5V       // -V2501
        StructRange(BIN_U8(00000010)),  // 10V      // -V2501
        StructRange(BIN_U8(00000011))   // 20V      // -V2501
    };

    uint8 valueA = vals[SET_RANGE_A].val;

    WritePin(Pin::A1, _GET_BIT(valueA, 1));
    WritePin(Pin::A2, _GET_BIT(valueA, 0));

    uint8 valueB = vals[SET_RANGE_B].val;

    WritePin(Pin::A3, _GET_BIT(valueB, 1));
    WritePin(Pin::A4, _GET_BIT(valueB, 0));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::RShift::Load(Chan::E ch)
{
    LAST_AFFECTED_CH = ch;

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    uint16 shift = SET_RSHIFT(ch);

    if (Chan(ch).IsA() && Device::CurrentMode() == Device::Mode::Tester)
    {
        shift = (uint16)((int)shift - Tester::DeltaRShiftA());
    }

    WriteRegisters(Pin::SPI3_CS1, (uint16)(mask[ch] | (shift << 2)));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::Trig::Level::Load()
{
    /// \todo ����� ����� ������ ��������. ����� ���-�� ������� � ������ SET_TRIGLEV_SOURCE
    uint16 value = (uint16)((Trig::MAX + Trig::MIN) - SET_TRIGLEV_SOURCE);

    WriteRegisters(Pin::SPI3_CS1, (uint16)(0xa000 | (value << 2)));
}

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::TBase::Load()
{
    static const uint8 values[TBase::Number] =
    {
        BIN_U8(00000000),  // -V2501  // 2ns     1       200MHz
        BIN_U8(00000000),  // -V2501  // 5ns     1       200MHz
        BIN_U8(00000000),  // -V2501  // 10ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 20ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 50ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 100ns   1       200MHz
        BIN_U8(00100000),  // -V2501  // 200ns   2       100MHz
        BIN_U8(00100001),  // -V2501  // 500ns   5       40MHz
        BIN_U8(00100010),  // -V2501  // 1us     10      20MHz
        BIN_U8(00100011),  // -V2501  // 2us     20      10MHz
        BIN_U8(01000101),  // -V2501  // 5us     50      4MHz
        BIN_U8(01000110),  // -V2501  // 10us    100     2MHz
        BIN_U8(01000111),  // -V2501  // 20us    200     1MHz
        BIN_U8(01001001),  // -V2501  // 50us    500     400kHz
        BIN_U8(01001010),  // -V2501  // 100us   1k      200kHz
        BIN_U8(01001011),  // -V2501  // 200us   2k      100kHz
        BIN_U8(01001101),  // -V2501  // 500us   5k      40kHz
        BIN_U8(01001110),  // -V2501  // 1ms     10k     20kHz
        BIN_U8(01001111),  // -V2501  // 2ms     20k     10kHz
        BIN_U8(01010001),  // -V2501  // 5ms     50k     4kHz
        BIN_U8(01010010),  // -V2501  // 10ms    100k    2kHz
        BIN_U8(01010011),  // -V2501  // 20ms    200k    1kHz
        BIN_U8(01010101),  // -V2501  // 50ms    500k    400Hz
        BIN_U8(01010110),  // -V2501  // 100ms   1M      200Hz
        BIN_U8(01010111),  // -V2501  // 200ms   2M      100Hz
        BIN_U8(01011001),  // -V2501  // 500ms   5M      40Hz
        BIN_U8(01011010),  // -V2501  // 1s      10M     20Hz
        BIN_U8(01011011),  // -V2501  // 2s      20M     10Hz
        BIN_U8(01011101),  // -V2501  // 5s      50M     4Hz
        BIN_U8(01011110)   // -V2501  // 10s     100M    2Hz
    };

    FPGA::ClearDataRand();

    FSMC::WriteToFPGA8(WR_TBASE, values[SET_TBASE]);

    TShift::Load();
}

#ifdef _WIN32
#pragma warning(pop)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::LoadHoldfOff()
{
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_ENABLE, TRIG_HOLDOFF_ENABLED ? 1U : 0U);

    uint value = (uint)(0 - TRIG_HOLDOFF + 1);

    BitSet32 bs(value);

    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_LOW, bs.byte0);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_MID, bs.byte1);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_HI, bs.byte2);
}

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 ValueForRange(Chan::E ch) // -V2506
{
    static const uint8 datas[ModeCouple::Size] =
    {
        BIN_U8(01000001),  // -V2501  // DC
        BIN_U8(10000001),  // -V2501  // AC
        BIN_U8(00000010)   // -V2501  // GND
    };

    if (SET_COUPLE(ch) == ModeCouple::GND && Device::CurrentMode() == Device::Mode::Osci)
    {
        return datas[ModeCouple::GND];
    }

    DEF_STRUCT(StructRange, uint16) values[Osci::Settings::Range::Number][2] =
    {   //             A                    B
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 2mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 5mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 10mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 20mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 50mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 100mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 200mV
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 500mV
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 1V
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 2V
        { BIN_U8(00011000), BIN_U8(00011000) }, // -V2501  // 5V
        { BIN_U8(00011000), BIN_U8(00011000) }, // -V2501  // 10V
        { BIN_U8(00011000), BIN_U8(00011000) }  // -V2501  // 20V
    };

    return (uint8)(values[SET_RANGE(ch)][ch].val | datas[SET_COUPLE(ch)]);
}

#ifdef WIN32
#pragma warning(pop)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::LoadCalibratorMode()
{
    HAL::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::TShift::Load()
{
    FPGA::post = (uint16)(SET_TSHIFT - TShift::Min());
    int Pred = (int)FPGA::NumPoints() - (int)FPGA::post;

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = (uint16)Pred;

    FPGA::post = (uint16)(~(FPGA::post + 1));
    FPGA::pred = (uint16)(~(FPGA::pred + 3));

    FSMC::WriteToFPGA16(WR_PRED_LO, FPGA::post);
    FSMC::WriteToFPGA16(WR_POST_LO, FPGA::pred);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::TShift::Change(int delta)
{
    TShift::Set(SET_TSHIFT + delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::Range::Change(Chan::E ch, int delta)
{
    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&SET_RANGE(ch)), (uint8)(Range::Number - 1)); // -V206
    }
    else
    {
        ::Math::LimitationDecrease<uint8>((uint8 *)(&SET_RANGE(ch)), 0);  // -V206
    }
    Range::LoadBoth();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::TBase::Change(int delta)
{
    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&SET_TBASE), (uint8)(TBase::Number - 1)); // -V206
    }
    else
    {
        if (SET_PEAKDET_EN &&                               // ���� ������ ����� �������� ���������
            SET_TBASE == TBase::MIN_PEAK_DET)               // � ���������� ������� �� �������, ��������������� ����������� � ������ �������� ��������� :
        {
            Display::ShowWarning(Warning::EnabledPeakDet);  // ������� ��������� �� ����
            return;                                         // � �������
        }

        ::Math::LimitationDecrease<uint8>((uint8 *)(&SET_TBASE), 0); // -V206
    }

    TBase::Load();
    FPGA::Start();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::RShift::Change(Chan::E ch, int delta)
{
    ::Math::AdditionThisLimitation<uint16>(&SET_RSHIFT(ch), STEP_RSHIFT * delta, RShift::MIN, RShift::MAX);

    Load(ch);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::Trig::Level::Change(int delta)
{
    ::Math::AdditionThisLimitation<uint16>(&SET_TRIGLEV_SOURCE, STEP_TRIGLEV * delta, Trig::MIN, Trig::MAX);

    Trig::Level::Load();

    Trig::NeedForDraw(2000);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::RShift::Set(Chan::E ch, uint16 rShift)
{
    ::Math::Limitation<uint16>(&rShift, MIN, MAX);
    SET_RSHIFT(ch) = rShift;
    Load(ch);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::Trig::Polarity::Load()
{
    FPGA::GiveStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::SET::TShift::Min()
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


    if (SET_PEAKDET_DIS)
    {
        return m[(int)FPGA_ENUM_POINTS].m[TPOS];
    }

    return m[(int)FPGA_ENUM_POINTS].m[TPOS] * 2; // ��� �������� ��������� ��� �������� ������ ���� � ��� ���� ������

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::SET::TShift::Zero()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::SET::TShift::Max()
{
    return 60000;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::SET::TShift::InPoints()
{
    return SET_PEAKDET_EN ? value : (value * 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FPGA::SET::TShift::TShift(int tshift)
{
    value = tshift;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::TShift::Set(int tShift)
{
    LIMITATION(tShift, Min(), Max()); //-V2516

    value = tShift;
    SET_TSHIFT.value = tShift;

    TShift::Load();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String FPGA::SET::TShift::ToString(TBase::E tBase) const
{
    float time = FPGA::Math::TShift2Abs(value, tBase);
    return Time(time).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::RShift::Draw()
{
    Draw(Chan::A);
    Draw(Chan::B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::RShift::Draw(Chan::E ch)
{
    Color::SetCurrent(Color::Channel(ch));

    int delta = (SET_RSHIFT(ch) - ZERO) / STEP_RSHIFT;

    int y = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top() - delta;

    //Painter::DrawChar(Grid::Left() - 8, y - 4, (char)SYMBOL_RSHIFT_NORMAL);
    Char((char)SYMBOL_RSHIFT_NORMAL).Draw(Grid::Left() - 8, y - 4);

    Font::SetCurrent(Font::Type::_5);

    //Painter::DrawChar(Grid::Left() - 7, y - 6, Chan(ch).IsA() ? '1' : '2', Color::BACK);
    Char(Chan(ch).IsA() ? '1' : '2').Draw(Grid::Left() - 7, y - 6, Color::BACK);

    Font::SetCurrent(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Osci::Settings::RShift::ToString(uint16 rShiftRel, Osci::Settings::Range::E range, Divider::E divider)
{
    float rShiftVal = FPGA::Math::RShift2Abs(rShiftRel, range) * Divider(divider).ToAbs();
    return Voltage(rShiftVal).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Osci::Settings::Trig::SyncPulse()
{
    return pulse;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::Trig::DrawOnGrid()
{
    if (needDraw)
    {
        int width = 85;
        int height = 18;

        int x = (Grid::Right() - Grid::Left()) / 2 + Grid::Left() - width / 2;
        int y = Grid::Bottom() - height - 20;

        //Painter::FillBoundedRegion(x, y, width, height, Color::BACK, Color::FILL);
        Region(width, height).DrawBounded(x, y, Color::BACK, Color::FILL);

        float trigLevVal = FPGA::Math::RShift2Abs(SET_TRIGLEV_SOURCE, SET_RANGE(TRIG_SOURCE)) * Divider(SET_DIVIDER(TRIG_SOURCE)).ToAbs();

        Voltage voltage(trigLevVal);

        String("����� %s", voltage.ToString(true).CString()).Draw(x + 7, y + 5, Color::FILL);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::Trig::NeedForDraw(uint timeMS)
{
    needDraw = true;
    Timer::SetAndStartOnce(Timer::Type::ShowLevelTrigLev, DisableDrawing, timeMS);
    NEED_FINISH_DRAW = 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Osci::Settings::Trig::DisableDrawing()
{
    needDraw = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Osci::Settings::TBase::Name() const
{
    static pString names[TBase::Number][Language::Number] =
    {
        {"2��",     "2ns"},
        {"5��",     "5ns"},
        {"10��",    "10ns"},
        {"20��",    "20ns"},
        {"50��",    "50ns"},
        {"0.1���",  "0.1us"},
        {"0.2���",  "0.2us"},
        {"0.5���",  "0.5us"},
        {"1���",    "1us"},
        {"2���",    "2us"},
        {"5���",    "5us"},
        {"10���",   "10us"},
        {"20���",   "20us"},
        {"50���",   "50us"},
        {"0.1��",   "0.1ms"},
        {"0.2��",   "0.2ms"},
        {"0.5��",   "0.5ms"},
        {"1��",     "1ms"},
        {"2��",     "2ms"},
        {"5��",     "5ms"},
        {"10��",    "10ms"},
        {"20��",    "20ms"},
        {"50��",    "50ms"},
        {"0.1�",    "0.1s"},
        {"0.2�",    "0.2s"},
        {"0.5�",    "0.5s"},
        {"1�",      "1s"},
        {"2�",      "2s"},
        {"5�",      "5s"},
        {"10�",     "10s"}
    };

    return names[value][LANG];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Osci::Settings::Range::Name() const
{
    static const struct StructRange
    {
        const char * names[Language::Number];
        StructRange(pString nRU, pString nEN)
        {
            names[Language::RU] = nRU;
            names[Language::EN] = nEN;
        }
    }
    names[Range::Number] =
    {
        StructRange("2��",  "2mV"),
        StructRange("5��",  "5mV"),
        StructRange("10��", "10mV"),
        StructRange("20��", "20mV"),
        StructRange("50��", "50mV"),
        StructRange("0.1�", "0.1V"),
        StructRange("0.2�", "0.2V"),
        StructRange("0.5�", "0.5V"),
        StructRange("1�",   "1V"),
        StructRange("2�",   "2V"),
        StructRange("5�",   "5V"),
        StructRange("10�",  "10V"),
        StructRange("20�",  "20V")
    };

    return names[value].names[LANG];
};
