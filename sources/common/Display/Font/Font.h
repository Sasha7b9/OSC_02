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
    static int  GetHeightSymbol(char symbol);
    static int  GetLengthSymbol(char symbol);
    /// ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    static void SetCurrent(TypeFont::E typeFont);
    /// ��������������� �����, ������ ������� ����� ��������� ������� SetCurrent()
    static void Pop();
    static bool IsBig();
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetHeight();
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    static void SetMinWidth(uint8 width);
    /// ������������� ���������� ������ �������� ����� ���������.
    static void SetSpacing(int spacing);
    /// ���������� ���������� ������ �������� ����� ���������
#ifndef PANEL
    static int GetSpacing();
#endif
};


extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];
