#pragma once
#include "Utils/String.h"


#define SEC_00_BOOT_1     ((uint)0x08000000)   //  16k
#define SEC_01_BOOT_2     ((uint)0x08004000)   //  16k
#define SEC_02            ((uint)0x08008000)   //  16k
#define SEC_03            ((uint)0x0800C000)   //  16k
#define SEC_04            ((uint)0x08010000)   //  64k
#define SEC_05_FIRM_1     ((uint)0x08020000)   // 128k
#define SEC_06_FIRM_2     ((uint)0x08040000)   // 128k
#define SEC_07_FIRM_3     ((uint)0x08060000)   // 128k
#define SEC_08            ((uint)0x08080000)   // 128k
#define SEC_09            ((uint)0x080A0000)   // 128k
/// ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 2.
#define SEC_10_SETTINGS_1 ((uint)0x080C0000)   // 128k
#define SEC_11_SETTINGS_2 ((uint)0x080E0000)   // 128k
#define SEC_12            ((uint)0x08100000)   //  16k
#define SEC_13            ((uint)0x08104000)   //  16k
#define SEC_14            ((uint)0x08108000)   //  16k
#define SEC_15            ((uint)0x0810C000)   //  16k
#define SEC_16            ((uint)0x08110000)   //  64k
#define SEC_17_RECORDER_1 ((uint)0x08120000)   // 128k
#define SEC_18_RECORDER_2 ((uint)0x08140000)   // 128k
#define SEC_19_DATA_1     ((uint)0x08160000)   // 128k
#define SEC_20_DATA_2     ((uint)0x08180000)   // 128k
#define SEC_21_DATA_3     ((uint)0x081A0000)   // 128k
#define SEC_22_DATA_4     ((uint)0x081C0000)   // 128k
#define SEC_23_DATA_5     ((uint)0x081E0000)   // 128k


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

    /// ������� ������ � ��������� ������� address
    static void EraseSector(uint address);

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

