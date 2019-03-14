#pragma once


struct BigSymbol
{
    uint8 code;     ///< ASCII-��� �������
    uint8 width;    ///< ������ ������� � �����
    uint16 offset;  ///< �������� ������� ����� ������� ������������ ������ ������� ������
};

struct BigFont
{
    uint8           height;       ///< ������ ���������
    uint8           numSymbols;   ///< ����� �������� � ������
    const uint8     *data;        ///< ������ ������ ��������
    const BigSymbol *symbols;     ///< ������ �� ������ ������� ��������

    uint8 GetWidth(uint8 symbol) const;
    bool ExistSymbol(uint8 symbol) const;
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
    static const Font *Current();
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetHeight();
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
};


extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];
extern const BigFont fontDigits64;
