#include "defines.h"
#include "AD9286.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL_PORTS.h"


using namespace Hardware;

using HAL::PORTS::Mode;
using HAL::PORTS::Pull;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PIN_SCK     HAL::PORTS::Pin::_10
#define PORT_SCK    HAL::PORTS::Port::_B

#define PIN_DAT     HAL::PORTS::Pin::_3
#define PORT_DAT    HAL::PORTS::Port::_C

#define PIN_CS      HAL::PORTS::Pin::_11
#define PORT_CS     HAL::PORTS::Port::_E

#define SCK         PORT_SCK, PIN_SCK
#define DAT         PORT_DAT, PIN_DAT
#define CS          PORT_CS,  PIN_CS


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void  WriteByte(uint8 address, uint8 byte);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AD9286::Init()
{
    /*
        ��� ���������� ������������ �����
        � ��� ��� ������ � ������ ������������ �� ��, ������, ������� � ���������� ��������� ����������� EEPROM AT25160

        CS  - 64 - PE11
        DAT - 29 - PC3
        SCK - 69 - PB10
    */
   
    HAL::PORTS::Init(PORT_CS, PIN_CS, Mode::Output_PP, Pull::Up);       // ������������� CS

    HAL::PORTS::Init(PORT_DAT, PIN_DAT, Mode::Output_PP, Pull::Up);     // ������������� DAT
    
    HAL::PORTS::Init(PORT_SCK, PIN_SCK, Mode::Output_PP, Pull::Up);     // ������������� SCK

    HAL::PORTS::Set(CS);
    HAL::PORTS::Reset(DAT);
    HAL::PORTS::Reset(SCK);

    Tune();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::Tune()
{
    WriteByte(0x09, 0x09);
    /// \todo ��� ������ ���� � ������� ������
    WriteByte(0xff, 0x01);   // ����� ��� �������������.
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteByte(uint8 address, uint8 byte)
{
    HAL::PORTS::Reset(CS);

    uint value = (uint)((address << 8) + byte);

    for (int i = 23; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            HAL::PORTS::Set(DAT);
        }
        else
        {
            HAL::PORTS::Reset(DAT);
        }
        PAUSE_ON_TICKS(100);

        HAL::PORTS::Set(SCK);
        PAUSE_ON_TICKS(100);

        HAL::PORTS::Reset(SCK);
    }

    HAL::PORTS::Set(CS);
}
