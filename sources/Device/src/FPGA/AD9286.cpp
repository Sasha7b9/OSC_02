#include "defines.h"
#include "AD9286.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL_PIO.h"


using namespace Hardware;

using HAL::PIO::Mode;
using HAL::PIO::Pull;




#define PIN_SCK     HAL::PIO::Pin::_10
#define PORT_SCK    HPort::_B

#define PIN_DAT     HAL::PIO::Pin::_3
#define PORT_DAT    HPort::_C

#define PIN_CS      HAL::PIO::Pin::_11
#define PORT_CS     HPort::_E

#define SCK         PORT_SCK, PIN_SCK
#define DAT         PORT_DAT, PIN_DAT
#define CS          PORT_CS,  PIN_CS



static void  WriteByte(uint8 address, uint8 byte);



void AD9286::Init()
{
    /*
        ��� ���������� ������������ �����
        � ��� ��� ������ � ������ ������������ �� ��, ������, ������� � ���������� ��������� ����������� EEPROM AT25160

        CS  - 64 - PE11
        DAT - 29 - PC3
        SCK - 69 - PB10
    */
   
    HAL::PIO::Init(PORT_CS, PIN_CS, Mode::Output_PP, Pull::Up);       // ������������� CS

    HAL::PIO::Init(PORT_DAT, PIN_DAT, Mode::Output_PP, Pull::Up);     // ������������� DAT
    
    HAL::PIO::Init(PORT_SCK, PIN_SCK, Mode::Output_PP, Pull::Up);     // ������������� SCK

    HAL::PIO::Set(CS);
    HAL::PIO::Reset(DAT);
    HAL::PIO::Reset(SCK);

    Tune();
}


void AD9286::Tune()
{
    WriteByte(0x09, 0x09);
    /// \todo ��� ������ ���� � ������� ������
    WriteByte(0xff, 0x01);   // ����� ��� �������������.
}


static void WriteByte(uint8 address, uint8 byte)
{
    HAL::PIO::Reset(CS);

    uint value = (uint)((address << 8) + byte);

    for (int i = 23; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            HAL::PIO::Set(DAT);
        }
        else
        {
            HAL::PIO::Reset(DAT);
        }
        PAUSE_ON_TICKS(100);

        HAL::PIO::Set(SCK);
        PAUSE_ON_TICKS(100);

        HAL::PIO::Reset(SCK);
    }

    HAL::PIO::Set(CS);
}
