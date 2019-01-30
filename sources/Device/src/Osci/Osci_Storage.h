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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Data
    {
    friend class DataAccessor;
    friend class FPGA::DataAccessor;
    public:
        void Create();
        const uint8 *DataA();
        const uint8 *DataB();
        const DataSettings *Settings();
    private:
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
    class Storage
    {
    friend class FPGA::StorageAccessor;
    friend class ::Reader;
    private:
        /// �������������� ����� � ��������� ��� ����� ������. ���������� ��������� �� ���������, ������� ����� ��������� �������
        static Data *PrepareForNewData();
        /// ���������� ��������� �� ������
        static Data *GetData(int fromEnd = 0);
    };
}
