//
//  � ���� ����� ���������� ������� ��������� ������������
//

#include "defines.h"
#include "FPGA/FPGA.h"
#include "Recorder/Recorder_win.h"


bool RecorderHAL::ReadyPoint()
{
    static bool timeLastRead = 0;   // ����� ����������� ������ �����

    return true;
}


uint8 RecorderHAL::ReadData(Chan::E)
{
    return VALUE::AVE;
}
