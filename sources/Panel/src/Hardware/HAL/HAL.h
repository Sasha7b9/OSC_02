#pragma once


/// ������������ ��� ���������� �������� �������
struct HAL_DAC2
{
    static void Init();
    /// �������� value �� 0 �� 100
    static void SetValue(uint value);
};
