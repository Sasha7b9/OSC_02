#pragma once
#include "Utils/String.h"


#ifdef GUI

struct SectorTypeDef
{
    uint address;
    uint8 *memory;
    uint size;
    /// ���������� true, ���� ������ �������� ������ ������ � ������� addr
    bool IsConsist(uint addr) const;

    void WriteByte(uint addr, uint8 value) const;
};

extern const SectorTypeDef sectors[];

#define ADDR_SECTOR_SETTINGS_1 ((uint)sectors[10].memory)
#define ADDR_SECTOR_RECORDER_1 ((uint)sectors[17].memory)

#else
/// ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 2.
#define ADDR_SECTOR_SETTINGS_1  ((uint)0x080C0000)
#define ADDR_SECTOR_RECORDER_1  ((uint)0x08120000)
#define ADDR_SECTOR_RECORDER_2  ((uint)0x08140000)
#endif



struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                ///< ������� � ���� ����� ����� �����.


struct EEPROM
{
    static void SaveSettings();

    static bool LoadSettings();

    static void DeleteAllData();
    /// ������� ������ � ��������� ������� address
    static void EraseSector(uint address);

    static void WriteData(uint address, const void *data, int size);

    static bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    static void DeleteData(int num);

    static void SaveData(int num, const DataSettings *ds, uint8 *dataA, uint8 *dataB);
    /// ���� ������� ����, ��������������� ������� ������� ����� true.
    static void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);
};



class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// ���������� ��������� ������������� ��������� ������. � freeForWrite ���������� ��������� ���� ��� ������
    static String GetSerialNumber(int *freeForWrite);
};

