#pragma once
#include "Keyboard/Keyboard.h"



namespace BufferButtons
{
    /// ������� ���������� ��� �������, ������� � ���������� ������
    void Push(KeyEvent str);
    /// ��������� ������� �� �������
    KeyEvent Extract();
    /// ���������� true, ���� ������� ������� �����
    bool IsEmpty();
};
