#pragma once
#include "Osci/DeviceSettings.h"


// ����� �� �������
struct Point
{
    Point(BitSet16 dataA, BitSet16 dataB)
    {
        data[Chan::A] = dataA;
        data[Chan::B] = dataB;
    }
    static Point CreateEmpty();
    bool IsEmpty();
    int Min(Chan::E ch);
    int Max(Chan::E ch);
private:
    BitSet16 data[2];
};


// ��������� ������ ������������ - ������� ������ �����
struct Record
{
    // ����� ������ ������ �����
    PackedTime timeStart;

    // ����� �������� ������.
    // ��� 0 - ����� 1; ��� 1 - ����� 2; ��� 2 - ������. ������ � ����� ������� ����������� ����� ��������������� ���������� � ��������� ����� ��������� Record
    uint8 bitMaskSources;

    // ����� ���������� �����
    uint numPoints;

    void Init();

    // ���������� �������� �����
    void AddPoint(BitSet16 dataA, BitSet16 dataB);

    // ����� ����� � ��������������
    uint NumPoints();

    // ���������� ������ ��������� ������
    int FreeMemory();
};


struct StorageRecorder
{
    // ������������� ����� �������� � ����� "�����������"
    static void Init();

    // ������ ����� ������ ��� �������� ������ � ���������
    static void CreateNewRecord();

    // ���������� ��������� �� ������� ������
    static Record *CurrentRecord();
};
