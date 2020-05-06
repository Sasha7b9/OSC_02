#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageTime.h"


// ����������� ������������ � ���������� �����
#define ENUM_TO_REL_POINTS(enum)    ((int)(1 << (static_cast<int>(enum) + 9)))


// ����� ������.
#define S_MEM_MODE_WORK         (set.mem._modeWork)
#define S_MEM_MODE_WORK_IS_DIR  (S_MEM_MODE_WORK == ModeWork::Dir)
#define S_MEM_MODE_WORK_IS_RAM  (S_MEM_MODE_WORK == ModeWork::RAM)
#define S_MEM_MODE_WORK_IS_ROM  (S_MEM_MODE_WORK == ModeWork::ROM)
struct ModeWork
{
    enum E
    {
        Dir,        // �������� �����.
        RAM,        // � ���� ������ ����� ����������� ��������� ���������� ���������.
        ROM,        // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������.
        Count       // ������������ � ������ Data.c. �����, ����� �������, ��� ������ �� �������� �� �� ����� �����.
    };
};


// ��� ������ ��� ������� ������ ������.
#define S_MEM_MODE_BTN_MEMORY   (set.mem._modeBtnMemory)
struct ModeBtnMemory
{
    enum E
    {
        Menu,     // ����� ������������ ��������������� �������� ����.
        Save      // ���������� ����������� ������ �� ������.
    };
};


// ��� ��������� ������ �� ������.
struct ModeSaveSignal
{
    enum E
    {
        BMP,
        TXT
    };
};


// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  // ���������� ������ ��������� �������.
        Saved,   // ���������� ���������� ������.
        Both     // ���������� ��� �������.
    };
};


// ����� ������������ ������.
struct FileNamingMode
{
    enum E
    {
        Mask,        // ��������� �� �����.
        Manually     // ��������� �������.
    };
};


// ����� ����� �������, � ������� ��� ������.
#define S_ENUMPOINTS_FPGA   (set.mem._enumPoints)
struct ENumPointsFPGA
{
    enum E
    {
        _512,
        _1k,
        _2k,
        _4k,
        _8k,
        Count
    };
    static int BytesInChannel();
    static int PointsInChannel();
};


// ����� ������ ���������� � ������ ������ - ����� ��
#define S_MEM_TYPE_SIGNAL_ROM               (set.mem._typeSignalROM)
#define S_MEM_TYPE_SIGNAL_ROM_IS_CURRENT    (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Current)
#define S_MEM_TYPE_SIGNAL_ROM_IS_RECORDED   (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Recorded)
#define S_MEM_TYPE_SIGNAL_ROM_IS_BOTH       (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Both)
struct TypeSignalROM
{
    enum E
    {
        Recorded,
        Current,
        Both,
        Count
    };
};

#define MAX_SYMBOLS_IN_FILE_NAME 35

#define S_MEM_FILE_NAME             (set.mem.fileName)
#define S_MEM_FLASH_AUTO_CONNECT    (set.mem._flashAutoConnect)
#define S_MEM_INDEX_CUR_SYMBOL_MASK (set.mem._indexCurSymbolNameMask)

struct SettingsMemory
{
    ENumPointsFPGA::E   _enumPoints;                             // ����� �����
    ModeBtnMemory::E    _modeBtnMemory;
    TypeSignalROM::E    _typeSignalROM;                          // ����� ������ �������� � ������ ������-����� ��
    ModeWork::E         _modeWork;                               // ����� ������.
    bool                _flashAutoConnect;                       // ���� true, ��� ����������� ������ ������������� ��������� ����-��������.
    int8                _indexCurSymbolNameMask;                 // ������ �������� ������� � ������ ������� ����� ��� ������ �����.
    ModeSaveSignal::E   modeSaveSignal;                         // � ����� ���� ��������� ������.
    char                fileName[MAX_SYMBOLS_IN_FILE_NAME];     // ��� ����� ��� ������ ������� �������.
    ModeShowIntMem::E   _;                                      // ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
    FileNamingMode::E   fileNamingMode;                         // ����� ���������� ������.
    char                fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; // \brief ����� ����� ��� ��������������� ���������� ������.
                        // \details ������� ����������.\n
                        // \code
                        // %y('\x42') - ���, %m('\x43') - �����, %d('\x44') - ����, %H('\x45') - ����, %M('\x46') - ������, %S('\x47') - �������
                        // %Nn('\x48''n') - ���������� �����, ������ �������� �� ����� n ���������, ��������, 7 � %3N ����� ������������� � 007
                        // �������
                        // name_%4N_%y_%m_%d_%H_%M_%S ����� ������������ ����� ���� name_0043_2014_04_25_14_45_32
                        // ��� ���� �������� ��������, ��� ���� ������������ %4N ����� ����� ��������� ����������, ��, ������ �����, ���� �������� 
                        // ����� ������ ����� 0001, �.�. ��� ����������� ������ ���������������.
                        // \endcode
};


struct PageMemory
{
    static void OnChanged_Points(bool active);

    static void OnOpenClose_Drive_Manager(bool enter = true);

    static void SaveSignalToFlashDrive();

    static const Page * const self;
};



struct PageRAM
{
    static const Page * const self;
};



struct PageROM
{
    static const Page * const self;
};



struct PageDrive
{
    static const Page * const self;


    struct Manager
    {
        static const Page * const self;
    };


    struct Mask
    {
        static const Page * const self;
    };

    struct SetName
    {
        static const Page *const self;
    };
};
