#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/DataSettings.h"


/*
    ��������  | ������������� ������
    �� ������ |
       0      | ���������� 1-�� ������
       16k    | ���������� 2-�� ������
       32k    |
*/


static const uint SIZE_BUFFER = 112 * 1024;
static uint8 buffer[SIZE_BUFFER];

static const uint SIZE_STORAGE_RAM = SIZE_BUFFER;


uint8 *IntRAM::Begin()
{
    return buffer;
}


uint8 *IntRAM::End()
{
    return Begin() + SIZE_STORAGE_RAM;
}


uint16 *IntRAM::Averager16k(Chan::E ch)
{
    uint8 *result = (ch == Chan::A) ? Begin() : (Begin() + 16 * 1024);

    return reinterpret_cast<uint16 *>(result);
}
