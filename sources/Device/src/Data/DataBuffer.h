#pragma once
#include "Data/DataSettings.h"


/// ����� �������� ������ 
class DataBuffer
{
public:
    static void Init();

    static uint Size();
    /// ���������� ��������� �� ������, ���������� � ����� ����������
    static DataSettings *Top();
};
