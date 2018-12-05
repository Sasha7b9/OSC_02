#pragma once
#include "Display/Colors.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class String
{
public:
    String();
    String(const String &);
    String(const char *format, ...);
    ~String();
        
    char *CString() const;
    /// ���������� ����� �� ������ � ������� �����������
    int Draw(int x, int y, Color color = Color::NUMBER) const;

private:

    bool Allocate(uint size);

    char *buffer;
};
