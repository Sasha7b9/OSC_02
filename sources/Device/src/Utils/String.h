#pragma once
#include "Display/Colors.h"


struct TypeConversionString
{
    enum E
    {
        None,           // �������������� ������ �� ������������
        FirstUpper      // ������ ������ - � ������ ��������, ��������� - � ������
    };
};


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void Set(TypeConversionString::E conv, const char *format, ...);

    char *c_str() const;
    /// ���������� ����� �� ������ � ������� �����������
    int Draw(int x, int y, Color color = Color::NUMBER) const;

    static const char * const ERROR;

    void Free();

    void Append(const char *str);

    void Append(char symbol);
    /// ������� numSymbols �� ������ ������
    void RemoveFromBegin(uint numSymbols);

    uint Size() const;

    char &operator[](uint i);

private:

    bool Allocate(uint size);
    void Conversion(TypeConversionString::E conv);

    char *buffer;
};
