#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory/IntRAM.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/DataSettings.h"


/*
    ��������  | ������������� ������
    �� ������ |
       0      | ���������� 1-�� ������
       16k    | ���������� 2-�� ������
       32k    | ������ ������������� 1-�� ������
       40k    | ������ ������������� 2-�� ������
       48k    |
*/


static const uint SIZE_BUFFER = 112 * 1024;
static uint8 buffer[SIZE_BUFFER];

static uint16 *const ave[2] = { reinterpret_cast<uint16 *>(buffer), reinterpret_cast<uint16 *>(buffer + 2 * FPGA::MAX_NUM_POINTS) };
static uint8 *const rand[2] = { buffer + 32 * 1024, buffer + 40 * 1024 };


uint16 *IntRAM::Averager16k(Chan::E ch)
{
    return ave[ch];
}


uint8 *IntRAM::ReadRand(Chan::E ch)
{
    return rand[ch];
}
