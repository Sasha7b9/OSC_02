#pragma once
#include "Utils/String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 2.
#define ADDR_SECTOR_SETTINGS_1  ((uint)0x080C0000)
#define ADDR_SECTOR_RECORDER_1  ((uint)0x08120000)
#define ADDR_SECTOR_RECORDER_2  ((uint)0x08140000)


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
    /// ������� ������ � ��������� ������� address
    void EraseSector(uint address);

    void WriteData(uint address, const void *data, int size);

    bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    void DeleteData(int num);

    void SaveData(int num, const DataSettings *ds, uint8 *dataA, uint8 *dataB);
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

