#pragma once


namespace Recorder
{
    struct Storage
    {
        /// ��������� ������ ������������ - ������� ������ �����
        struct Data
        {
            /// ����� ����� � ��������������
            uint NumPoints();
        };
        /// �������������. ����������� ��� ��������� ������ "�����������"
        static void Init();
        /// ���������� �������� �����
        static void AddPoint(BitSet16 dataA, BitSet16 dataB);

        static Data &CurrentFrame();
    };
}
