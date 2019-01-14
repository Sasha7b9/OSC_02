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
    inline uint8 *Data() { return buffer; };
    uint   Size() const;
private:
    uint8 *buffer;
    uint size;
};
