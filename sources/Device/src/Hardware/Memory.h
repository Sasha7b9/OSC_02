#pragma once
#include "Utils/String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                ///< ������� � ���� ����� ����� �����.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Memory
{
    void SaveSettings();

    bool LoadSettings();

    void DeleteAllData();

    bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    void DeleteData(int num);

    void SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    /// ���� ������� ����, ��������������� ������� ������� ����� true.
    void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// ���������� ��������� ������������� ��������� ������. � freeForWrite ���������� ��������� ���� ��� ������
    static String GetSerialNumber(int *freeForWrite);
};

