#pragma once
#include "Data/DataSettings.h"


/// ����� �� �������
struct PointRec
{
    PointRec(BitSet16 dataA, BitSet16 dataB)
    {
        data[Chan::A] = dataA;
        data[Chan::B] = dataB;
    }
    static PointRec CreateEmpty();
    bool IsEmpty();
    int Min(Chan::E ch);
    int Max(Chan::E ch);
private:
    BitSet16 data[2];
};


/// ��������� ������ ������������ - ������� ������ �����
struct Record
{
    Record() : start(0), numPoints(0), pointer(0) {} //-V730

    void SetDataAddress(uint16 *address);
    /// ���������� �������� �����
    void AddPoint(BitSet16 dataA, BitSet16 dataB);
    /// ����� ����� � ��������������
    uint NumPoints();
    /// �������� ����� � ������� position
    PointRec GetPoint(uint position, uint maxPoints);
    /// �������� ��������� �����
    PointRec NextPoint(uint maxPoints);
    /// ����� ������ ������ �����
    PackedTime timeStart;
    /// ���������� ������ ��������� ������
    int FreeMemory();
private:
    /// ��������� �� ����� ������ - ���������� ����� ������ ���������� �����
    PointRec *start;
    /// ���������� ���������� �����
    uint numPoints;
    /// ��������� �� ��������� ���������� ������
    uint pointer;
};


struct StorageRecorder
{
    static void CreateNewRecord();

    static Record *CurrentRecord();
};
