#include "defines.h"
#include "Multimeter.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Multimeter
{
    class USART3_
    {
    public:
        static void Init(pFuncVV recvCallback)
        {
            HAL::USART3_::Init(recvCallback);
        }
        static void Transmit(void *_buffer, uint timeout)
        {
            uint8 *pointer = (uint8 *)_buffer;

            uint size = 0;
            while (*pointer != 0x0a)
            {
                size++;
                pointer++;
            }

            HAL::USART3_::Transmit(_buffer, size + 1, timeout);
        }
        static void StartReceiveIT(void *_buffer)
        {
            HAL::USART3_::StartReceiveIT(_buffer, 13);
        }
    };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// � ���� ����� ������������ ���������� � ����������� ���������� �����
static uint8 bufferUART[20];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ReceiveCallback();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multimeter::ChangeMode()
{
    Display::ChangedMode();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Init()
{
    USART3_::Init(ReceiveCallback);

    uint8 send[] = { 0x02, 'V', '0', 0x0a };

    USART3_::Transmit(send, 10);

    USART3_::StartReceiveIT(bufferUART);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeAVP()
{
    ChangeMode();

    char send[] = { 0x02, 'Z', (MULTI_AVP == MultimeterAVP::On) ? '1' : '0', 0x0a };

    USART3_::Transmit(send, 100);

    USART3_::StartReceiveIT(bufferUART);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Calibrate(int calibr)
{
    char send[] = { 0x02, 'K', 'A', 'L', 'I', 'B', (calibr == 0) ? '0' : '1', 0x0a };
    
    USART3_::Transmit(send, 100);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Update()
{
    if (!Device::State::InModeMultimeter())
    {
        return;
    }
    
    uint8 range = 0;
    if(set.mult.meas == Measure::VoltageDC)        { range = (uint8)MULTI_RANGE_VOLTAGE_DC; }
    else if(set.mult.meas == Measure::VoltageAC)   { range = (uint8)MULTI_RANGE_VOLTAGE_AC; }
    else if(set.mult.meas == Measure::CurrentDC)   { range = (uint8)MULTI_RANGE_CURRENT_DC; }
    else if(set.mult.meas == Measure::CurrentAC)   { range = (uint8)MULTI_RANGE_CURRENT_AC; }
    else if(set.mult.meas == Measure::Resistance)  { range = (uint8)MULTI_RANGE_RESISTANCE; }
    else
    {
        // ������ ������� ����
    }

    char symbol = Measure(set.mult.meas).Symbol();

    uint8 send[] = {0x02, (uint8)symbol, (uint8)(range + 0x30), 0x0a};

    USART3_::Transmit(send, 100);

    USART3_::StartReceiveIT(bufferUART);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Multimeter::Measure::E Multimeter::Measure::GetCode(const char buffer[13])
{
    Measure::E result = Measure::Size;

    int pos = 0;

    while (pos < 13 && buffer[pos] != 0x0a)
    {
        ++pos;
    }

    if (pos == 13)
    {
        return Measure::Size;
    }

    switch (buffer[pos - 2])
    {
    case 'U':
        result = Measure::VoltageDC;
        break;
    case 'V':
        result = Measure::VoltageAC;
        break;
    case 'I':
        result = Measure::CurrentDC;
        break;
    case 'J':
        result = Measure::CurrentAC;
        break;
    case 'R':
        result = Measure::Resistance;
        break;
    case 'Y':
        result = Measure::TestDiode;
        break;
    case 'W':
        result = Measure::Bell;
        break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReceiveCallback()
{
    Multimeter::Display::SetMeasure(bufferUART);
    Multimeter::USART3_::StartReceiveIT(bufferUART);
}
