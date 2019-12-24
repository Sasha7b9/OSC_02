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
    StretchADC::SetReal();
    ShiftADC::SetReal();

    if (!Calibrate(Chan::A))
    {
        Display::Message::ShowAndWaitKey("���������� ������ 1 �� ������", true);

        return BadExit();
    }
    else if (!Calibrate(Chan::B))
    {
        Display::Message::ShowAndWaitKey("���������� ������ 2 �� ������", true);

        return BadExit();
    }
    else
    {
        // ������ ������ ������ �� �����
    }

    Display::Message::ShowAndWaitKey("���������� ������� ���������", true);

    NormalExit();
}


bool Calibrator::Calibrate(Chan::E ch)
{
    Display::Message::ShowAndWaitKey(ch == Chan::A ?  "���������� ���������� ���������� �� ����� 1 � ������� ����� ������" :
                                                      "���������� ���������� ���������� �� ����� 2 � ������� ����� ������", true);

    Display::Message::Show(ch == Chan::A ? "�������� ����� 1" : "�������� ����� 2", true);

    bool result = Balance(ch, false) && Stretch(ch);

    Display::Message::Hide();

    return result;
}


bool Calibrator::Balance(Chan::E ch, bool showHint)
{
    Settings old = set;

    ShiftADC::SetDisabled();

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
        Balance(ch, static_cast<Range::E>(range));
    }

    std::memcpy(&old.dbg.nrst.shiftADC.shift[ch][0], &ShiftADC::Ref().shift[ch][0], sizeof(ShiftADC::Ref().shift[ch][0]) * Range::Count);

    set = old;

    Osci::Init();

    if (showHint)
    {
        Display::Message::Hide();
    }

    return true;
}


void Calibrator::Balance(Chan::E ch, Range::E range)
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
        if (!Transceiver::InInteraction())
        {
            if (HAL_PIO::Read(PIN_P2P))
            {
                HAL_FSMC::SetAddrData(addr);
                sum += HAL_FSMC::ReadData0();
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0.0F)
    {
        ShiftADC::Set(ch, range, static_cast<int8>(delta * 200.0F / 125.0F + 0.5F));
    }
    else
    {
        ShiftADC::Set(ch, range, static_cast<int8>(delta * 200.0F / 125.0F - 0.5F));
    }
}


float Calibrator::FindStretchK(Chan::E ch)
{
    Osci::Stop();

    float sumMIN = 0.0F;
    float sumMAX = 0.0F;

    int numMIN = 0;
    int numMAX = 0;

    uint8 *addr = ((ch == Chan::A) ? RD::DATA_A : RD::DATA_B) + 1;

    for (int i = 0; i < 300; i++)
    {
        if (!Transceiver::InInteraction())
        {
            HAL_FSMC::SetAddrData(addr);
            uint8 d = HAL_FSMC::ReadData0();

            if (d > VALUE::MAX - 32)
            {
                sumMAX += d;
                numMAX++;
            }
            else if (d < VALUE::MIN + 32)
            {
                sumMIN += d;
                numMIN++;
            }
            else
            {
                return -1.0F;
            }
        }
    }

    static const float pointsInPixel = (VALUE::MAX - VALUE::MIN) / 200.0F;  // ������� �������� �� ������ �������� ���� ����� ������� �� ���������

    float patternMIN = VALUE::MIN + pointsInPixel * 20;
    float patternMAX = VALUE::MAX - pointsInPixel * 20;
    float patternDELTA = patternMAX - patternMIN;

    float min = sumMIN / numMIN;
    float max = sumMAX / numMAX;
    float delta = max - min;

    return (patternDELTA / delta);
}


bool Calibrator::Stretch(Chan::E ch)
{
    Settings old = set;

    ShiftADC::SetReal();
    StretchADC::SetDisabled();

    ModeCouple(ch).SetAC();
    RShift(ch).Set(0);
    Range(ch).Set500mV();
    TBase::Set200us();
    TShift::Set(0);
    TrigSource::Set(ch);
    TrigLevel(ch).Set(0);

    float k = FindStretchK(ch);

    if (k > 0.0F)
    {
        old.dbg.nrst.stretchADC.stretch[ch] = k;
    }

    set = old;

    Osci::Init();

    return (k > 0.0F);
}


Calibrator::Mode::E &Calibrator::Mode::Ref()
{
    return set.serv.calibratorMode;
}


void Calibrator::NormalExit()
{
    StretchADC::SetReal();
    ShiftADC::SetReal();
}


void Calibrator::BadExit()
{
    StretchADC::SetDisabled();
    ShiftADC::SetDisabled();
}
