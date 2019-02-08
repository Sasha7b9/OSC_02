#include "defines.h"
#include "Multimeter.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include "Device.h"
#include <cstring>

#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;
using namespace Multimeter::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Multimeter::zero = 0;

/// ������� ������� � ������

namespace Multimeter
{
    static void SendCommand(char *data);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// � ���� ����� ������������ ���������� � ����������� ���������� �����
static char bufferUART[10];


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
        static void _Transmit(void *_buffer, uint size, uint timeout)
        {
            HAL::USART3_::Transmit(_buffer, size, timeout);
        }
        /// ��������� ���� ������ �����
        static void StartReceiveOneByte()
        {
            HAL::USART3_::StartReceiveIT(bufferUART, 1);
        }
    };
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Buffer
{
public:
    static void Push(char byte)
    {
        if (pointer == 0 && byte != 0x02)
        {
            return;
        }

        data[pointer++] = byte;

        if (pointer == 20)
        {
            pointer = 0;
        }

        if (ExistData())
        {
            data[pointer - 1] = 0;
            Multimeter::Display::SetMeasure(data);
            pointer = 0;
        }
    };

private:
    static int pointer;

    static char data[20];

    static bool ExistData()
    {
        if (pointer == 0)
        {
            return false;
        }

        if (data[pointer - 1] == 0x0a)
        {
            return true;
        }

        return false;
    }
};

int Buffer::pointer = 0;
char Buffer::data[20];


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

    char send[] = { 0x02, 'V', '0', 0x0a, 0 };

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeAVP()
{
    char send[] = { 0x02, 'Z', (MULTI_AVP == AVP::On) ? '1' : '0', 0x0a, 0 };

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Calibrate(int i)
{
    char send[] = { 0x02, 'K', 'A', 'L', 'I', 'B', (char)(i | 0x30), 0x0a, 0 };

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::SendCommand(char *data)
{
    ChangeMode();

    uint size = std::strlen(data);

    if (data[size - 1] != 0x0a)
    {
        LOG_ERROR("������������ ������");
    }

    USART3_::_Transmit(data, size, 100);

    USART3_::StartReceiveOneByte();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::LoadZero()
{
    char send[] = { 0x02, 'N', (zero | 0x30), 0x0a, 0};

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Update()
{
    if (!Device::State::InModeMultimeter())
    {
        return;
    }
    
    uint8 range = 0;
    if(MULTI_MEASURE == Measure::VoltageDC)        { range = (uint8)MULTI_RANGE_DC; }
    else if(MULTI_MEASURE == Measure::VoltageAC)   { range = (uint8)MULTI_RANGE_AC; }
    else if(MULTI_MEASURE == Measure::Resistance)  { range = (uint8)MULTI_RANGE_RESISTANCE; }
    else
    {
        // ������ ������� ����
    }

    char symbol = Measure(MULTI_MEASURE).Symbol();

    char send[] = {0x02, symbol, (char)(range + 0x30), 0x0a, 0};

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Multimeter::Measure::E Multimeter::Measure::ForSymbol(char symbol)
{
    Measure::E result = Measure::Size;

    switch (symbol)
    {
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
    Buffer::Push(bufferUART[0]);

    // � ��� ������ �� ������ ����������
    Multimeter::USART3_::StartReceiveOneByte();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Beep(bool /*on*/)
{

}
