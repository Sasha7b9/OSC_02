#pragma once
#include "Data/DataSettings.h"


namespace FPGA
{
    class DataAccessor;
    class StorageAccessor;
}

class Reader;

namespace Osci
{
    class DataAccessor;
    class StorageAccess;
    class DataP2P;
    class Storage;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Data
    {
    friend class DataAccessor;
    friend class FPGA::DataAccessor;
    friend class DataP2P;
    public:
        void Create();
        const uint8 *DataA();
        const uint8 *DataB();
        const DataSettings *Settings();
    protected:
        /// ��������� �� ������ ������� ������
        uint8 *dataA;
        /// ��������� �� ������ ������� ������
        uint8 *dataB;
        /// ���������� ����� ������
        uint num;
        /// ������� ����� ������ ��� ����
        static uint allDatas;
        /// ��������� ��������
        DataSettings settings;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class DataP2P
    {
    friend class DataAccessor;
    friend class Storage;
    public:
        void Logging() const;
        /// time - �����, ����� ���� ������� �����
        void AddPoints(uint timeMS, BitSet16 pointA, BitSet16 pointB);
    private:
        void Create();
        /// �����, � ������� ������ ������� ����� numPoint
        float TimePointMS(uint numPoint) const;
        /// ���������� true, ���� ������� ������ �����, ��� ��������� ��� ������� �������
        bool NeedAdditionPoints(uint timeMS) const;
        /// ���������� ��������� �����
        uint readingPoints;
        /// ��������� �� ��������� �����, ������� ����� ����������� ���������
        uint pointer;
        /// ����� ������ ����� ����������. ������ ����� ����� ���������� ����� �����, ��������������� TBase
        uint timeStart;

        Data data;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Storage
    {
    friend class FPGA::StorageAccessor;
    friend class ::Reader;
    friend class StorageAccessor;
    private:
        /// �������������� ����� � ��������� ��� ����� ������. ���������� ��������� �� ���������, ������� ����� ��������� �������
        static Data *PrepareForNewData();
        /// ���������� ��������� �� ������
        static Data *GetData(int fromEnd = 0);
    public:
        /// �������������� ����� ����� ��� �������� ������ ����������� ������
        static void PrepareNewFrameP2P();
        /// ���������� ����� ����������� ������
        static DataP2P *GetFrameP2P();
    };
}
