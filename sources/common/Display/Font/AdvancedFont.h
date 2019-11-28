#pragma once
#include "Font.h"



struct AdvancedFont
{
    AdvancedFont() {};
    /// ���������� ����������� �����
    AdvancedFont(TypeFont::E type);
    /// ���������� true, ���� � ������ row ���� �������
    bool RowNotEmpty(uint8 symbol, int row);
    /// ���������� ������ �������
    uint8 GetWidth(uint8 symbol);
    /// ���������� ������ �������
    uint8 GetHeight(uint8 symbol);
    /// ���������� ������ ������ �������� �������
    uint8 GetHeight();

    bool BitIsExist(uint8 symbol, int row, int bit);

private:
    static TypeFont::E currentType;
};
