#pragma once
#include "common/Command.h"




typedef bool(*pFuncBU8)(uint8);

class Decoder
{
public:

    static void AddData(uint8 data);

    static void Update();
    /// ��� �������� ����� ��� ���������� ������ ��������
    static void SetBufferForScreenRow(uint8 *pixels);
private:

    static int pointer;

    static uint8 *pixels;
    /// ����������� �������
    static pFuncBU8 curFunc;

    static void RunStep(uint8 data);
    /// ��� ������� ���� �������� ����� ���������� ���������� ����
    static void FinishCommand();
    /// ��������� ��������� ������ � �������
    static bool AddToConsole(uint8);

    static bool FuncLengthText(uint8);

    static bool FuncScreen(uint8);

    static bool ButtonPress(uint8);
};
