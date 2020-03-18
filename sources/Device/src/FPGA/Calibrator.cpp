#include "defines.h"
#include "FPGA/Calibrator.h"
#include "FPGA/FPGA.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/SettingsNRST.h"
#include <cmath>
#include <cstring>


// ������������� ����� Ch
static bool CalibrateChannel(Chan::E ch);

// ������������� ����� �� ����� ���������
static void BalanceRange(Chan::E ch, Range::E range);

// "���������" �����
static bool StretchChannel(Chan::E ch);

// ����� ����������� �������� ������
static float FindStretchChannel(Chan::E ch);

static void NormalExit();

static void BadExit();


void Calibrator::Calibrate()
{
    ExtraStretch::SetTypeReal();
    ExtraShift::SetTypeReal();

    if (!CalibrateChannel(Chan::A))
    {
        Display::Message::ShowAndWaitKey("���������� ������ 1 �� ������", true);

        return BadExit();
    }
    else if (!CalibrateChannel(Chan::B))
    {
        Display::Message::ShowAndWaitKey("���������� ������ 2 �� ������", true);

        return BadExit();
    }

    Display::Message::ShowAndWaitKey("���������� ������� ���������", true);

    NormalExit();
}


static bool CalibrateChannel(Chan::E ch)
{
    Display::Message::ShowAndWaitKey(ch == Chan::A ?  "���������� ���������� ���������� �� ����� 1 � ������� ����� ������" :
                                                      "���������� ���������� ���������� �� ����� 2 � ������� ����� ������", true);

    Display::Message::Show(ch == Chan::A ? "�������� ����� 1" : "�������� ����� 2", true);

    bool result = Calibrator::BalanceChannel(ch, false) && StretchChannel(ch);

    Display::Message::Hide();

    return result;
}


bool Calibrator::BalanceChannel(Chan::E ch, bool showHint)
{
    SettingsNRST old = setNRST;

    ExtraShift::SetTypeDisabled();

    static const pString messages[Chan::Count] =
    {
        "���������� ����� 1",
        "���������� ����� 2"
    };

    if (showHint)
    {
        Display::Message::Show(messages[ch], true);
    }

    ModeCouple(ch).SetGND();

    TBase::Set100ms();

    for (int range = 0; range < Range::Count; range++)
    {
        BalanceRange(ch, static_cast<Range::E>(range));
    }

    std::memcpy(&old.exShift.value[ch][0], &setNRST.exShift.value[ch][0], sizeof(setNRST.exShift.value[ch][0]) * Range::Count);

    setNRST = old;

    Osci::Init();

    if (showHint)
    {
        Display::Message::Hide();
    }

    return true;
}


static void BalanceRange(Chan::E ch, Range::E range)
{
    Osci::Stop();

    Range(ch).Set(range);

    RShift(ch).Set(0);

    Osci::Start(false);

    float sum = 0;

    int numPoints = 0;

    uint8 *addr = ((ch == Chan::A) ? RD::DATA_A : RD::DATA_B) + 1;

    while (numPoints < 100)
    {
        if (!HAL_BUS::Panel::InInteraction())
        {
            if (HAL_PIO::Read(PIN_P2P))
            {
                HAL_BUS::FPGA::SetAddrData(addr);
                sum += HAL_BUS::FPGA::ReadA0();
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0.0F)
    {
        ExtraShift::SetValue(ch, range, static_cast<int8>(delta * 200.0F / 125.0F + 0.5F));
    }
    else
    {
        ExtraShift::SetValue(ch, range, static_cast<int8>(delta * 200.0F / 125.0F - 0.5F));
    }
}


static bool StretchChannel(Chan::E ch)
{
    SettingsNRST old = setNRST;

    ExtraShift::SetTypeReal();
    ExtraStretch::SetTypeDisabled();

    ModeCouple(ch).SetAC();
    RShift(ch).Set(0);
    Range(ch).Set500mV();
    TBase::Set200us();
    TShift::Set(0);
    TrigSource::Set(ch);
    TrigLevel(ch).Set(0);


    float k = FindStretchChannel(ch);

    if (k > 0.0F)
    {
        old.exStretch.value[ch] = k;
    }

    setNRST = old;

    Osci::Init();

    return (k > 0.0F);
}


static float FindStretchChannel(Chan::E ch)
{
#define NUM_POINTS 300
    
    Timer::PauseOnTime(500);

    FPGA::GiveStart(static_cast<uint16>(~(1)), static_cast<uint16>(~(NUM_POINTS + 100)));
    
    do 
    {
        FPGA::ReadFlag();

    } while (!FPGA::flag.DataReady());

    uint16 addrRead = static_cast<uint16>(Osci::ReadLastRecord(ch) - NUM_POINTS - 50);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, addrRead);
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);

    uint8 *a0 = Chan(ch).IsA() ? RD::DATA_A : RD::DATA_B;
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    Buffer buffer(NUM_POINTS);

    buffer.data[0] = HAL_BUS::FPGA::ReadA0();
    buffer.data[0] = HAL_BUS::FPGA::ReadA1();

    for(uint i = 0; i < NUM_POINTS; i++)
    {
        buffer.data[i] = HAL_BUS::FPGA::ReadA1();
    }

    uint8 *data = buffer.data;
    data = data;

    return 1.0F;
}


static void NormalExit()
{
    ExtraStretch::SetTypeReal();
    ExtraShift::SetTypeReal();
}


static void BadExit()
{
    ExtraStretch::SetTypeDisabled();
    ExtraShift::SetTypeDisabled();
}
