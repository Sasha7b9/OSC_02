#include "defines.h"
#include "FPGA/FPGA.h"


/// ����� ������ ���������� ������ � ���������� ������
static uint timePrevReadP2P = 0;


void FPGA::ReadFlag()
{
    flag.flag = 0xFFFF;
}
