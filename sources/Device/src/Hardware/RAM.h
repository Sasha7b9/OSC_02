#pragma once



namespace Memory
{
    namespace RAM
    {
        /// ���������� ������� ������ �� ���������
        void ResetSignal();
        /// ���������� ������� ��������� ������
        void SelectNextSignal();
        /// ���������� ������� ���������� ������
        void SelectPrevSignal();
        /// ������� ����� ������� � ������ ���������
        int16 CurrentSignal();
    }
}
