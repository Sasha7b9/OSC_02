#pragma once


struct ContextTester
{
    static void OnPressStart();
    /// ��������� ���� ������ ��� ������-����������. � ������� time ������ ������ ���� ������� numPoints �����
    /// ���� ���������� false - ����� �� ������
    static bool Start();
    static bool Read(uint16 *dataA, uint8 *dataB);
};