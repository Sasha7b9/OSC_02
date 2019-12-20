#pragma once
#include "common/Command.h"




typedef bool(*pFuncBU8)(uint8);

class DDecoder
{
public:

    static void AddData(uint8 data);

    static void Update();
    /// ��� �������� ����� ��� ���������� ������ ��������
    static void SetBufferForScreenRow(uint8 *pixels);
};
