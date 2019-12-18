#pragma once


class Heap
{
public:
    /// ������ ��������� RAM, ��� �������� ��������� �������
    static uint8 *BeginStorageRAM();
    /// ����� �������� RAM, ��� �������� ��������� ������� (��������� �� ��������� ���� �� ����������)
    static uint8 *EndStorageRAM();
};
