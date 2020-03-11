#pragma once
#include "Utils/String.h"


struct DataSettings;
struct SettingsNRST;


struct ROM
{
	static const uint SIZE = 2 * 1024 * 1024;

    // ������� ��� ����������/�������������� ��������
    struct Settings
    {
        static void Save();

        static bool Load();

        // ���������� ����� ������� ���������� ����� � ������� ��������
        static uint FirstFreeAddressForSettings();

        // \brief ���������� ����� ���������� �������� ��� 0, ���� ��������� �� �����������. fromEnd ���������, ����� ��������� �� �����
        // ����� ��������� - 0 - ���������, 1 - ������������� � ��� �����
        static uint AddressSavedSettings(int fromEnd);

        static void ReadBytes(uint address, void *data, uint size);

        static uint ReadDoubleWord(uint address);
    };

    // ������� ��� ����������/�������� �������������� ������������� ��������
    struct NRST
    {
        // ���������� ��������� �� ���������� � ROM �������������� ���������. nullptr � ������, ���� �������� ��� ���
        static SettingsNRST *GetSaved();
        // ��������� ��������� �� ������ nrst � ROM
        static void Save(SettingsNRST *nrst);
    };

    // ������� ��� ����������/�������������� ������
    struct Data
    {
        static const uint MAX_NUM_SAVED_WAVES = 23; ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                                    ///< ������� � ���� ����� ����� �����.

        // ���� ������� ����, ��������������� ������� ������� ����� true.
        static void GetInfo(bool existData[MAX_NUM_SAVED_WAVES]);

        static void Save(uint numInROM, const DataSettings *ds);
        
        // �������� ������ � ������� ����� num. ���� ������� ���, � ds ������������ 0
        static const DataSettings *Read(uint numInROM);

        static void Erase(uint numInROM);

        static void EraseAll();
    };
};



struct OTP
{
    static bool SaveSerialNumber(char *servialNumber);
    
    // ���������� ��������� ������������� ��������� ������. � freeForWrite ���������� ��������� ���� ��� ������
    static String GetSerialNumber(int *freeForWrite);
};

