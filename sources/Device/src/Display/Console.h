#pragma once



namespace Console
{
    void Draw();

    void AddString(char *string);

    int NumberOfLines();
    /// ���������� true, ���� ������� ������ ������������ �� ������
    bool IsShown();
    /// ��� ������� ���� �������, ����� ������������ �������� ������������ ���������� ����� � �������
    void OnChanged_MaxStringsInConsole();
};

