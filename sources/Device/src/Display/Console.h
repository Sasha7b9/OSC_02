#pragma once


struct Console
{
    static void Draw();

    static void AddString(char *string);

    static int NumberOfLines();
    /// ���������� true, ���� ������� ������ ������������ �� ������
    static bool IsShown();
    /// ��� ������� ���� �������, ����� ������������ �������� ������������ ���������� ����� � �������
    static void OnChanged_MaxStringsInConsole();
};

