#pragma once
#include "defines.h"


struct BigSymbol
{
    uint8 code;     ///< ASCII-��� �������
    uint8 width;    ///< ������ ������� � �����
    uint16 offset;  ///< �������� ������� ����� ������� ������������ ������ ������� ������
};

struct FullSymbol
{
    BigSymbol symbol;
    uint8     *offset;  /// ������� �� ������ ���� �����
    bool RowNotEmpty(int row) const;
    bool BitIsExist(int row, int bit) const;
private:
    /// ���������� ��������� �� ������ ���� ������
    uint8 *GetRow(int row) const;
    uint8 BytesInRow() const;
};

struct BigFont
{
    uint8           height;       ///< ������ ���������
    uint8           numSymbols;   ///< ����� �������� � ������
    const uint8     *data;        ///< ������ ������ ��������
    const BigSymbol *symbols;     ///< ������ �� ������ ������� ��������

    uint8 GetWidth(uint8 symbol) const;
    bool GetFullSymbol(FullSymbol &symbol, uint8 code) const;
};


struct Symbol
{
    uchar width;
    uchar bytes[8];
};

struct Font
{
    struct Type
    {
        enum E
        {
            _5,
            _8,
            _UGO,
            _UGO2,
            _Big64,
            Size,
            None
        } value;
    };

    int _height;
    Symbol symbols[256];

public:
    static int  GetLengthText(pString text);
    static int  GetHeightSymbol(char symbol);
    static int  GetLengthSymbol(char symbol);
    /// ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    static void SetCurrent(Font::Type::E typeFont);
    /// ��������������� �����, ������ ������� ����� ��������� ������� SetCurrent()
    static void Pop();
    static bool IsBig();
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetHeight();
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    static void SetMinWidth(uint8 width);
    static void SetSpacing(int spacing);
};


extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];
extern const BigFont fontDigits64;
