#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"


#define READ_BYTE(address)  (*((uint8 *)address))
#define SIZE_RECORD         512                      // ������ ����� ������


class Record;

struct SectorSet
{
    Record *FirstRecord();
    Sector sector;
};


static SectorSet sectorFirst = { Sector::Get(Sector::_10_SETTINGS_1) };
static SectorSet sectorSecond = { Sector::Get(Sector::_11_SETTINGS_2) };


class Record
{
public:
    Record *Next();
    SectorSet &
    Settings set;
};


// ���������� ��������� �� ��������� ���������� ������
static Record *LastRecord();


void ROM::Settings::Save()
{
    Record *record = sectorFirst.FirstRecord();         // ����� ��� ������ ��������

    Record *last = LastRecord();                        // ��������� �� ��������� ���������� ���������

    if(last)
    {
        Record *record = last->Next();                  // ������� ����� ��������� �� last ������� � ��� �� �������

        if(!record)                                     // ���� ������ ����� �� �������
        {

        }
    }

    set.number = record->set.number + 1;                // ���� ������ ������, �� ����� ����� ����� 0 = 0xFFFFFFFF + 1
    set.crc32 = set.CalcWriteCRC32();
    HAL_ROM::WriteBufferBytes(reinterpret_cast<uint>(record), &set, sizeof(set));
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


bool ROM::Settings::Load()
{
    return false;
}
