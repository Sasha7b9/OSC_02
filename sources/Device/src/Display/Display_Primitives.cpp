#include "defines.h"
#include "Command.h"
#include "Display_Primitives.h"
#include "Painter.h"
#include "Utils/Buffer.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include <cstring>



Display::Primitives::Region::Region(int _width, int _height) : width(_width), height(_height)
{
}


void Display::Primitives::Region::DrawBounded(int x, int y, Color colorFill, Color colorBound)
{
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorFill);
    Rectangle(width, height).Draw(x, y, colorBound);
}


Display::Primitives::Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{
}
    

Display::Primitives::HLine::HLine(int _width) : width(_width)
{
}


Display::Primitives::VLine::VLine(int _height) : height(_height)
{
}


Display::Primitives::Line::Line(int _x0, int _y0, int _x1, int _y1) : x0(_x0), y0(_y0), x1(_x1), y1(_y1)
{
    if (x0 < 0)
    {
        x0 = 0;
    }
    if (y0 < 0)
    {
        y0 = 0;
    }
    if (x1 < 0)
    {
        x1 = 0;
    }
    if (y1 < 0)
    {
        y1 = 0;
    }
}


Display::Primitives::Char::Char(Symbol5::E _ch) : ch((char)_ch), font(Font::Type::_5)
{

}


Display::Primitives::Char::Char(SymbolUGO::E _ch) : ch((char)_ch), font(Font::Type::_UGO)
{

}


Display::Primitives::Char::Char(SymbolUGO2::E _ch) : ch((char)_ch), font(Font::Type::_UGO2)
{

}

Display::Primitives::Char::Char(char _ch) : ch(_ch), font(Font::Type::_8)
{

}


int Display::Primitives::Char::Draw(int x, int y, Color color)
{
    Font::SetCurrent(font);

	String("%c", ch).Draw(x, y, color);

    int result = x + Font::GetLengthSymbol(ch) + 1;

    Font::Pop();

    return result;
}


void Display::Primitives::Char::Draw4SymbolsInRect(int x, int y, Color color)
{
    color.SetAsCurrent();

    Font::SetCurrent(font);

    for (char i = 0; i < 2; i++)
    {
        String("%c", ch + i).Draw(x + 8 * i, y);
        String("%c", ch + i + 16).Draw(x + 8 * i, y + 8);
    }

    Font::Pop();
}


void Display::Primitives::Char::Draw10SymbolsInRect(int x, int y, Color color)
{
    color.SetAsCurrent();

    Font::SetCurrent(font);

    for (char i = 0; i < 5; i++)
    {
        String("%c", ch + i).Draw(x + 8 * i, y);
        String("%c", ch + i + 16).Draw(x + 8 * i, y + 8);
    }

    Font::Pop();
}


Display::Primitives::Text::Text(const char *_text, uint8 _size) : text(_text), sizeOfType(_size)
{
}


Display::Primitives::Text::Text(const String &string, uint8 _size) : sizeOfType(_size)
{
    text = string.CString();
}


int Display::Primitives::Text::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    if (text[0] != '\0')
    {
        if (sizeOfType != 1)
        {
            DrawBig(x, y);
        }
        else
        {
            x = DrawSmall(x, y);
        }
    }

    return x;
}


int Display::Primitives::Text::DrawWithLimitation(int x, int y, int limitX, int limitY, int limitWidth, int limitHeight)
{
    int retValue = x;

    while (*text)
    {
        x = DrawCharWithLimitation(x, y, *text, limitX, limitY, limitWidth, limitHeight);
        retValue += Font::GetLengthSymbol(*text);
        text++;
    }

    return retValue + 1;
}


int Display::Primitives::Text::DrawCharWithLimitation(int eX, int eY, char _symbol, int limitX, int limitY, int limitWidth, int limitHeight)
{
    uint8 symbol = static_cast<uint8>(_symbol);

    int8 width = static_cast<int8>(Font::GetWidth(symbol));
    int8 height = static_cast<int8>(Font::GetHeight());

    for (int b = 0; b < height; b++)
    {
        if(Font::RowNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (Font::BitIsExist(symbol, b, bit))
                {
                    if ((x >= limitX) && (x <= (limitX + limitWidth)) && (y >= limitY) && (y <= limitY + limitHeight))
                    {
                        Point().Draw(x, y);
                    }
                }
                x++;
            }
        }
    }

    return eX + width + 1;
}


