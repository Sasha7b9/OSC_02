#pragma once


namespace Osci
{
    namespace Display
    {
        class Accumulator
        {
        public:
            /// ��� ������� ����� �������� ����� ������ ��������� ��������
            static void NextDraw();
            /// ����� ����������
            static void Reset();
        };
    }
}
