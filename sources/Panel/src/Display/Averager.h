#pragma once


namespace Averager
{
    namespace Tester
    {
        /// ������ ���������� ���������. ���� �������� ���������� �� ����, ������� ��� ������� ��� ���������� ������, ���������� ���������.
        void SetCount(int enumAverage);

        void Process(const uint16 *x, const uint8 *y, int step);

        uint16 *X();
        uint8 *Y();
    }
}