int Display::Primitives::Text::DrawInCenterRect(int eX, int eY, int width, int eHeight, Color color)
{
    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeightSymbol(text[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2 + 1;
    return Draw(x, y, color);
}


int Display::Primitives::Text::DrawOnBackground(int x, int y, Color colorBackground)
{
    int width = Font::GetLengthText(text);
    int height = Font::GetHeight();

    Color colorText(Color::GetCurent());
    Region(width, height).Fill(x - 1, y, colorBackground);

    colorText.SetAsCurrent();

    return Text(text).Draw(x, y);
}


void Display::Primitives::Text::DrawRelativelyRight(int xRight, int y, Color color)
{
    Text(text).Draw(xRight - Font::GetLengthText(text), y, color);
}


static bool IsLetter(char symbol)
{
    static const bool isLetter[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
    };

    return isLetter[static_cast<uint8>(symbol)];
}


static char *GetWord(const char *firstSymbol, int *length, char buffer[20])
{
    int pointer = 0;
    *length = 0;

    while (IsLetter(*firstSymbol))
    {
        buffer[pointer] = *firstSymbol;
        pointer++;
        firstSymbol++;
        (*length)++;
    }
    buffer[pointer] = '\0';

    return buffer;
}


static bool IsConsonant(char symbol)
{
    static const bool isConsonat[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false
    };

    return isConsonat[static_cast<uint8>(symbol)];
}


static bool CompareArrays(const bool *array1, const bool *array2, int numElems) //-V2506
{
    for (int i = 0; i < numElems; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}


/// \brief ������� ��������� �������. C letters ���������� ����� �����, ��� ����� ����� �������, � lettersInSyllable ����� �������� ����� ���� � 
/// ��������� �����. ���� ����� �����������, ������� ���������� false
static bool FindNextTransfer(const char *letters, int8 *lettersInSyllable) //-V2506
{

#define VOWEL       0   // �������
#define CONSONANT   1   // ���������

    *lettersInSyllable = static_cast<int8>(std::strlen(letters)); //-V1029

    if (std::strlen(letters) <= 3) //-V2513
    {
        return false;
    }

    static const bool template1[3] = { false, true, true };               //     011     2   // ����� ������� ������� �������
    static const bool template2[4] = { true, false, true, false };        //     1010    2
    static const bool template3[4] = { false, true, false, true };        //     0101    3
    static const bool template4[4] = { true, false, true, true };         //     1011    3
    static const bool template5[4] = { false, true, false, false };       //     0100    3
    static const bool template6[4] = { true, false, true, true };         //     1011    3
    static const bool template7[5] = { true, true, false, true, false };  //     11010   3
    static const bool template8[6] = { true, true, false, true, true };   //     11011   4

    bool consonant[20];

    int size = static_cast<int>(std::strlen(letters)); //-V2513

    for (int i = 0; i < size; i++)
    {
        consonant[i] = IsConsonant(letters[i]);
    }

    if (CompareArrays(template1, consonant, 3))
    {
        *lettersInSyllable = 2;
        return true;
    }

    if (CompareArrays(template2, consonant, 4))
    {
        *lettersInSyllable = 2;
        return true;
    }

    if (std::strlen(letters) < 5) //-V2513
    {
        return false;
    }

    if (CompareArrays(template3, consonant, 4) || CompareArrays(template4, consonant, 4) || CompareArrays(template5, consonant, 4) ||
        CompareArrays(template6, consonant, 4))
    {
        *lettersInSyllable = 3;
        return true;
    }

    if (std::strlen(letters) < 6) //-V2513
    {
        return false;
    }

    if (CompareArrays(template7, consonant, 5))
    {
        *lettersInSyllable = 3;
        return true;
    }

    if (CompareArrays(template8, consonant, 5))
    {
        *lettersInSyllable = 4;
        return true;
    }

    return false;
}


static int8 *BreakWord(char *word)
{
    int num = 0;
    static int8 lengthSyllables[10];
    char *position = word;
    while (FindNextTransfer(position, &(lengthSyllables[num])))
    {
        position += lengthSyllables[num];
        num++;
    }
    lengthSyllables[num + 1] = 0;

    if (std::strcmp(word, "���������") == 0) //-V2513
    {
        int8 lengths[] = { 5, 2, 2, 0 };
        std::memcpy(lengthSyllables, lengths, 4);
    }
    else if (std::strcmp(word, "���������������") == 0) //-V2513
    {
        int8 lenghts[] = { 4, 3, 4, 5, 3, 0 };
        std::memcpy(lengthSyllables, lenghts, 6);
    }
    else
    {
        // ����� ������ �� ������
    }

    return lengthSyllables;
}


// ���������� ����� ����� �� ����� numSyllable(�����������) ������ �� ������ ��������
static char *PartWordForTransfer(char *word, const int8 *lengthSyllables, int numSyllable, char buffer[30])
{
    uint length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += static_cast<uint>(lengthSyllables[i]);
    }
    std::memcpy(static_cast<void *>(buffer), static_cast<void *>(word), length);
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}


// ���� draw == false, �� �������� ������ �� ����, ������ ������������ ������ ��� ����������
static int DrawPartWord(char *word, int x, int y, int xRight, bool draw) //-V2506
{
    int8 *lengthSyllables = BreakWord(word);
    int numSyllabels = 0;
    char buffer[30];
    for (int i = 0; i < 10; i++)
    {
        if (lengthSyllables[i] == 0)
        {
            numSyllabels = i;
            break;
        }
    }

    for (int i = numSyllabels - 2; i >= 0; i--)
    {
        char *subString = PartWordForTransfer(word, lengthSyllables, i, buffer);
        int length = Font::GetLengthText(subString);
        if (xRight - x > length - 5)
        {
            if (draw)
            {
                String(subString).Draw(x, y);
            }
            return static_cast<int>(std::strlen(subString) - 1);
        }
    }

    return 0;
}


int Display::Primitives::Text::DrawInRectWithTransfers(int eX, int eY, int eWidth, int eHeight, Color color)
{
    color.SetAsCurrent();

    int top = eY;
    int left = eX;
    int right = eX + eWidth;
    int bottom = eY + eHeight;

    char buffer[20];
    int numSymbols = static_cast<int>(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < bottom && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // ��� ��������� �������� ��� ����, �.�. ����� �� �������
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                //x = DrawChar(x, y, symbol);
                x = Char(symbol).Draw(x, y);
            }
            else                                            // � ����� ������� �� ������� ���� ��� ��������� �������, �.�. ������� �����
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    //x = DrawText(x, y, word);
                    x = Text(word).Draw(x, y);
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}


// ���������� ������ ������, ������� ����� ����� text, ��� ������ �� left �� right � ���������� height. ���� bool == false, �� ����� �� ������ �� ����� 
static bool GetHeightTextWithTransfers(int left, int top, int right, const char *text, int *height)
{
    char buffer[20];
    int numSymbols = static_cast<int>(std::strlen(text)); //-V2513

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // ��� ��������� �������� ��� ����, �.�. ����� �� �������
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x += Font::GetLengthSymbol(symbol);
            }
            else                                            // � ����� ������� �� ������� ���� ��� ��������� �������, �.�. ������� �����
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = DrawPartWord(word, x, y, right, false);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x += Font::GetLengthText(word);
                }
            }
        }
        x = left;
        y += 9;
    }

    *height = y - top + 4;

    LIMITATION(*height, 0, 239); //-V2516

    return curSymbol == numSymbols;
}


