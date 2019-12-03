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
private:
    static CHAR_BUF2(buffer, 33, 100);
    /// true ��������, ��� ��� ������� ������ ������� � ��������� � �� ����� ������ ������ (��� ����������, ����� ���������� ��� �� ����������)
    static bool inProcessDrawConsole;
    /// ���������� ����������� ����� � �������
    static int stringInConsole;
    /// ����� ����������� ���������� �������� ������������� ���������� ����� � �������
    static int16 prevMaxStrinsInConsole;

    static void DeleteFirstString();
};

