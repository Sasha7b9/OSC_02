#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"


#define READ_BYTE(address)      (*((uint8 *)address))


void ROM::Settings::Save()
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

    HAL_ROM::WriteBufferBytes(address, &set, sizeof(set));
}


uint ROM::Settings::FirstFreeAddressForSettings() //-V2506
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


uint ROM::Settings::AddressSavedSettings(int)
{
    uint addrPrev = 0;

    uint address = ADDR_SECTOR(Sector::_10_SETTINGS_1);

    while (ReadDoubleWord(address) != MAX_UINT)
    {
        if(address < ADDR_SECTOR(Sector::_10_SETTINGS_1) || address > ADDR_SECTOR(Sector::_10_SETTINGS_1) + 128 * 1024)
        {
            address = address;
        }

        addrPrev = address;
        
        uint offset = ReadDoubleWord(address);
        
        address += offset;
    }

    return addrPrev;
}


void ROM::Settings::ReadBytes(uint address, void *data, uint size)
{
    uint8 *buffer = static_cast<uint8 *>(data);

    for (uint i = 0; i < size; i++)
    {
        buffer[i] = READ_BYTE(address++);
    }
}

String OTP::GetSerialNumber(int *freeForWrite)
{
    char buffer[20];
    *freeForWrite = HAL_OTP::GetSerialNumber(buffer);
    return String(buffer);
}


bool OTP::SaveSerialNumber(char *servialNumber)
{
    return HAL_OTP::SaveSerialNumber(servialNumber);
}


uint ROM::Settings::ReadDoubleWord(uint address)
{
    return *(reinterpret_cast<uint *>(address));
}


bool ROM::Settings::Load()
{
    uint size = sizeof(set);
    size = size;

    uint address = AddressSavedSettings(0);
    
    if (address && ReadDoubleWord(address) == sizeof(set))
    {
        ReadBytes(address, &set, ReadDoubleWord(address));
        return true;
    }

    return false;
}
