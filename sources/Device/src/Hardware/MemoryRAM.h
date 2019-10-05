#pragma once


struct MemoryRAM
{
    /// ���������� ������� ������ �� ���������
    static void ResetSignal();
    /// ���������� ������� ��������� ������
    static void SelectNextSignal();
    /// ���������� ������� ���������� ������
    static void SelectPrevSignal();
    /// ������� ����� ������� � ������ ���������
    static int16 CurrentSignal();
};
