#include "defines.h"
#include "HAL.h"
#include "Hardware/Beeper.h"
#include <stm32f4xx_hal.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint GetSector(uint strtAddress);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::EEPROM_::EraseSector(uint startAddress)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef isFLASH =
    {
        FLASH_TYPEERASE_SECTORS,
        0,
        GetSector(startAddress),
        1,
        FLASH_VOLTAGE_RANGE_3
    };

    uint error = 0;

    if (HAL_FLASHEx_Erase(&isFLASH, &error) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_FLASH_Lock();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint GetSector(uint startAddress)
{
    if (startAddress == 0x080C0000U)
    {
        return FLASH_SECTOR_10;
    }
    else if (startAddress == 0x080E0000U)
    {
        return FLASH_SECTOR_11;
    }
    else
    {
        // ������ �� ������
    }


    return FLASH_SECTOR_0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::EEPROM_::WriteBytes(uint address, const uint8 *data, int size)
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
void HAL::EEPROM_::WriteBufferBytes(uint address, void *buffer, int size)
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
