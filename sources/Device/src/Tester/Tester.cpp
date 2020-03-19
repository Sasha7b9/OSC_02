#include "defines.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


static Settings oldSet = Settings::defaultSettings;


int Tester::step = 0;
float Tester::stepU = 0.0F;
bool Tester::enabled = false;

static uint16 dataX[Tester::NUM_STEPS][TESTER_NUM_POINTS];  /// \todo ������� ���, ����� ��� ��������� ������-���������� ����������� ������ ������� �� Heap.cpp
static uint8  dataY[Tester::NUM_STEPS][TESTER_NUM_POINTS];

// ������ ������ � ����
static void ReadFPGA(uint16 *dataA, uint8 *dataB);

// ��������� ���� ������ ��� ������-����������. � ������� time ������ ������ ���� ������� numPoints �����
// ���� ���������� false - ����� �� ������
static void StartFPGA();


void Tester::Init()
{
    for (int i = 0; i < Chan::Count; i++)
    {
        for (int j = 0; j < NUM_STEPS; j++)
        {
            for (int k = 0; k < TESTER_NUM_POINTS; k++)
            {
                dataX[j][k] = 0;
                dataY[j][k] = 0;
            }
        }
    }

    HAL_PIO::Init(PIN_TESTER_DAC, HMode::Analog, HPull::No);
    HAL_PIO::Init(PIN_TESTER_PNP, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_ON, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_I, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_U, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_STR, HMode::RisingIT, HPull::No);

    HAL_PIO::Set(PIN_TESTER_ON);         // ��������� ������-��������

    HAL_DAC2::Init();

    Disable();
}


void Tester::DeInit()
{

}


void Tester::Update()
{
}


void Tester::Enable() // -V2506
{
    if(enabled)
    {
        return;
    }
    
    enabled = true;

    oldSet = set;

    ModeCouple(Chan::A).SetGND();
    ModeCouple(Chan::B).SetGND();

    Range(Chan::A).Set2V();
    Range(Chan::B).Set2V();

    RShift(Chan::A).Set(0);
    RShift(Chan::B).Set(0);

    HAL_PIO::Reset(PIN_TESTER_ON);       // �������� ������-���������

    Osci::Stop();

    step = 0;                               // ������ ��������� ������� ����� �������� ������� ���

    LoadPolarity();

    LoadStep();

    HAL_NVIC::EnableIRQ(HAL_NVIC::irqEXTI9_5);    // �������� ���������� �� �������� ���������
}


int16 Tester::DeltaRShiftA()
{
    return 18;
}


void Tester::Disable() // -V2506
{
    if(!enabled)
    {
        return;
    }

    enabled = false;

    HAL_NVIC::DisableIRQ(HAL_NVIC::irqEXTI9_5);      // ��������� ���������� �� �������� ���������

    HAL_PIO::Set(PIN_TESTER_ON);

    oldSet.test.control = Control();
    oldSet.test.polarity = Polarity();
    oldSet.test.stepU = StepU();
    oldSet.test.stepI = StepI();

    set = oldSet;
    //set.Save();

    Osci::Init();

    FPGA::OnPressStart();
}


void Tester::StartStop()
{
    if (enabled)
    {
        HAL_NVIC::DisableIRQ(HAL_NVIC::irqEXTI9_5);      // ��������� ���������� �� �������� ���������
        enabled = false;
    }
    else
    {
        HAL_NVIC::EnableIRQ(HAL_NVIC::irqEXTI9_5);    // �������� ���������� �� �������� ���������
        enabled = true;
    }
}


void Tester::ProcessStep()
{
                                                                                                                                                /*
       |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |---
       |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
    ___|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|

       | step == 0 |  2.5 ��   | step == 2 |  2.5 ��   | step == 4 |  2.5 ��   | step == 6 |  2.5 ��   | step == 8 |  2.5 ��   |
       |     0V    |  ������   |   1 * dU  |  ������   |   2 * dU  |  ������   |   3 * dU  |  ������   |  4 * dU   |  ������   |
       |<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|                  */


    if (HAL_BUS::Panel::InInteraction() || Timer::IsBusy())
    {
        return;
    }

    if ((step % 2) == 0)        // ���� ��� ������ ����, �� ����� ������������� ����������
    {
        HAL_DAC2::SetValue(static_cast<uint>(stepU * step / 2));
        // ��������� ���� ��� ������ ������������ ���������� �����. ����� ����� ������������� � ������� 2.5 �� (������������� ������ �����)
        StartFPGA();
        FPGA::flag.flag = 0;
    }
    else
    {
        FPGA::ReadFlag();
        if(FPGA::flag.DataReady())
        {
            ReadData();
        }
        else
        {
            return;
        }
    }

    ++step;

    if (step == 10)
    {
        step = 0;
    }
}


