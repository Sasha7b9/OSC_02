#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Console
{
public:
    static void Draw();

    static void AddString(char *string);

    static int NumberOfLines();
    /// ���������� true, ���� ������� ������ ������������ �� ������
    static bool IsShown();
};

