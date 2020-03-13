#pragma once
#include "defines.h"
#include <ff.h>

// Key configuration
#define KEY_START_ADDRESS   (uint)0x0
#define KEY_PAGE_NUMBER     20
#define KEY_VALUE           0xAAAA5555

// Flash configuration
#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8020000
#define MAIN_PROGRAM_PAGE_NUMBER    21
#define NUM_OF_PAGES                256
#define FLASH_PAGE_SIZE             2048

#define TIME_WAIT   5000    // ����� ������ ��������


struct State
{
    enum E
    {
        Start,
        Mount,            // ������������ ������
        WrongFlash,       // ������ ����, �� ��������� ������
        RequestAction,    // ��� ������ - ���������� ��� ���
        NotFile,          // ���� ���� �������������, �� ���������� �� �� ����
        Upgrade,          // ������� ��������
        Ok                // ���������� ������ ���������
    };
};

typedef enum
{
    StateDisk_Idle,
    StateDisk_Start
} StateDisk;

typedef struct
{
    bool    isRun;
    uint8   notUsed[3];
    float   value;
    float   direction;
    uint    timePrev;
} DisplayStr;

typedef struct
{
    FATFS       USBDISKFatFS;
    char        USBDISKPath[4];
    StateDisk   state;
    uint8       notUsed[3];
    FIL         file;
    int         connection;
    int         active;
} FlashDrive;

typedef struct
{
    FlashDrive  drive;
    DisplayStr  display;
    uint8       notUsed[3];
} MainStruct;


// ������ ��������� ������������ �� ���� ������� ��������� ��� ���������� ������� ���
// ������ ��� �� ������ ���� �������� � ������� malloc � ������ ��������� � ���������� � ������ �������� �� �������� ���������
extern MainStruct *ms; //-V707
