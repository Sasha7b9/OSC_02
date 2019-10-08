#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Hardware/Timer.h"
#include "Utils/Queue.h"
#include <cstring>


Queue<WarningStruct> warnings;


/// ������� �������������� � ������� ������� �����
static void RemoveOld();
/// ���������� true, ���� ��������� ��������� � ������� ����� ��
static bool BackMessageSame(const char *message);

static void DrawMessages();



void Warnings::AddWarning(const char *warning)
{
    if (BackMessageSame(warning))
    {
        warnings[warnings.Size() - 1].timeStart = TIME_MS;
    }
    else
    {
        warnings.Push(WarningStruct(warning));
    }
}


static bool BackMessageSame(const char *message)
{
    if (warnings.IsEmpty())
    {
        return false;
    }

    return std::strcmp(message, warnings[warnings.Size() - 1].message) != 0;
}


void Warnings::Draw()
{
    if (warnings.Size() == 0)
    {
        return;
    }

    RemoveOld();

    if (!warnings.IsEmpty())
    {
        DrawMessages();
    }
}


static void RemoveOld()
{
    while (!warnings.IsEmpty() && warnings[0].IsDead())
    {
        warnings.Front();
    }
}


static void DrawMessages()
{
    Font::SetCurrent(TypeFont::_8);

    int y = Grid::BottomForWarnings();                   // ���������� y ������� ������ ���� ��������������, � ������� ����� ���������� ��������� ���������

    int size = warnings.Size();

    for (int i = size - 1; i >= 0; i--)
    {
        int h = warnings[i].Height(Grid::Width());

        y -= h;

        if (y < (Grid::Bottom() - Grid::Height()))
        {
            break;
        }

        warnings[i].Draw(Grid::Left(), y, Grid::Width());
    }
}


bool Warnings::IsDrawing()
{
    return (warnings.Size() != 0);
}


WarningStruct::WarningStruct(const char *msg) : message(msg)
{
    timeStart = TIME_MS;
}


bool WarningStruct::IsDead() const
{
    return (TIME_MS - timeStart) > 5000;
}

int WarningStruct::Height(int) const
{
    return 10;
}

void WarningStruct::Draw(int x, int y, int width) const
{
    Text(message).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::FILL);
}
