#include "defines.h"
#include "device.h"
#include "Multimeter.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"


class USART3_
{
public:
    static void Init(pFuncVV recvCallback)
    {
        HAL_USART3::Init(recvCallback);
    }
    static void Transmit(void *_buffer, uint timeout)
    {
        uint8 *pointer = static_cast<uint8 *>(_buffer);

        uint size = 0;
        while (*pointer != 0x0a)
        {
            size++;
            pointer++;
        }

        HAL_USART3::Transmit(_buffer, size + 1, timeout);
    }
    static void StartReceiveIT(void *_buffer)
    {
        HAL_USART3::StartReceiveIT(_buffer, 13);
    }
};



/// � ���� ����� ������������ ���������� � ����������� ���������� �����
static uint8 bufferUART[20];



static void ReceiveCallback();



void Multimeter::ChangeMode()
{
    DisplayMultimeter::ChangedMode();
}


void Multimeter::Init()
{
    USART3_::Init(ReceiveCallback);

    uint8 send[] = { 0x02, 'V', '0', 0x0a };

    USART3_::Transmit(send, 10);

    USART3_::StartReceiveIT(bufferUART);
}


void Multimeter::DeInit()
{

}


void Multimeter::ChangeAVP()
{
    ChangeMode();

    char send[] = { 0x02, 'Z', AVP::IsEnabled() ? '1' : '0', 0x0a };

    USART3_::Transmit(send, 100);

    USART3_::StartReceiveIT(bufferUART);
}


void Multimeter::Calibrate(int calibr)
{
    char send[] = { 0x02, 'K', 'A', 'L', 'I', 'B', (calibr == 0) ? '0' : '1', 0x0a };
    
    USART3_::Transmit(send, 100);
}


void Multimeter::Update()
{
    if (!Device::InModeMultimeter())
    {
        return;
    }
    
    uint8 range = 0;
    if(Measure::IsVoltageDC())        { range = static_cast<uint8>(RangeDC()); }
    else if(Measure::IsVoltageAC())   { range = static_cast<uint8>(set.mult.rangeVoltageAC); }
    else if(Measure::IsCurrentDC())   { range = static_cast<uint8>(set.mult.rangeCurrentDC); }
    else if(Measure::IsCurrentAC())   { range = static_cast<uint8>(set.mult.rangeCurrentAC); }
    else if(Measure::IsResistance())  { range = static_cast<uint8>(set.mult.rangeResist); }
    else
    {
        // ������ ������� ����
    }

    char symbol = Measure::Symbol();

    uint8 send[] =
    {
        0x02,
        static_cast<uint8>(symbol),
        static_cast<uint8>(range + 0x30),
        0x0a
    };

    USART3_::Transmit(send, 100);

    USART3_::StartReceiveIT(bufferUART);
}


Multimeter::Measure::E Multimeter::Measure::GetCode(const char buffer[13])
{
    Measure::E result = Count;

    int pos = 0;

    while (pos < 13 && buffer[pos] != 0x0a)
    {
        ++pos;
    }

    if (pos == 13)
    {
        return Count;
    }

    switch (buffer[pos - 2])
    {
    case 'U':
        result = VoltageDC;
        break;
    case 'V':
        result = VoltageAC;
        break;
    case 'I':
        result = CurrentDC;
        break;
    case 'J':
        result = CurrentAC;
        break;
    case 'R':
        result = Resistance;
        break;
    case 'Y':
        result = TestDiode;
        break;
    case 'W':
        result = Bell;
        break;
    }

    return result;
}


static void ReceiveCallback()
{
    DisplayMultimeter::SetMeasure(bufferUART);
    USART3_::StartReceiveIT(bufferUART);
}


Multimeter::AVP::E &Multimeter::AVP::Ref()
{
    return set.mult.avp;
}


Multimeter::Measure::E &Multimeter::Measure::Ref()
{
    return set.mult.meas;
}


Multimeter::RangeDC::E &Multimeter::RangeDC::Ref()
{
    return set.mult.rangeVoltageDC;
}
