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
        static void AddPoint(uint16 dataA, uint16 dataB);

        static Data &CurrentFrame();
    };
}
