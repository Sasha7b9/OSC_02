#pragma once
#include "Display/Colors.h"



class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void Set(const char *format, ...);

    char *CString() const;
    /// ���������� ����� �� ������ � ������� �����������
    int Draw(int x, int y, Color color = Color::NUMBER) const;

    static const char * const ERROR;

private:

    bool Allocate(uint size);
    void Free();

    char *buffer;
};
