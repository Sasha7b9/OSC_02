#pragma once


class ExtRAM
{
public:
    /// ������ ���� ������� ������
    static uint8 *Begin();
    /// �� ��������� ������ ������� ������
    static uint8 *End();
    /// ���������� �� ������� ������ �� ������ address ������ buffer, size ������ 4
    static void Write(uint8 *buffer, uint size, uint8 *address);
};
