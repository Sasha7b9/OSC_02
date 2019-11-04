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

        /// �������� ������ � ������� ����� num. ���� ������� ���, � ds ������������ 0
        static bool Get(int num, DataSettings **ds, uint8 **dataA, uint8 **dataB);

        static void Delete(int num);

        static void DeleteAll();

        static void Save(int num, const DataSettings *ds, uint8 *dataA, uint8 *dataB);
        /// ���� ������� ����, ��������������� ������� ������� ����� true.
        static void GetInfo(bool existData[MAX_NUM_SAVED_WAVES]);
    };
};



class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// ���������� ��������� ������������� ��������� ������. � freeForWrite ���������� ��������� ���� ��� ������
    static String GetSerialNumber(int *freeForWrite);
};

