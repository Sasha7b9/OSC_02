#pragma once


struct MemoryRAM
{
    static int16 currentSignal;

    /// ���������� ������� ��������� ������
    static void SelectNextSignal();
    /// ���������� ������� ���������� ������
    static void SelectPrevSignal();
};
