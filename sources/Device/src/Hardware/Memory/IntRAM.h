#pragma once


class IntRAM
{
public:
    /// ������ ��������� RAM, ��� �������� ��������� �������
    static uint8 *BeginStorageRAM();
    /// ����� �������� RAM, ��� �������� ��������� ������� (��������� �� ��������� ���� �� ����������)
    static uint8 *EndStorageRAM();
};
