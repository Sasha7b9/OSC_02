#include "defines.h"
#include "Hardware/Memory/MemoryROM.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"


/// ���������� ����� ������� ���������� ����� � ������� ��������
static uint FirstFreeAddressForSettings();
/// \brief ���������� ����� ���������� �������� ��� 0, ���� ��������� �� �����������. fromEnd ���������, ����� ��������� �� �����
/// ����� ��������� - 0 - ���������, 1 - ������������� � ��� �����
static uint AddressSavedSettings(int fromEnd);

static void ReadBytes(uint address, void *data, uint size);

static uint ReadDoubleWord(uint address);


#define READ_BYTE(address)      (*((uint8 *)address))



void MemoryROM::Settings::Save()
{
    /*
        �������� ���������� ������� ���������� �����
        1. ������� ����� = (uint*)ADDR_SECTOR_SETTINGS_1.
        2. �������, ��� �������� �� �������� ������
        2. ���� ��� 0xffffffff, �� ��� ������ ��������� ����� - �������.
        3. ���������� � �������� ������ �������� *((uint*)ADDR_SECTOR_SETTINGS_1).
        4. ��������� � 2
    */
    /*
        1. ���������� � Settings.size �������� sizeof(Settings)
        2. ������� ����� ������� ���������� ����� (�������� ����)
        3. ������� ������_���������_������ = ADDR_SECTOR_SETTINGS_1 + (128 * 1024) - �����_�������_����������_�����
        4. ���� ������_���������_������ > ������_���������_��������, �� ��������� �� ���������� ������ ��������� Settings � �������.
        5. ����� ������� ������ �������� � ���������� � ��� ������ ��������� Settings
    */

    /// \todo ������ ������� ��� �������� �������� ������������, ������ ��� ����� �������� ����� ����������� ������� � ����� �������� ��� ���������

    set.size = sizeof(set);

    uint address = FirstFreeAddressForSettings();

    uint freeMemory = END_SECTOR(Sector::_10_SETTINGS_1) - address;

    if((address == MAX_UINT) || (freeMemory <= sizeof(Settings)) || (address < ADDR_SECTOR(Sector::_10_SETTINGS_1)))
    {
        SECTOR(Sector::_10_SETTINGS_1).Erase();

        address = ADDR_SECTOR(Sector::_10_SETTINGS_1);
    }

    set.size = sizeof(set);

    HAL_FLASH::WriteBufferBytes(address, &set, sizeof(set));
}


static uint FirstFreeAddressForSettings() //-V2506
{
    uint address = ADDR_SECTOR(Sector::_10_SETTINGS_1);

    do
    {
        uint value = ReadDoubleWord(address);

        if (value == MAX_UINT)              // ��� ������� ��������, ��� �� ����� ������ ��� ������ �� ������������, ����� ����� ��� �� �������
        {                                   // ������ ��������� (Settings), ���� ������ ������ �������� �� ��� �����
            return address;
        }

        address += value;                   // ��������� �� ������ ��������� ���� �� ����������

    } while (address < END_SECTOR(Sector::_10_SETTINGS_1));
    
    return MAX_UINT;        // ������-�� �� ���� ����� ����� ����� �� �����. ���� �� ��������� �����, ��������� ������
}


static uint AddressSavedSettings(int)
{
    uint addrPrev = 0;

    uint address = ADDR_SECTOR(Sector::_10_SETTINGS_1);

    while (ReadDoubleWord(address) != MAX_UINT)
    {
        addrPrev = address;
        address += ReadDoubleWord(address);
    }

    return addrPrev;
}


static void ReadBytes(uint address, void *data, uint size)
{
    uint8 *buffer = static_cast<uint8 *>(data);

    for (uint i = 0; i < size; i++)
    {
        buffer[i] = READ_BYTE(address++);
    }
}

String MemoryOTP::GetSerialNumber(int *freeForWrite)
{
    char buffer[20];
    *freeForWrite = HAL_OTP::GetSerialNumber(buffer);
    return String(buffer);
}


bool MemoryOTP::SaveSerialNumber(char *servialNumber)
{
    return HAL_OTP::SaveSerialNumber(servialNumber);
}


static uint ReadDoubleWord(uint address)
{
    return *(reinterpret_cast<uint *>(address));
}


bool MemoryROM::Settings::Load()
{
    uint address = AddressSavedSettings(0);

    if (address && ReadDoubleWord(address) == sizeof(set))
    {
        ReadBytes(address, &set, ReadDoubleWord(address));
        return true;
    }

    return false;
}
