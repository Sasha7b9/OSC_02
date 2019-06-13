#pragma once


namespace Averager
{
    namespace Tester
    {
        /// ������ ���������� ���������. ���� �������� ���������� �� ����, ������� ��� ������� ��� ���������� ������, ���������� ���������.
        void SetCount(int enumAverage);

        void ProcessX(const uint16 *data, int step);

        void ProcessY(const uint8 *data, int step);

        uint16 *X();
        uint8 *Y();
    }
}
