#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Osci/Processing/Cursors.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������� ��������� ������ ������ �� ������ ������ ����.
void DrawSB_Exit(int x, int y);
/// ������������� ������� ��������� ������� ������ ������ �� ������ ������ ����.
void OnPressSB_Exit();

void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);

void DrawMenuCursTime(int x, int y, bool left, bool right);
/// ������������ ������� ��������� ��� ����� ������ ���������� ������� ���������.
void CalculateConditions(int16 pos0, int16 pos1, Osci::Measurements::Cursors::Control::E cursCntrl, bool *condTopLeft, bool *condBottomRight);