int Display::Primitives::Text::DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill)
{
    int height = 0;
    GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, text, &height);

    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    DrawInRectWithTransfers(x + 3, y + 3, width - 8, height, colorFill);
    return y + height;
}


int Display::Primitives::Text::DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill)
{
    Region(width, height).DrawBounded(x, y, colorBackground, colorFill);

    colorFill.SetAsCurrent();

    return Text(text).DrawInCenterRect(x, y, width, height);
}


void Display::Primitives::Text::DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground)
{
    int lenght = Font::GetLengthText(text);
    //int eX = DrawStringInCenterRect(x, y, width, height, text, colorBackground);
    int eX = Text(text).DrawInCenterRect(x, y, width, height, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Region(w, h).Fill(eX - lenght - widthBorder, y - widthBorder + 1);
    //DrawStringInCenterRect(x, y, width, height, text, colorText);
    Text(text).DrawInCenterRect(x, y, width, height, colorText);
}


Display::Primitives::DashedVLine::DashedVLine(int _height, int _deltaFill, int _deltaEmpty, int _deltaStart) : height(_height), deltaFill(_deltaFill), deltaEmpty(_deltaEmpty), deltaStart(_deltaStart)
{
}


void Display::Primitives::DashedVLine::Draw(int x, int y0)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR("������������ �������� deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // ���� ����� ����� �������� �� � ������ ������
    {
        y += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // ���� ������ ����� ���������� �� �����
        {
            //DrawVLine(x, y0, y - 1);
            VLine(y - y0 - 1).Draw(x, y0);
        }
    }

    int y1 = y0 + height;

    while (y < y1)
    {
        //DrawVLine(x, y, y + deltaFill - 1);
        VLine(deltaFill - 1).Draw(x, y);

        y += (deltaFill + deltaEmpty);
    }
}


Display::Primitives::DashedHLine::DashedHLine(int _width, int _deltaFill, int _deltaEmpty, int _deltaStart) : 
    width(_width), deltaFill(_deltaFill), deltaEmpty(_deltaEmpty), deltaStart(_deltaStart)
{
}


void Display::Primitives::DashedHLine::Draw(int x0, int y)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR("������������ �������� deltaStart = %d", deltaStart);
        return;
    }

    int x = x0;

    if (deltaStart != 0)                // ���� ����� ����� �������� �� � ������ ������
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // ���� ������ ����� ���������� �� �����
        {
            //DrawHLine(y, x0, x - 1);
            HLine(x - 1 - x0).Draw(x0, y);
        }
    }

    int x1 = x0 + width;

    while (x < x1)
    {
        //DrawHLine(y, x, x + deltaFill - 1);
        HLine(deltaFill - 1).Draw(x, y);

        x += (deltaFill + deltaEmpty);
    }
}