void Tester::ReadData()
{
    int halfStep = step / 2;

    uint16 *x = &dataX[halfStep][0];
    uint8 *y = &dataY[halfStep][0];

    ReadFPGA(x, y);

    RecountPoints(x, y);

    DisplayTester::SetPoints(halfStep, x, y);
}


void Tester::RecountPoints(uint16 *x, uint8 *y)
{
    static const float scaleX = 332.0F / 240.0F;
    static const float scaleY = 249.0F / 255.0F;

    static const int dX = 20;
    static const int dY = -2;

    static const int x0 = 160;
    static const int y0 = 120;

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        int X = 255 - x[i] + dX;
        X = static_cast<int>(x0 + (X - x0) * scaleX);
        LIMITATION(X, 0, 319);
        x[i] = static_cast<uint16>(X);

        int Y = y[i] + dY;
        Y = static_cast<uint8>(y0 + (Y - y0) * scaleY);
        LIMITATION(Y, 0, 239);
        y[i] = static_cast<uint8>(Y);
    }
}


void Tester::LoadPolarity()
{
    // ������������� ����������
    HAL_PIO::Write(PIN_TESTER_PNP, Polarity::IsPositive() ? 1 : 0);
}


void Tester::LoadStep()
{
    // ������������� ���������� ����������� ��� �����
    HAL_PIO::Write(PIN_TESTER_U, Control::IsVoltage() ? 1 : 0);

    HAL_PIO::Write(PIN_TESTER_I, Control::IsVoltage() ? 0 : 1);

    if (Control::IsVoltage())
    {
        stepU =  255.0F / 3 * (StepU::Is500mV() ? 2 : 0.4F) / 5;
    }
    else
    {
        stepU = 255.0F / 3 * (StepI::Is20mA() ? 2 : 0.4F) / 5;
    }
}


pString Tester::Scale::ToString() const // -V2506
{
    if (Chan(ch).IsA())
    {
        return Range::ToString(static_cast<Range::E>(value), Divider::_1);
    }

    static const pString names[] =
    {
        "2���",
        "5���",
        "10���",
        "20���",
        "50���",
        "100���",
        "200���",
        "500���",
        "1��",
        "2��",
        "5��",
        "10��",
        "20��"
    };

    pString name = names[value];

    if (name)
    {
        return names[value];
    }

    return 0;
}


String Tester::Shift::ToString(Scale::E scale) // -V2506
{
    if (ch == Chan::A)
    {
        return RShift::ToString(shift, static_cast<Range::E>(scale), Divider::_1);
    }

    float shiftAbs = RShift::ToAbs(shift, static_cast<Range::E>(scale)) * 1e-3F;

    return Current(shiftAbs).ToString();
}


Tester::Control::E &Tester::Control::Ref()
{
    return set.test.control;
}


Tester::Polarity::E &Tester::Polarity::Ref()
{
    return set.test.polarity;
}


Tester::StepU::E &Tester::StepU::Ref()
{
    return set.test.stepU;
}


Tester::StepI::E &Tester::StepI::Ref()
{
    return set.test.stepI;
}


Tester::ViewMode::E &Tester::ViewMode::Ref()
{
    return set.test.viewMode;
}


ENumAverage::E &Tester::ENUMAverage::Ref()
{
    return set.test.enumAverage;
}


static void ReadFPGA(uint16 *dataA, uint8 *dataB)
{
    uint16 aRead = (uint16)(Osci::ReadLastRecord(Chan::A) - TESTER_NUM_POINTS);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, aRead);         // ��������� �����, � �������� ����� ������ ������
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);        // � ��� ������� ����, ����� ������ ���������� � ����

    HAL_BUS::FPGA::SetAddrData(RD::DATA_A + 1, RD::DATA_B + 1);

    for(int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataA++ = HAL_BUS::FPGA::ReadA0();
    }

    HAL_BUS::FPGA::Write16(WR::PRED_LO, aRead);         // ��������� �����, � ��������� ����� ������ ������
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);        // � ��� ������� ����, ����� ������ ���������� � ����

    for(int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataB++ = HAL_BUS::FPGA::ReadA1();
    }
}


static void StartFPGA()
{
    // � ��� ���������� �������. �� ���������� ������� ������ ����������� �� ����� 2.5 ��
    // 2.5�� / 12��� = 0.2 ��/��� = 10���/���

    TBase::Set500us();

    FPGA::GiveStart(static_cast<uint16>(~(400)), static_cast<uint16>(~(1)));

    FPGA::ForcedStart();
}
