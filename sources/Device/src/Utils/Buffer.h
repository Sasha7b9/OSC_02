#pragma once
#include "defines.h"


/*
    ����� ����������� ������������ ��� ��������������� ���������/������������ ������ �� ����
*/

class Buffer
{
public:
    Buffer(uint size = 0U);
    ~Buffer();
    void Realloc(uint size);
    void Fill(uint8 value);
    uint8 *data;
    uint Size() const { return size; }
private:
    uint size;
    void Free();
    void Malloc(uint size);
};
