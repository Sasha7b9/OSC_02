#pragma once
#include "defines.h"
#include "FPGA/FPGA.h"


class Interpolator
{
public:
    static void Run(uint8 *data, uint num);
};


static uint8 *begin = nullptr;  // ������ �������������� ������
static uint8 *end = nullptr;    // ����� �������������� ������


// ��������� ��������� �������
struct Segment
{
    uint8 *start;   // ��������� �� ������ ������� �������
    uint8 *end;     // ��������� �� ������ ������� �� ��������
};


// ������� ������ ������ ������� (�.�. � ������� ��������� ������� "������" ��������), ������� � �������� � ������� start
static bool FindEmptySegment(uint8 *start, Segment *segment);

// ��������������� ������ � ��������
static void InterpolateSegment(Segment *segment);

// ������� ����� ������� ��������. ���������� end, ���� ������ �������� ���
static uint8 *FindEmptyElement(uint8 * const start);

// ����� ������ "��������" ������� ������. ���������� end, ���� ������ �������� ���
static uint8 *FindReadedElement(uint8 * const start);


void Interpolator::Run(uint8 *data, uint num)
{
    begin = data;
    end = begin + num;

    Segment segment;

    segment.end = begin;

    while(FindEmptySegment(segment.end, &segment))
    {
        InterpolateSegment(&segment);
    }
}


static bool FindEmptySegment(uint8 *start, Segment *segment)
{
    segment->start = FindEmptyElement(start);

    segment->end = FindReadedElement(segment->start);

    return (segment->start != end) && (segment->end != end);
}


static uint8 *FindEmptyElement(uint8 * const start)
{
    uint8 *element = start;
    ShiftPoint::E *points = randShift.points;
    int delta = TBase::DeltaPoint();
    int index = (start - begin) % delta;

    while(element != end)
    {
        if((*element == VALUE::NONE) || (points[index] == ShiftPoint::INTERPOLATED))   // ���� ��������� ������� "������"
        {
            break;                  // �� �� ��� �����
        }
        element++;
        index++;
        if(index == delta)
        {
            index = 0;
        }
    }

    return element;
}


static uint8 *FindReadedElement(uint8 * const start)
{
    uint8 *element = start;
    ShiftPoint::E *points = randShift.points;
    int delta = TBase::DeltaPoint();
    int index = (start - begin) % delta;
    
    while(element != end)
    {
        if(points[index] != ShiftPoint::INTERPOLATED)
        {
            break;
        }
        element++;
        index++;
        if(index == delta)
        {
            index = 0;
        }
    }

    return element;
}


static void InterpolateSegment(Segment *segment)
{
    if(segment->start == end || segment->start == begin || segment->end == end)
    {
        return;
    }

    float before = *(segment->start - 1);               // �������� ����� "������" ���������
    float after = *(segment->end);                      // �������� ����� "�������" ��������

    int numTicks = segment->end - segment->start + 2;   // �� ������� �������� ������ ��� �������

    float delta = (after - before) / numTicks;

    for(int tick = 0; tick < numTicks - 1; tick++)
    {
        float value = before + delta * (tick + 1);

        *(segment->start + tick) = static_cast<uint8>(value + 0.5F);
    }
}
