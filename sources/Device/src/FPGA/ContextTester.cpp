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

    TBase::Load(TBase::_500us);

    HAL_FSMC::WriteToFPGA16(WR::POST_LO, static_cast<uint16>(~(400)));
    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, static_cast<uint16>(~(1)));
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

    HAL_FSMC::SetAddrData(RD::DATA_A + 1, RD::DATA_B + 1);

    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataA++ = HAL_FSMC::ReadData0();
    }

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // ��������� �����, � ��������� ����� ������ ������
    HAL_FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // � ��� ������� ����, ����� ������ ���������� � ����

    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataB++ = HAL_FSMC::ReadData1();
    }

    return true;
}
