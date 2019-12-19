#pragma once
#include "defines.h"


struct Symbol
{
    uchar width;
    uchar bytes[8];
};

struct TypeFont
{
    enum E
    {
        _5,
        _8,
        _UGO,
        _UGO2,
        _GOST28,
        _GOST72bold,    // ������������ ��� ������ ��������� �����������
        _OMEGA72,       // ������������ � ����������� ��� ������ ����������� ��. ����� ������ ���� ������
        Count,
        None
    } value;
};

struct Font
{
    int _height;
    Symbol symbols[256];

public:
    static int  GetLengthText(pString text);

    /// ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    static void Set(const TypeFont::E typeFont);
    static TypeFont::E Current();
    /// ��������������� �����, ������ ������� ����� ��������� ������� SetCurrent()
    static void Pop();
    static bool IsBig();
    static uint8 GetHeight();
    static void SetMinWidth(uint8 width);
    /// ������������� ���������� ������ �������� ����� ���������.
    static void SetSpacing(int spacing);

#ifdef STM32F437xx
    /// ���������� ���������� ������ �������� ����� ���������
    static int GetSpacing();
    static void SetLength(uint8);
#endif

#ifdef PANEL
    static void SendLengthText(char *text);
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetWidth(char symbol);
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
#endif
};
