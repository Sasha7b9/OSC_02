#pragma once


namespace Osci
{
    namespace Display
    {
        class Accumulator
        {
        public:
            /// ��� ������� ����� �������� ����� ������ ��������� ��������
            static void NextFrame();
            /// ����� ����������
            static void Reset();
        };
    }
}
