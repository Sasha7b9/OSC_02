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
    /// ������ �� ������� ������ address � ����� buffer ������, size ������ 4
    static void Read(uint8 *buffer, uint size, uint8 *address);

    /// ���������� ������� ��������� ������ ��������� �����������.
    static float Test1();
    static float Test2();
    static bool Test3(int *num);
    /// ���������� ����� � ��������, ������� �������� ������/������ sizekB �������� �� ������� ������. ���������� -1, ���� ��������� ������
    static float TestTime(uint sizekB);
    /// ���� �������� ������/������ 1�� ������� RAM
    static float TestTime1kB(uint8 *address);
};
