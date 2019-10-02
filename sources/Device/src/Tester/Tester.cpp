#include "defines.h"
#include "Transceiver.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


using HAL::PIO::State;


static Settings oldSet = Settings::defaultSettings;

uint16 Tester::Pin_TEST_ON = HAL::PIO::Pin::_13;
uint16 Tester::Pin_PNP = HAL::PIO::Pin::_14;
uint16 Tester::Pin_U = HAL::PIO::Pin::_15;
uint16 Tester::Pin_I = HAL::PIO::Pin::_0;
uint16 Tester::Pin_TEST_STR = HAL::PIO::Pin::_9;

/// ��������� FPGA � ������������ � �������������� �����������
static void LoadFPGA();
/// ������� ������ ��������� ���������
static void ReadData();
/// ����������� ����� ��� ������� ���������
static void RecountPoints(uint16 *x, uint8 *y);
/// ������� ���
static int step = 0;
/// ��� ��������� ����������
static float stepU = 0.0F;
/// ������������� � true �������� ��������, ��� ������ ����� �������
static bool enabled = false;

static uint16 dataX[Tester::NUM_STEPS][TESTER_NUM_POINTS];  /// \todo ������� ���, ����� ��� ��������� ������-���������� ����������� ������ ������� �� Heap.cpp
static uint8  dataY[Tester::NUM_STEPS][TESTER_NUM_POINTS];



namespace Tester
{
    class DAC2_
    {
    public:
        static void Init()
        {
            HAL::DAC2_::Init();
        }
        static void SetValue(uint value)
        {
            HAL::DAC2_::SetValue(value);
        }
    };
}



void Tester::Init()
{
    for (int i = 0; i < Chan::Size; i++)
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

    HAL::PIO::Init(HAL::PIO::Port::_A, HAL::PIO::Pin::_5, HAL::PIO::Mode::Analog, HAL::PIO::Pull::No);    // ����������� �������� ����

    //                         TEST_ON               PNP               U
    uint pins = (uint)(Tester::Pin_TEST_ON | Tester::Pin_PNP | Tester::Pin_U);
    HAL::PIO::Init(Port_TEST_ON, pins, HAL::PIO::Mode::Output_PP, HAL::PIO::Pull::Down);

    //                               I
    HAL::PIO::Init(Port_I, Tester::Pin_I, HAL::PIO::Mode::Output_PP, HAL::PIO::Pull::Down);

    //              TEST_STR - EXTI9
    HAL::PIO::Init(Port_TEST_STR, Tester::Pin_TEST_STR, HAL::PIO::Mode::RisingIT, HAL::PIO::Pull::No);

    HAL::PIO::Set(Port_TEST_ON, Tester::Pin_TEST_ON);         // ��������� ������-��������

    DAC2_::Init();

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

    set.ch[Chan::A].couple = set.ch[Chan::B].couple = ModeCouple::GND;

    set.ch[Chan::A].range = Range::_2V;
    set.ch[Chan::B].range = Range::_2V;

    Range::LoadBoth();

    RShift::Set(Chan::A, RShift::ZERO);
    RShift::Set(Chan::B, RShift::ZERO);

    HAL::PIO::Reset(Port_TEST_ON, Pin_TEST_ON);       // �������� ������-���������

    LoadFPGA();

    Osci::Stop();

    step = 0;                               // ������ ��������� ������� ����� �������� ������� ���

    LoadPolarity();

    LoadStep();

    HAL::NVIC_::EnableIRQ(HAL::NVIC_::irqEXTI9_5);    // �������� ���������� �� �������� ���������
}


int Tester::DeltaRShiftA()
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

    HAL::NVIC_::DisableIRQ(HAL::NVIC_::irqEXTI9_5);      // ��������� ���������� �� �������� ���������

    HAL::PIO::Set(Port_TEST_ON, Pin_TEST_ON);

    oldSet.test.control = set.test.control;
    oldSet.test.polarity = set.test.polarity;
    oldSet.test.stepU = set.test.stepU;
    oldSet.test.stepI = set.test.stepI;

    set = oldSet;
    //set.Save();

    Osci::Init();

    FPGA::OnPressStart();
}


