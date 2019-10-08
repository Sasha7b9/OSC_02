#include "defines.h"
#include "Console.h"
#include "Display/Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>
#include <cstdio>


static CHAR_BUF2(buffer, 33, 100);

/// true ��������, ��� ��� ������� ������ ������� � ��������� � �� ����� ������ ������ (��� ����������, ����� ���������� ��� �� ����������)
static bool inProcessDrawConsole = false;
/// ���������� ����������� ����� � �������
static int stringInConsole = 0;
/// ����� ����������� ���������� �������� ������������� ���������� ����� � �������
static int16 prevMaxStrinsInConsole = -1;



void Console::Draw()
{
    if (prevMaxStrinsInConsole == -1)
    {
        prevMaxStrinsInConsole = set.dbg.numStrings;
    }

    if (!IsShown() || stringInConsole == 0)
    {
        return;
    }

    inProcessDrawConsole = true;

    Font::SetCurrent(TypeFont::_5);

    int y = 1;

    for (int i = 0; i < stringInConsole; i++)
    {
        int length = Font::GetLengthText(buffer[i]);
        Region(length, 6).Fill(0, y + 3, Color::BACK);
        String(buffer[i]).Draw(1, y, Color::FILL);
        y += 6;
    }

    Font::SetCurrent(TypeFont::_8);

    inProcessDrawConsole = false;
}


static void DeleteFirstString()
{
    for (int16 i = 1; i < stringInConsole; i++)
    {
        std::strcpy(buffer[i - 1], buffer[i]); //-V2513
    }
    stringInConsole--;
}


void Console::AddString(char *string)
{
    /// \todo �� ���������� ��������� ������. ������� ���������� ����������
    if (!inProcessDrawConsole)      // ���������� �� ������� ����, ��� ������ �� ���������� ����� ������� � ������ ������
    {
        static int count = 0;
        if (stringInConsole == set.dbg.numStrings)
        {
            DeleteFirstString();
        }
        std::sprintf(buffer[stringInConsole], "%d %s", count++, string);
        stringInConsole++;
    }
}


int Console::NumberOfLines()
{
    return stringInConsole;
}


bool Console::IsShown()
{
    return set.dbg.showConsole != 0;
}


void Console::OnChanged_MaxStringsInConsole()
{
    /// \todo �����, ������, �� ������ ���������� ��������� � ������, ����� �������� ����� ������, ��� ����������� ���������� �� ����������

    int delta = prevMaxStrinsInConsole - set.dbg.numStrings;

    for (int i = 0; i < delta; i++)
    {
        DeleteFirstString();
    }

    prevMaxStrinsInConsole = set.dbg.numStrings;


}
