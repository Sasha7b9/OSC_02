#pragma once
#include "Data/DataSettings.h"


namespace FPGA
{
    class DataAccessor;
}

class Buffer;

class Reader;

class DataAccessor;
class DataOsciP2P;
class Storage;

class DataOsci
{
    friend class DataAccessor;
    friend class FPGA::DataAccessor;
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
protected:
    /// ��������� �� ������ ������� ������
    uint8 *dataA;
    /// ��������� �� ������ ������� ������
    uint8 *dataB;
    /// ��������� �� ���������� ���������� ������
    DataOsci *prev;
    /// ��������� �� ��������� ���������� ������
    DataOsci *next;
    /// ���������� ����� ������
    uint num;
    /// ������� ����� ������ ��� ����
    static uint allDatas;
    /// ��������� ��������
    DataSettings settings;
};

class DataOsciP2P
{
    friend class DataAccessor;
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
private:
    /// �����, � ������� ������ ������� ����� numPoint
    float TimePointMS(uint numPoint) const;
    /// ���������� true, ���� ������� ������ �����, ��� ��������� ��� ������� �������
    bool NeedAdditionPoints(uint timeMS) const;
    /// ���������� fromEnd-� ���� � ����� ������
    uint8 ByteFromEnd(Chan::E ch, int fromEnd);
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
    int pointerToByte;

    DataOsci data;
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
