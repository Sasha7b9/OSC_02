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

    TBase::Set500us();

    FPGA::GiveStart(static_cast<uint16>(~(400)), static_cast<uint16>(~(1)));

    FPGA::ForcedStart();

    return true;
}

bool ContextTester::Read(uint16 *dataA, uint8 *dataB)
{
    uint start = TIME_MS;
    FPGA::flag.flag = 0;
    while (!FPGA::flag.DataReady())         // ��� ����� ���������� ������
    {
        FPGA::ReadFlag();

        if (TIME_MS - start > 20)           /// \todo ��������� �������. ���� ������� ���, ����� ����� �������� ���� ���������. ���� ����������, �� �����
        {                                   /// ������� ������
            return false;
        }
    }

    uint16 aRead = (uint16)(FPGA::ReadLastRecord(Chan::A) - TESTER_NUM_POINTS);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, aRead);         // ��������� �����, � �������� ����� ������ ������
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);        // � ��� ������� ����, ����� ������ ���������� � ����

    HAL_BUS::FPGA::SetAddrData(RD::DATA_A + 1, RD::DATA_B + 1);

    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataA++ = HAL_BUS::FPGA::ReadA0();
    }

    HAL_BUS::FPGA::Write16(WR::PRED_LO, aRead);         // ��������� �����, � ��������� ����� ������ ������
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);        // � ��� ������� ����, ����� ������ ���������� � ����

    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataB++ = HAL_BUS::FPGA::ReadA1();
    }

    return true;
}
