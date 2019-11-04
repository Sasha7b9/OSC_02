#pragma once
#include "Utils/String.h"


struct DataSettings;


struct FlashMemory
{
	static const uint SIZE = 2 * 1024 * 1024;

    static void Write(uint address, const void *data, int size);

    /// ������� ��� ����������/�������������� ��������
    struct Settings
    {
        static void Save();

        static bool Load();
    };

    /// ������� ��� ����������/�������������� ������
    struct Data
    {
        static const int MAX_NUM_SAVED_WAVES = 23;  ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                                    ///< ������� � ���� ����� ����� �����.

        /// ���� ������� ����, ��������������� ������� ������� ����� true.
        static void GetInfo(bool existData[MAX_NUM_SAVED_WAVES]);

        static void Save(int numInROM, const DataSettings *ds, uint8 *dataA, uint8 *dataB);
        /// �������� ������ � ������� ����� num. ���� ������� ���, � ds ������������ 0
        static bool Read(int numInROM, DataSettings **ds);

        static void Delete(int numInROM);

        static void DeleteAll();
    };
};



class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// ���������� ��������� ������������� ��������� ������. � freeForWrite ���������� ��������� ���� ��� ������
    static String GetSerialNumber(int *freeForWrite);
};

