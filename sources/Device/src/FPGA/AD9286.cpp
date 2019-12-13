#include "defines.h"
#include "AD9286.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL_PIO.h"


#define PIN_DAT     HPin::_3
#define PORT_DAT    HPort::_C

#define PIN_CS      HPin::_11
#define PORT_CS     HPort::_E

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
   
    HAL_PIO::Init(PORT_CS, PIN_CS, HMode::Output_PP, HPull::Up);       // ������������� CS

    HAL_PIO::Init(PORT_DAT, PIN_DAT, HMode::Output_PP, HPull::Up);     // ������������� DAT
    
    HAL_PIO::Init(PORT_AD9286_SCK, HMode::Output_PP, HPull::Up);     // ������������� SCK

    HAL_PIO::Set(CS);
    HAL_PIO::Reset(DAT);
    HAL_PIO::Reset(PORT_AD9286_SCK);

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
    HAL_PIO::Reset(CS);

    uint value = static_cast<uint>((address << 8) + byte);

    for (int i = 23; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            HAL_PIO::Set(DAT);
        }
        else
        {
            HAL_PIO::Reset(DAT);
        }
        PAUSE_ON_TICKS(100);

        HAL_PIO::Set(PORT_AD9286_SCK);
        PAUSE_ON_TICKS(100);

        HAL_PIO::Reset(PORT_AD9286_SCK);
    }

    HAL_PIO::Set(CS);
}
