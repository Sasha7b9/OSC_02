#pragma once


namespace Averager
{
    namespace Tester
    {
        /// ������ ���������� ���������. ���� �������� ���������� �� ����, ������� ��� ������� ��� ���������� ������, ���������� ���������.
        void SetCount(int enumAverage);

        void ProcessX(uint16 *data, int step);

        void ProcessY(uint8 *data, int step);
    }
}
