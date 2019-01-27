#pragma once
#include "Utils/String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                ///< ������� � ���� ����� ����� �����.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EEPROM
{
public:

    static void SaveSettings();

    static bool LoadSettings();

    static void DeleteAllData();

    static bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    static void DeleteData(int num);

    static void SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    /// ���� ������� ����, ��������������� ������� ������� ����� true.
    static void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// ���������� ��������� ������������� ��������� ������. � freeForWrite ���������� ��������� ���� ��� ������
    static String GetSerialNumber(int *freeForWrite);
};

