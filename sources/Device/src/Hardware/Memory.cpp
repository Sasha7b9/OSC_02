#include <stm32f4xx_hal.h>
#include "defines.h"
#include "Memory.h"
#include "Hardware/Beeper.h"
#include <cstring>

#include "Hardware/HAL/HAL.h"


using HAL::EEPROM_;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
static int GetSerialNumber(char buffer[17]);
/// ���������� ����� ������� ���������� ����� � ������� ��������
static uint FirstFreeAddressForSettings();
/// \brief ���������� ����� ���������� �������� ��� 0, ���� ��������� �� �����������. fromEnd ���������, ����� ��������� �� �����
/// ����� ��������� - 0 - ���������, 1 - ������������� � ��� �����
static uint AddressSavedSettings(int fromEnd);

static void WriteBytes(uint address, const uint8 *data, int size);

static void ReadBytes(uint address, void *data, uint size);

static uint ReadDoubleWord(uint address);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 2.
#define ADDR_SECTOR_SETTINGS_1  ((uint)0x080C0000)
/// ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 1.
#define ADDR_SECTOR_SETTINGS_2  ((uint)0x080E0000)

#define SIZE_SECTOR_128         (128 * 1024)

#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */


#define READ_BYTE(address)          (*((uint8 *)address))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Memory::SaveSettings()
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

    uint freeMemory = ADDR_SECTOR_SETTINGS_1 + SIZE_SECTOR_128 - address;

    if(address == MAX_UINT || freeMemory <= sizeof(Settings))
    {
        EEPROM_::EraseSector(ADDR_SECTOR_SETTINGS_1);
        address = ADDR_SECTOR_SETTINGS_1;
    }

    set.size = sizeof(set);
    WriteBytes(address, (uint8 *)&set, sizeof(set));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint FirstFreeAddressForSettings() //-V2506
{
    uint address = ADDR_SECTOR_SETTINGS_1;

    do
    {
        uint value = ReadDoubleWord(address);

        if (value == MAX_UINT)              // ��� ������� ��������, ��� �� ����� ������ ��� ������ �� ������������, ����� ����� ��� �� �������
        {                                   // ������ ��������� (Settings), ���� ������ ������ �������� �� ��� �����
            return address;
        }

        address += value;                   // ��������� �� ������ ��������� ���� �� ����������

    } while(address < (ADDR_SECTOR_SETTINGS_1 + SIZE_SECTOR_128));
    
    
    return MAX_UINT;        // ������-�� �� ���� ����� ����� ����� �� �����. ���� �� ��������� �����, ��������� ������
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint AddressSavedSettings(int)
{
    uint addrPrev = 0;

    uint address = ADDR_SECTOR_SETTINGS_1;

    while (ReadDoubleWord(address) != MAX_UINT)
    {
        addrPrev = address;
        address += ReadDoubleWord(address);
    }

    return addrPrev;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteBytes(uint address, const uint8 *data, int size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();
    
    for (int i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data[i]) != HAL_OK)
        {
            ERROR_HANDLER();
        }
        ++address;
    }

    HAL_FLASH_Lock();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadBytes(uint address, void *data, uint size)
{
    uint8 *buffer = (uint8 *)data;

    for (uint i = 0; i < size; i++)
    {
        buffer[i] = READ_BYTE(address++);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Memory::GetData(int /*num*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Memory::DeleteData(int /*num*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Memory::GetDataInfo(bool /*existData*/[MAX_NUM_SAVED_WAVES])
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Memory::SaveData(int /*num*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Memory::DeleteAllData()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int GetSerialNumber(char buffer[17]) //-V2506
{
    /// \todo �������� - ������ �������������� �������. ������ ������ ������ �������� ������� �����, ������� � ��� ��������, � �� 16, ��� ������.

    const int allShotsMAX = 512 / 16;   // ������������ ����� ������� � OTP ��������� ������.

    uint8 *address = (uint8 *)FLASH_OTP_END - 15; //-V566

    do
    {
        address -= 16;
    } while (*address == 0xff && address > (uint8 *)FLASH_OTP_BASE); //-V566

    if (*address == 0xff)   // �� ����� ������ � �����������, ����� �� ���������� ������ OTP
    {
        buffer[0] = 0;
        return allShotsMAX;
    }

    std::strcpy(buffer, (char *)address); //-V2513

    return allShotsMAX - (address - (uint8 *)FLASH_OTP_BASE) / 16 - 1; //-V566 //-V110
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String OTPmem::GetSerialNumber(int *freeForWrite)
{
    char buffer[20];
    *freeForWrite = ::GetSerialNumber(buffer);
    return String(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteBufferBytes(uint address, void *buffer, int size)
{
    Beeper::WaitForCompletion();

    CLEAR_FLASH_FLAGS

        HAL_FLASH_Unlock();
    for (int i = 0; i < size; i++)
    {
        uint64_t data = ((uint8 *)buffer)[i];
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data);
        address++;
    }
    HAL_FLASH_Lock();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool OTPmem::SaveSerialNumber(char *servialNumber) //-V2506
{
    // ������� ������ ������ ������ ������ 16 ���� � ������� OTP, ������� � ������
    uint8 *address = (uint8 *)FLASH_OTP_BASE; //-V566

    while ((*address) != 0xff &&                    // *address != 0xff ��������, ��� ������ � ��� ������ ��� �������������
           address < (uint8 *)FLASH_OTP_END - 16) //-V566
    {
        address += 16;
    }

    if (address < (uint8 *)FLASH_OTP_END - 16) //-V566
    {
        WriteBufferBytes((uint)address, (uint8 *)servialNumber, (int)std::strlen(servialNumber) + 1); //-V205
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint ReadDoubleWord(uint address)
{
    return (*((uint *)address));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Memory::LoadSettings()
{
    uint address = AddressSavedSettings(0);

    if (address && ReadDoubleWord(address) == sizeof(set))
    {
        ReadBytes(address, &set, ReadDoubleWord(address));
        return true;
    }

    return false;
}
