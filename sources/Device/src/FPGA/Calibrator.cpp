#include "defines.h"
#include "common/Transceiver.h"
#include "FPGA/Calibrator.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include <cmath>
#include <cstring>


void Calibrator::Calibrate()
{
    if (!Calibrate(Chan::A))
    {
        Display::FuncOnWait::StartAndWaitKey("���������� ������ 1 �� ������", true);
    }
    else if (!Calibrate(Chan::B))
    {
        Display::FuncOnWait::StartAndWaitKey("���������� ������ 2 �� ������", true);
    }
    else
    {
        Display::FuncOnWait::StartAndWaitKey("���������� ������� ���������", true);
    }
}


bool Calibrator::Calibrate(Chan::E ch)
{
    Display::FuncOnWait::StartAndWaitKey(ch == Chan::A ?  "���������� ����������� ���������� �� ����� 1 � ������� ����� ������" :
                                                "���������� ����������� ���������� �� ����� 2 � ������� ����� ������", true);

    return Balance(ch) && Stretch(ch);
}


bool Calibrator::Balance(Chan::E ch)
{
    Settings old = set;

    static const pString messages[Chan::Count] =
    {
        "������������ ������ 1",
        "������������ ������ 2"
    };

    Display::FuncOnWait::Start(messages[ch], true);

    ModeCouple(ch, ModeCouple::GND);

    TBase::Load(TBase::_100ms);

    for (int range = 0; range < Range::Count; range++)
    {
        BalanceChannel(ch, static_cast<Range::E>(range));
    }

    std::memcpy(&old.dbg.nrst.shiftADC[ch][0], &set.dbg.nrst.shiftADC[ch][0], sizeof(set.dbg.nrst.shiftADC[ch][0]) * Range::Count);

    set = old;

    Osci::Init();

    Display::FuncOnWait::Stop();

    return true;
}


void Calibrator::BalanceChannel(Chan::E ch, Range::E range)
{
    Osci::Stop();

    Range(ch).Load(range);

    set.dbg.nrst.shiftADC[ch][range] = 0;

    RShift(ch, 0);

    Osci::Start();

    float sum = 0;

    int numPoints = 0;

    uint8 *address = (ch == Chan::A) ? RD::DATA_A : RD::DATA_B;
    address++;

    while (numPoints < 100)
    {
        if (!Transceiver::InInteraction())
        {
            if (HAL_PIO::Read(HPort::_G, HPin::_1))
            {
                sum += HAL_FSMC::ReadFromFPGA(address);
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0.0F)
    {
        set.dbg.nrst.shiftADC[ch][range] = static_cast<int8>(delta * 200.0F / 125.0F + 0.5F);
    }
    else
    {
        set.dbg.nrst.shiftADC[ch][range] = static_cast<int8>(delta * 200.0F / 125.0F - 0.5F);
    }
}


bool Calibrator::Stretch(Chan::E)
{
    return true;
}
