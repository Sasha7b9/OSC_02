#pragma once
#include "Osci/DeviceSettings.h"


struct PointFloat
{
    float min;
    float max;

    // �������������� ����� � ������
    void Prepare();

    bool IsEmpty() const;

    void Add(float value);
};


// ��������� ������ ������������ - ������� ������ �����
struct Record
{
    PackedTime timeStart;       // ����� ������ ������ �����
    int        numPoints;       // ����� ���������� �����
    uint8      sources;         // ����� �������� ������.
                                // ��� 0 - ����� 1; ��� 1 - ����� 2; ��� 2 - ������. ������ � ����� ������� ����������� ����� ��������������� ���������� � ��������� ����� ��������� Record
    uint8      bytesOnPoint;    // ������� ���� ����� �� ���� ����� ���� ����������

    uint8      offsetB;         // �������� �������� ������ B
    uint8      offsetSensor;    // �������� �������� �������

    // ���������������� ��������� ����� ������� ������
    void Init();

    // ���������� �������� �����
    void AddPoints(BitSet16 dataA, BitSet16 dataB);

    // ���������� ����� ������� � ������
    void AddPoint(float value);

    // ����� ����� � ��������������
    int NumPoints() const;

    // ���������� ������ ��������� ������, ��������� ��� �������� ����� ������
    uint FreeMemory() const;

    // ���������� true, ���� ������ ��������
    bool IsValid() const;

    // ���������� ����� ������� ���������� �� ������� �����
    uint8 *End() const;

    uint8 *Begin() const;

private:
    
    BitSet16 *ValueA(int number);          // ��������� �� ����� number ������ A
    BitSet16 *ValueB(int number);          // ��������� �� ����� number ������ B
    PointFloat *ValueSensor(int number);    // ��������� �� ����� number �������

    // � ����� ������ ���������� ������
    uint8 *BeginData();

    // ��������� �� ������ ������ ����� � ������� number
    uint8 *AddressPoints(int number);

    // ��������������� ����� ����� ��������� num1 � num2
    void Interpolate(int num1, int num2);
};


struct StorageRecorder
{
    // ������������� ����� �������� � ����� "�����������"
    static void Init();

    // ������ ����� ������ ��� �������� ������ � ���������
    static bool CreateNewRecord();

    // ���������� ��������� �� ������� ������
    static Record *LastRecord();

    // ���������� ���������� ���������� �������. 0 - ���������, 1 - �������������. ���� ��� ������, �� � 0-�� ������ ����������� ����� ������
    static uint NumRecords();
};
