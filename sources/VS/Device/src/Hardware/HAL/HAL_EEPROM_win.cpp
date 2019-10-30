#include "defines.h"
#include "Hardware/Memory/Memory.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


bool operator >=(const Address &left, const Address &right)
{
    return (left.addressMC >= right.addressMC);
}

bool operator<(const Address &left, const Address &right)
{
    return (left.addressMC < right.addressMC);
}


uint8 eeprom[FlashMemory::SIZE];


struct SectorTypeDef
{
	Address address;
	uint8*  memory;
	uint    size;
	/// ���������� true, ���� ������ �������� ������ ������ � ������� addr
	bool IsConsist(Address addr) const
	{
		return (addr >= Address::FromPC(memory)) && (addr < Address::FromPC(memory + size));
	}

	void WriteByte(Address addr, uint8 value) const
	{
		if (IsConsist(addr))
		{
            *addr.ToPC() = value;
		}
	}
};


Address Address::FromPC(uint addr)
{
    uint deltaPC = addr - reinterpret_cast<uint>(&eeprom[0]);

    return Address(SEC_00_BOOT_1 + deltaPC);
}

Address Address::FromMC(uint addr)
{
    return Address(addr);
}

Address Address::FromPC(uint8 *addr)
{
    return FromPC(reinterpret_cast<uint>(addr));
}

uint8 *Address::ToPC() const
{
    uint deltaMC = addressMC - SEC_00_BOOT_1;

    return eeprom + deltaMC;
}


static const SectorTypeDef sectors[24] =
{
	{Address::FromMC(SEC_00_BOOT_1),     &eeprom[0],                     _16K},
	{Address::FromMC(SEC_01_BOOT_2),     &eeprom[_16K],                  _16K},
	{Address::FromMC(SEC_02),            &eeprom[_16K * 2],              _16K},
	{Address::FromMC(SEC_03),            &eeprom[_16K * 3],              _16K},
	{Address::FromMC(SEC_04),            &eeprom[_16K * 4],              _64K},
	{Address::FromMC(SEC_05_FIRM_1),     &eeprom[_128K],                 _128K},
	{Address::FromMC(SEC_06_FIRM_2),     &eeprom[_128K * 2],             _128K},
	{Address::FromMC(SEC_07_FIRM_3),     &eeprom[_128K * 3],             _128K},
	{Address::FromMC(SEC_08),            &eeprom[_128K * 4],             _128K},
	{Address::FromMC(SEC_09),            &eeprom[_128K * 5],             _128K},
	{Address::FromMC(SEC_10_SETTINGS_1), &eeprom[_128K * 6],             _128K},
	{Address::FromMC(SEC_11_SETTINGS_2), &eeprom[_128K * 7],             _128K},
	{Address::FromMC(SEC_12),            &eeprom[_128K * 8],             _16K},
	{Address::FromMC(SEC_13),            &eeprom[_128K * 8 + _16K],      _16K},
	{Address::FromMC(SEC_14),            &eeprom[_128K * 8 + _16K * 2],  _16K},
	{Address::FromMC(SEC_15),            &eeprom[_128K * 8 + _16K * 3],  _16K},
	{Address::FromMC(SEC_16),            &eeprom[_128K * 8 + _16K * 4],  _64K},
	{Address::FromMC(SEC_17_RECORDER_1), &eeprom[_128K * 8 + _128K],     _128K},
	{Address::FromMC(SEC_18_RECORDER_2), &eeprom[_128K * 8 + _128K * 2], _128K},
	{Address::FromMC(SEC_19_DATA_1),     &eeprom[_128K * 8 + _128K * 3], _128K},
	{Address::FromMC(SEC_20_DATA_2),     &eeprom[_128K * 8 + _128K * 4], _128K},
	{Address::FromMC(SEC_21_DATA_3),     &eeprom[_128K * 8 + _128K * 5], _128K},
	{Address::FromMC(SEC_22_DATA_4),     &eeprom[_128K * 8 + _128K * 6], _128K},
	{Address::FromMC(SEC_23_DATA_5),     &eeprom[_128K * 8 + _128K * 7], _128K}
};

