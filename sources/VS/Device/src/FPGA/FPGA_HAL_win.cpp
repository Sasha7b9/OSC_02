#include "defines.h"
#include "FPGA/FPGA.h"


/// ����� ������ ���������� ������ � ���������� ������
static uint timePrevReadP2P = 0;


extern uint16 flagFPGA;


void FPGA::ReadFlag()
{
    flagFPGA = 0xFFFF;
}
