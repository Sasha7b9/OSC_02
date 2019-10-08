#include "defines.h"
#include "FPGA/ContextTester.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"


void ContextTester::OnPressStart()
{
    FPGA::OnPressStart();
}

bool ContextTester::Start()
{
    // � ��� ���������� �������. �� ���������� ������� ������ ����������� �� ����� 2.5 ��
// 2.5�� / 12��� = 0.2 ��/��� = 10���/���

    set.time.base = TBase::_500us;

    TBase::Load();

    HAL_FSMC::WriteToFPGA16(WR::POST_LO, (uint16)(~(400)));
    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, (uint16)(~(1)));
    HAL_FSMC::WriteToFPGA8(WR::START, 0xff);

    FPGA::GiveStart();

    return true;
}

bool ContextTester::Read(uint16 *dataA, uint8 *dataB)
{
    uint start = TIME_MS;
    FPGA::flag.flag = 0;
    while (!FPGA::flag.DataReady())    // ��� ����� ���������� ������
    {
        FPGA::ReadFlag();

        if (TIME_MS - start > 20)        /// \todo ��������� �������. ���� ������� ���, ����� ����� �������� ���� ���������. ���� ����������, �� �����
        {                               /// ������� ������
            return false;
        }
    }

    uint16 aRead = (uint16)(FPGA::ReadLastRecord(Chan::A) - TESTER_NUM_POINTS);

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // ��������� �����, � �������� ����� ������ ������
    HAL_FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // � ��� ������� ����, ����� ������ ���������� � ����

    uint8 *addrA = RD::DATA_A; // -V566
    addrA++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataA++ = *addrA;
    }

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // ��������� �����, � ��������� ����� ������ ������
    HAL_FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // � ��� ������� ����, ����� ������ ���������� � ����

    uint8 *addrB = RD::DATA_B; // -V566
    addrB++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataB++ = *addrB;
    }

    return true;
}