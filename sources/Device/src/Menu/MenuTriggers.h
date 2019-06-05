#pragma once
#include "Keyboard/Keyboard.h"


/*
    � ���� ����� - �������� ���� - ��� �������-������� �� ������� ������� ����������
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������� - ����������� ���������� �������
class TriggerDebugMenu
{
public:
    /// ���������� true, ���� ������� �������� - ������� ���������
    static bool Triggered(KeyEvent &event);

private:
    static const int SIZE_BUFFER_FOR_BUTTONS = 10;
    static Key::E bufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
    static const Key::E sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
};
