#pragma once
#include "defines.h"


/*
    ����� ����������� ������������ ��� ��������������� ���������/������������ ������ �� ����
*/

class Buffer
{
public:
    Buffer(uint size);
    ~Buffer();
    uint8 *data;
    uint size;
};
