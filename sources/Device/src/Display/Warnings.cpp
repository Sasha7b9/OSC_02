#include "defines.h"
#include "Display/DisplayTypes.h"
#include "Display/Warnings.h"
#include "Hardware/Timer.h"
#include "Utils/Queue.h"
#include <cstring>


Queue<WarningStruct> warnings;


/// ������� �������������� � ������� ������� �����
static void RemoveOld();
/// ���������� true, ���� ����� ����� ������� ��������� � ������� �������
static bool FrontMessageIsDead();
/// ���������� true, ���� ��������� ��������� � ������� ����� ��
static bool BackMessageSame(const char *message);

static void DrawMessages(int left, int down, int width, int height);
/// ���������� ������ ������������� � ������� ������� weidth ���������
static int HeightMessage(const char *message, int width);



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


void Warnings::Show(int left, int down, int width, int height)
{
    RemoveOld();

    if (!warnings.IsEmpty())
    {
        DrawMessages(left, down, width, height);
    }
}


static void RemoveOld()
{
    while (!warnings.IsEmpty() && FrontMessageIsDead())
    {
        warnings.Front();
    }
}


static bool FrontMessageIsDead()
{
    return (TIME_MS - warnings[0].timeStart) > 3000;
}


static void DrawMessages(int left, int down, int width, int height)
{
    int y = down;                   // ���������� y ������� ������ ���� ��������������, � ������� ����� ���������� ��������� ���������

    int size = warnings.Size();

    for (int i = size - 1; i >= 0; i--)
    {
        const char *warning = warnings[i].message;

    }
}

