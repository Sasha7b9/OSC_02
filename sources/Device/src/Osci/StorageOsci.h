#pragma once
#include "Data/DataSettings.h"


class Buffer;
class Reader;
class DataOsciP2P;
class Storage;

class DataOsci
{
    friend class DataOsciP2P;
    friend class HeapWorker;
public:
    void Create();
    const uint8 *DataA();
    const uint8 *DataB();
    const uint8 *GetData(Chan::E ch);
    const DataSettings *Settings();
    uint Number() const { return num; };
    /// ���������� �����, ���������� ���������� ������ � �������
    uint FullSize() const;
    /// ��������� �� ������ ������� ������
    uint8 *dataA;
    /// ��������� �� ������ ������� ������
    uint8 *dataB;
    /// ��������� ��������
    DataSettings settings;
protected:
    /// ��������� �� ���������� ���������� ������
    DataOsci *prev;
    /// ��������� �� ��������� ���������� ������
    DataOsci *next;
    /// ���������� ����� ������
    uint num;
    /// ������� ����� ������ ��� ����
    static uint allDatas;
};

class DataOsciP2P
{
    friend class Storage;
    friend class StructDataP2P;
    friend class ::Reader;
public:
    /// time - �����, ����� ���� ������� �����
    void AddPoints(BitSet16 pointA, BitSet16 pointB);
    const uint8 *DataA();
    const uint8 *DataB();
    const DataSettings *Settings();
    /// ��������� buffer ������� ��� ������ �� �����
    void FillBufferForDraw(Chan::E ch, Buffer *buffer);

    void Create();
    /// ����� ������ ����� ����������. ������ ����� ����� ���������� ����� �����, ��������������� TBase
    uint timeStart;

    DataOsci data;
private:
    /// �����, � ������� ������ ������� ����� numPoint
    float TimePointMS(uint numPoint) const;
    /// ���������� true, ���� ������� ������ �����, ��� ��������� ��� ������� �������
    bool NeedAdditionPoints(uint timeMS) const;
    /// ���������� fromEnd-� ���� � ����� ������
    uint8 ByteFromEnd(Chan::E ch, uint fromEnd);
    /// ���������� ���������� ����������� ����
    uint ReadingBytes() const;
    /// ��������� ����� ��� ��������� ��� ���������� ���������
    void FillBufferForPeakDetEnabled(Chan::E ch, Buffer *buffer);
    /// ��������� ����� ��� ��������� ��� ����������� ���������
    void FillBufferForPeakDetDisabled(Chan::E ch, Buffer *buffer);
    /// ����������� ����� � ���������� �������
    void PrepareBuffer(Buffer *buffer, uint size);
    /// ���������� ��������� �����
    uint readingPoints;
    /// ��������� �� ��������� �����, ������� ����� ������� ���������
    uint pointerToByte;
};

class StorageOsci
{
public:
    /// ���������� ��������� �� ������
    static DataOsci *GetData(int fromEnd = 0);
    /// �������. ����������� ����� ������������ ����� ������
    static void Clear();
    /// �������������� ����� � ��������� ��� ����� ������. ���������� ��������� �� ���������, ������� ����� ��������� �������
    static DataOsci *PrepareForNewData();
    /// �������������� ����� ����� ��� �������� ������ ����������� ������
    static void PrepareNewFrameP2P();
    /// ���������� ����� ����������� ������
    static DataOsciP2P *GetFrameP2P();

    static int NumElementsInStorage();
};
