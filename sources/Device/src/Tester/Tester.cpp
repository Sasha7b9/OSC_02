#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>
#include "Hardware/Timer.h"
#include "Data/Reader.h"


using namespace FPGA::Settings;
using namespace Osci::Settings;
using namespace Tester::Settings;


using HAL::FSMC;
using HAL::PIO::State;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/// ������� ���
static int step = 0;
/// ��� ��������� ����������
static float stepU = 0.0F;
/// ������������� � true �������� ��������, ��� ������ ����� �������
static bool enabled = false;

static uint8 data[Chan::Size][Tester::NUM_STEPS][TESTER_NUM_POINTS];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Init()
{
    for (int i = 0; i < Chan::Size; i++)
    {
        for (int j = 0; j < NUM_STEPS; j++)
        {
            for (int k = 0; k < TESTER_NUM_POINTS; k++)
            {
                data[i][j][k] = 0;
            }
        }
    }

    DAC2_::Init();

    Disable();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Update()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Enable() // -V2506
{
    if(enabled)
    {
        return;
    }
    
    enabled = true;

    oldSet = set;

    SET_COUPLE_A = SET_COUPLE_B = ModeCouple::GND;

    SET_RANGE(Chan::A) = Osci::Settings::Range::_2V;
    SET_RANGE(Chan::B) = Osci::Settings::Range::_2V;

    Osci::Settings::Range::LoadBoth();

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Tester::DeltaRShiftA()
{
    return 18;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Disable() // -V2506
{
    if(!enabled)
    {
        return;
    }

    enabled = false;

    HAL::NVIC_::DisableIRQ(HAL::NVIC_::irqEXTI9_5);      // ��������� ���������� �� �������� ���������

    HAL::PIO::Set(Port_TEST_ON, Pin_TEST_ON);

    oldSet.test_control = TESTER_CONTROL;
    oldSet.test_polarity = TESTER_POLARITY;
    oldSet.test_stepU = TESTER_STEP_U;
    oldSet.test_stepI = TESTER_STEP_I;

    set = oldSet;
    //set.Save();

    Osci::Init();

    FPGA::OnPressStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void LoadFPGA()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::ProcessStep()
{
                                                                                                                                                  /*
       |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |---
       |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
    ___|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|

       | step == 0 |  2.5 ��   | step == 2 |  2.5 ��   | step == 4 |  2.5 ��   | step == 6 |  2.5 ��   | step == 8 |  2.5 ��   |
       |     0V    |  ������   |   1 * dU  |  ������   |   2 * dU  |  ������   |   3 * dU  |  ������   |  4 * dU   |  ������   |
       |<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|                  */

    if (FSMC::InterchangeWithPanel() || Timer::IsBusy())
    {
        return;
    }

    if ((step % 2) == 0)        // ���� ��� ������ ����, �� ����� ������������� ����������
    {
        DAC2_::SetValue((uint)(stepU * step / 2));
        // ��������� ���� ��� ������ ������������ ���������� �����. ����� ����� ������������� � ������� 2.5 �� (������������� ������ �����)
        FPGA::ForTester::Start();
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//static void AverageData(const uint8 *dataNew, uint16 *dataAve)
//{
//    uint8 *_new = (uint8 *)dataNew;
//    uint16 *av = dataAve;
//
//    uint16 numAve = 1;
//
//    for (int i = 0; i < TESTER_NUM_POINTS; i++)
//    {
//        av[i] = (uint16)(av[i] - (av[i] >> numAve));
//
//        av[i] += *_new;
//
//        *_new = (uint8)(av[i] >> numAve);
//
//        _new++;
//    }
//}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadData()
{
    int halfStep = step / 2;

    uint8 *x = &data[Chan::A][halfStep][0];
    uint8 *y = &data[Chan::B][halfStep][0];

//    uint16 *av[2][5] =
//    {
//        {AVE_1, AVE_1 + 240, AVE_1 + 240 * 2, AVE_1 + 240 * 3, AVE_1 + 240 * 4},
//        {AVE_2, AVE_2 + 240, AVE_2 + 240 * 2, AVE_2 + 240 * 3, AVE_2 + 240 * 4}
//    };

    if(FPGA::ForTester::Read(x, y))
    {
        //AverageData(x, av[0][halfStep]);
        //AverageData(y, av[1][halfStep]);

        Tester::Display::SetPoints(halfStep, x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadPolarity()
{
    // ������������� ����������
    HAL::PIO::Write(Port_PNP, Pin_PNP, TESTER_POLARITY_IS_POSITITVE ? State::Enabled : State::Disabled);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadStep()
{
    // ������������� ���������� ����������� ��� �����
    HAL::PIO::Write(Port_U, Pin_U, TESTER_CONTROL_IS_U ? State::Enabled : State::Disabled);

    HAL::PIO::Write(Port_I, Pin_I, TESTER_CONTROL_IS_U ? State::Disabled : State::Enabled);

    if (TESTER_CONTROL_IS_U)
    {
        stepU =  255.0F / 3 * ((TESTER_STEP_U == StepU::_500mV) ? 2 : 0.4F) / 5;
    }
    else
    {
        stepU = 255.0F / 3 * ((TESTER_STEP_I == StepI::_20mA) ? 2 : 0.4F) / 5;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16 pin)
{
    if (pin == HAL::PIO::Pin::_9)      // ���������� �� ������-����������
    {
        Tester::ProcessStep();
    }
}