Display::Primitives::VPointLine::VPointLine(int _height, float _delta) : height(_height), delta(_delta)
{
}


void Display::Primitives::VPointLine::Draw(int _x, int _y, Color color)
{
    color.SetAsCurrent();

    int y0 = _y;
    int y1 = y0 + height;

    for (int y = y0; y <= y1; y += static_cast<int>(delta))
    {
        //SetPoint(x, y);
        Point().Draw(_x, y);
    }
}


Display::Primitives::HPointLine::HPointLine(int _width, float _delta) : width(_width), delta(_delta)
{
}


void Display::Primitives::HPointLine::Draw(int _x, int _y)
{
    int x0 = _x;
    int x1 = _x + width;

    for (int x = x0; x <= x1; x += static_cast<int>(delta))
    {
        //SetPoint(x, y);
        Point().Draw(x, _y);
    }
}


Display::Primitives::MultiHPointLine::MultiHPointLine(int _numLines, const uint8 *_y, int _delta, int _count) : numLines(_numLines), y(_y), delta(_delta), count(_count)
{
}


Display::Primitives::VLineArray::VLineArray(int _numLines, uint8 *_y0y1) : numLines(_numLines), y0y1(_y0y1)
{
}


void Display::Primitives::VLineArray::Draw(int x, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        uint8 y0 = *y0y1++;
        uint8 y1 = *y0y1++;
        VLine(y1 - y0).Draw(x, y0);
    }
}


Display::Primitives::MultiVPointLine::MultiVPointLine(int _numLines, uint16 *_x0, int _delta, int _count) : numLines(_numLines), x0(_x0), delta(_delta), count(_count)
{
}
