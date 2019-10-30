#pragma once
#include "Utils/String.h"


#define S00_BOOT_1     ((uint)0x08000000)   //  16k
#define S01_BOOT_2     ((uint)0x08004000)   //  16k
#define S02            ((uint)0x08008000)   //  16k
#define S03            ((uint)0x0800C000)   //  16k
#define S04            ((uint)0x08010000)   //  64k
#define S05_FIRM_1     ((uint)0x08020000)   // 128k
#define S06_FIRM_2     ((uint)0x08040000)   // 128k
#define S07_FIRM_3     ((uint)0x08060000)   // 128k
#define S08            ((uint)0x08080000)   // 128k
#define S09            ((uint)0x080A0000)   // 128k
/// ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 2.
#define S10_SETTINGS_1 ((uint)0x080C0000)   // 128k
#define S11_SETTINGS_2 ((uint)0x080E0000)   // 128k
#define S12            ((uint)0x08100000)   //  16k
#define S13            ((uint)0x08104000)   //  16k
#define S14            ((uint)0x08108000)   //  16k
#define S15            ((uint)0x0810C000)   //  16k
#define S16            ((uint)0x08110000)   //  64k
#define S17_RECORDER_1 ((uint)0x08120000)   // 128k
#define S18_RECORDER_2 ((uint)0x08140000)   // 128k
#define S19            ((uint)0x08160000)   // 128k
#define S20            ((uint)0x08180000)   // 128k
#define S21            ((uint)0x081A0000)   // 128k
#define S22            ((uint)0x081C0000)   // 128k
#define S23            ((uint)0x081E0000)   // 128k


struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                ///< ������� � ���� ����� ����� �����.


/// ��������� ��������� ����� � �������� ������������ �����������
struct Address
{
    uint addressMC;
    /// ���������� ����� �� ��������� ������������ �����������, ������������ �� ������ PC
    static Address FromPC(uint addr);
    static Address FromPC(uint8 *addr);
    /// ��������� ����� �� ��������� ������������ ����������������
    static Address FromMC(uint addr);
    /// ����������� � ����� PC
    uint8 *ToPC() const;
private:
    explicit Address(uint addr) : addressMC(addr) {};
};

struct FlashMemory
{
	static const uint SIZE = 2 * 1024 * 1024;

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