void Tester::StartStop()
{
    if (enabled)
    {
        HAL::NVIC_::DisableIRQ(HAL::NVIC_::irqEXTI9_5);      // ��������� ���������� �� �������� ���������
        enabled = false;
    }
    else
    {
        HAL::NVIC_::EnableIRQ(HAL::NVIC_::irqEXTI9_5);    // �������� ���������� �� �������� ���������
        enabled = true;
    }
}


static void LoadFPGA()
{

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


    if (Transceiver::InInteraction() || 
        Timer::IsBusy())
    {
        return;
    }

    if ((step % 2) == 0)        // ���� ��� ������ ����, �� ����� ������������� ����������
    {
        DAC2_::SetValue((uint)(stepU * step / 2));
        // ��������� ���� ��� ������ ������������ ���������� �����. ����� ����� ������������� � ������� 2.5 �� (������������� ������ �����)
        if (!FPGA::ForTester::Start())
        {
            return;
        }
    }
    else
    {
        ReadData();
    }

    ++step;

    if (step == 10)
    {
        step = 0;
    }
}


static void ReadData()
{
    int halfStep = step / 2;

    uint16 *x = &dataX[halfStep][0];
    uint8 *y = &dataY[halfStep][0];

    if(FPGA::ForTester::Read(x, y))
    {
        RecountPoints(x, y);

        Tester::Display::SetPoints(halfStep, x, y);
    }
}


static void RecountPoints(uint16 *x, uint8 *y)
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
        X = (int)(x0 + (X - x0) * scaleX);
        LIMITATION(X, 0, 319); //-V2516
        x[i] = (uint16)X;

        int Y = y[i] + dY;
        Y = (uint8)(y0 + (Y - y0) * scaleY);
        LIMITATION(Y, 0, 239); //-V2516
        y[i] = (uint8)Y;
    }
}


void Tester::LoadPolarity()
{
    // ������������� ����������
    HAL::PIO::Write(Port_PNP, Pin_PNP, (set.test.polarity == TesterPolarity::Positive) ? State::Enabled : State::Disabled);
}


void Tester::LoadStep()
{
    // ������������� ���������� ����������� ��� �����
    HAL::PIO::Write(Port_U, Pin_U, (set.test.control == TesterControl::Voltage) ? State::Enabled : State::Disabled);

    HAL::PIO::Write(Port_I, Pin_I, (set.test.control == TesterControl::Voltage) ? State::Disabled : State::Enabled);

    if (set.test.control == TesterControl::Voltage)
    {
        stepU =  255.0F / 3 * ((set.test.stepU == TesterStepU::_500mV) ? 2 : 0.4F) / 5;
    }
    else
    {
        stepU = 255.0F / 3 * ((set.test.stepI == TesterStepI::_20mA) ? 2 : 0.4F) / 5;
    }
}


void HAL_GPIO_EXTI_Callback(uint16 pin)
{
    if (pin == HAL::PIO::Pin::_9)      // ���������� �� ������-����������
    {
        Tester::ProcessStep();
    }
}



pString Tester::Scale::ToString() const // -V2506
{
    if (Chan(ch).IsA())
    {
        return Range((Range::E)value).ToString(Divider::_1);
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
    else
    {
        LOG_ERROR("������ �� ��������");
    }

    return 0;
}


String Tester::Shift::ToString(Scale::E scale) // -V2506
{
    if (ch == Chan::A)
    {
        return RShift::ToString(shift, (Range::E)scale, Divider::_1);
    }

    float shiftAbs = FPGA::Math::RShift2Abs(shift, (Range::E)scale) * 1e-3F;

    return Current(shiftAbs).ToString();
}