const uint HAL_FLASH::Sector::address[HAL_FLASH::Sector::NUM_SECTORS] =
{
    reinterpret_cast<uint>(&eeprom[0]),                     // 00_BOOT_1
    reinterpret_cast<uint>(&eeprom[_16K]),                  // 01_BOOT_2
    reinterpret_cast<uint>(&eeprom[_16K * 2]),              // 02
    reinterpret_cast<uint>(&eeprom[_16K * 3]),              // 03
    reinterpret_cast<uint>(&eeprom[_16K * 4]),              // 04
    reinterpret_cast<uint>(&eeprom[_128K]),                 // 05_FIRM_1
    reinterpret_cast<uint>(&eeprom[_128K * 2]),             // 06_FIRM_2
    reinterpret_cast<uint>(&eeprom[_128K * 3]),             // 07_FIRM_3
    reinterpret_cast<uint>(&eeprom[_128K * 4]),             // 08
    reinterpret_cast<uint>(&eeprom[_128K * 5]),             // 09
    reinterpret_cast<uint>(&eeprom[_128K * 6]),             // 10_SETTINGS_1
    reinterpret_cast<uint>(&eeprom[_128K * 7]),             // 11_SETTINGS_2
    reinterpret_cast<uint>(&eeprom[_128K * 8]),             // 12
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K]),      // 13
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 2]),  // 14
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 3]),  // 15
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 4]),  // 16
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K]),     // 17_RECORDER_1
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 2]), // 18_RECORDER_2
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 3]), // 19_DATA_1
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 4]), // 20_DATA_2
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 5]), // 21_DATA_3
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 6]), // 22_DATA_4
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 7])  // 23_DATA_5
};

const uint HAL_FLASH::Sector::size[HAL_FLASH::Sector::NUM_SECTORS] =
{
    _16K,   // 00_BOOT_1
    _16K,   // 01_BOOT_2
    _16K,   // 02
    _16K,   // 03
    _64K,   // 04
    _128K,  // 05_FIRM_1
    _128K,  // 06_FIRM_2
    _128K,  // 07_FIRM_3
    _128K,  // 08
    _128K,  // 09
    _128K,  // 10_SETTINGS_1
    _128K,  // 11_SETTINGS_2
    _16K,   // 12
    _16K,   // 13
    _16K,   // 14
    _16K,   // 15
    _64K,   // 16
    _128K,  // 17_RECORDER_1
    _128K,  // 18_RECORDER_2
    _128K,  // 19_DATA_1
    _128K,  // 20_DATA_2
    _128K,  // 21_DATA_3
    _128K,  // 22_DATA_4
    _128K   // 23_DATA_5
};


/// ���������� ������ �� ��������� �������� ������� � ��������� ������� address
static const SectorTypeDef &GetSector(Address address)
{
    const SectorTypeDef *result = &sectors[0];

    for (int i = 0; i < 24; i++)
    {
        const SectorTypeDef &sector = sectors[i];

        if (sector.IsConsist(address))
        {
            result = &sector;
            break;
        }
    }

    return *result;
}


void HAL_FLASH::WriteBufferBytes(uint address, const void *buffer, int size)
{
    WriteBytes(address, static_cast<const uint8 *>(buffer), size);
}


void HAL_FLASH::EraseSector(uint address)
{
    const SectorTypeDef &sector = GetSector(Address::FromMC(address));

    std::memset(sector.memory, 0xFF, sector.size);
}


void HAL_FLASH::WriteBytes(uint address, const uint8 *buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        const SectorTypeDef &sector = GetSector(Address::FromMC(address));

        sector.WriteByte(Address::FromMC(address++), *buffer++);
    }
}