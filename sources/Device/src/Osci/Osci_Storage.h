#pragma once
#include "Data/DataSettings.h"


namespace FPGA
{
    class DataAccessor;
}

class Buffer;

class Reader;

namespace Osci
{
    class DataAccessor;
    class DataP2P;
    class Storage;

    ////
    class Data
    {
    friend class DataAccessor;
    friend class FPGA::DataAccessor;
    friend class DataP2P;
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
        Data *prev;
        /// ��������� �� ��������� ���������� ������
        Data *next;
        /// ���������� ����� ������
        uint num;
        /// ������� ����� ������ ��� ����
        static uint allDatas;
        /// ��������� ��������
        DataSettings settings;
    };

    ////
    class DataP2P
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
    private:
        void Create();
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
        /// ����� ������ ����� ����������. ������ ����� ����� ���������� ����� �����, ��������������� TBase
        uint timeStart;

        Data data;
    };

    ////
    class Storage
    {
    public:
        /// ���������� ��������� �� ������
        static Data *GetData(int fromEnd = 0);
        /// �������. ����������� ����� ������������ ����� ������
        static void Clear();
        /// �������������� ����� � ��������� ��� ����� ������. ���������� ��������� �� ���������, ������� ����� ��������� �������
        static Data *PrepareForNewData();
        /// �������������� ����� ����� ��� �������� ������ ����������� ������
        static void PrepareNewFrameP2P();
        /// ���������� ����� ����������� ������
        static DataP2P *GetFrameP2P();

        static int NumElementsInStorage();
    };
}
