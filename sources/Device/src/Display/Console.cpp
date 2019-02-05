#include "defines.h"
#include "Console.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include <cstring>
#include <cstdio>

#include "Settings/Settings.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_CONSOLE   20
static CHAR_BUF2(buffer, SIZE_CONSOLE, 100);

/// true ��������, ��� ��� ������� ������ ������� � ��������� � �� ����� ������ ������ (��� ����������, ����� ���������� ��� �� ����������)
static bool inProcessDrawConsole = false;
/// ���������� ����������� ����� � �������
static int stringInConsole = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Console::Draw()
{
    if (!IsShown())
    {
        return;
    }

    inProcessDrawConsole = true;

    Font::SetCurrent(Font::Type::_5);

    int y = 1;

    for (int i = 0; i < stringInConsole; i++)
    {
        int length = Font::GetLengthText(buffer[i]);
        Region(length, 6).Fill(0, y + 3, Color::BACK);
        String(buffer[i]).Draw(1, y, Color::FILL);
        y += 6;
    }

    Font::SetCurrent(Font::Type::_8);

    inProcessDrawConsole = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Console::AddString(char *string)
{
    /// \todo �� ���������� ��������� ������. ������� ���������� ����������
    if (!inProcessDrawConsole)      // ���������� �� ������� ����, ��� ������ �� ���������� ����� ������� � ������ ������
    {
        static int count = 0;
        if (stringInConsole == SIZE_CONSOLE)
        {
            for (int i = 1; i < SIZE_CONSOLE; i++)
            {
                std::strcpy(buffer[i - 1], buffer[i]); //-V2513
            }
            stringInConsole--;
        }
        std::sprintf(buffer[stringInConsole], "%d %s", count++, string);
        stringInConsole++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Console::NumberOfLines()
{
    return stringInConsole;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Console::IsShown()
{
    return set.dbg_showConsole != 0;
}
