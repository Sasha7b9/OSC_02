#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"


#define FILE_NAME_MASK          (set.mem.fileNameMask)
#define FILE_NAME               (set.mem.fileName)
#define INDEX_SYMBOL            (set.mem.indexCurSymbolNameMask)
#define FLASH_AUTOCONNECT       (set.mem.flashAutoConnect)

#define FILE_NAMING_MODE        (set.mem.fileNamingMode)
#define FILE_NAMING_MODE_MASK   (FILE_NAMING_MODE == FileNamingMode::Mask)
#define FILE_NAMING_MODE_MANUAL (FILE_NAMING_MODE == FileNamingMode::Manually)

#define MODE_WORK               (set.mem.modeWork)
#define MODE_WORK_IS_DIR        (MODE_WORK == ModeWork::Dir)
#define MODE_WORK_IS_RAM        (MODE_WORK == ModeWork::RAM)

#define SHOW_IN_INT             (set.mem.modeShowIntMem)
#define SHOW_IN_INT_SAVED       (SHOW_IN_INT == ModeShowIntMem::Saved)
#define SHOW_IN_INT_DIRECT      (SHOW_IN_INT == ModeShowIntMem::Direct)
#define SHOW_IN_INT_BOTH        (SHOW_IN_INT == ModeShowIntMem::Both)

/// ����������� ������������ � ���������� �����
#define ENUM_TO_REL_POINTS(enum) ((uint)(1 << ((enum) + 9)))

#define FPGA_ENUM_POINTS        (set.mem.enumPoints)
#define FPGA_POINTS_512         (FPGA_ENUM_POINTS == ENumPointsFPGA::_512)
#define FPGA_POINTS_8k          (FPGA_ENUM_POINTS == ENumPointsFPGA::_8k)
#define FPGA_NUM_POINTS         ENUM_TO_REL_POINTS(FPGA_ENUM_POINTS)

#define MODE_BTN_MEMORY         (set.mem.modeBtnMemory)
#define MODE_BTN_MEMORY_IS_MENU (MODE_BTN_MEMORY == ModeBtnMemory::Menu)
#define MODE_BTN_MEMORY_IS_SAVE (MODE_BTN_MEMORY == ModeBtnMemory::Save)

#define MODE_SAVE               (set.mem.modeSaveSignal)
#define MODE_SAVE_BMP           (MODE_SAVE == ModeSaveSignal::BMP)
#define MODE_SAVE_TXT           (MODE_SAVE == ModeSaveSignal::TXT)

#define MEM_DATA_SCALE          (set.mem.DataScale)
#define MEM_DATA_SCALE_RECALC   (MEM_DATA_SCALE == MemDataScale_Recalculated)


/// ����� ������.
struct ModeWork
{
    enum E
    {
        Dir,        ///< �������� �����.
        RAM,        ///< � ���� ������ ����� ����������� ��������� ���������� ���������.
        ROM,        ///< � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������.
        Number      ///< ������������ � ������ Data.c. �����, ����� �������, ��� ������ �� �������� �� �� ����� �����.
    } value;
    explicit ModeWork(E v) : value(v) {};
};

/// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     ///< ����� ������������ ��������������� �������� ����.
        Save      ///< ���������� ����������� ������ �� ������.
    } value;
};

/// ��� ��������� ������ �� ������.
struct ModeSaveSignal
{
    enum E
    {
        BMP,
        TXT
    } value;
};

/// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  ///< ���������� ������ ��������� �������.
        Saved,   ///< ���������� ���������� ������.
        Both     ///< ���������� ��� �������.
    } value;
};

/// ����� ������������ ������.
struct FileNamingMode
{
    enum E
    {
        Mask,        ///< ��������� �� �����.
        Manually     ///< ��������� �������.
    } value;
};


struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    Osci::Settings::Memory::ENumPointsFPGA::E   enumPoints;                             ///< ����� �����
    ModeBtnMemory::E                            modeBtnMemory;
    ModeWork::E                                 modeWork;                               ///< ����� ������.
    bool                                        flashAutoConnect;                       ///< ���� true, ��� ����������� ������ ������������� ��������� ����-��������.
    int8                                        indexCurSymbolNameMask;                 ///< ������ �������� ������� � ������ ������� ����� ��� ������ �����.
    ModeSaveSignal::E                           modeSaveSignal;                         ///< � ����� ���� ��������� ������.
    char                                        fileName[MAX_SYMBOLS_IN_FILE_NAME];     ///< ��� ����� ��� ������ ������� �������.
    ModeShowIntMem::E                           modeShowIntMem;                         ///< ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
    FileNamingMode::E                           fileNamingMode;                         ///< ����� ���������� ������.
    char                                        fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; ///< \brief ����� ����� ��� ��������������� ���������� ������.
                        ///< \details ������� ����������.\n
                        /// \code
                        /// %y('\x42') - ���, %m('\x43') - �����, %d('\x44') - ����, %H('\x45') - ����, %M('\x46') - ������, %S('\x47') - �������
                        /// %Nn('\x48''n') - ���������� �����, ������ �������� �� ����� n ���������, ��������, 7 � %3N ����� ������������� � 007
                        /// �������
                        /// name_%4N_%y_%m_%d_%H_%M_%S ����� ������������ ����� ���� name_0043_2014_04_25_14_45_32
                        /// ��� ���� �������� ��������, ��� ���� ������������ %4N ����� ����� ��������� ����������, ��, ������ �����, ���� �������� 
                        /// ����� ������ ����� 0001, �.�. ��� ����������� ������ ���������������.
                        /// \endcode
};

Osci::Settings::Memory::ENumPointsFPGA::E NumPoints_2_ENumPoints(int numPoints);
int ENumPoints_2_NumPoints(Osci::Settings::Memory::ENumPointsFPGA::E numPoints);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMemory
{
    static void OnChanged_Points(bool active);

    static void OnOpenClose_Drive_Manager(bool enter = true);

    static void SaveSignalToFlashDrive();

    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageRAM
{
    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageROM
{
    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageDrive
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageManager
    {
        static const Page * const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageMask
    {
        static const Page * const self;
    };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageSetName
{
    static const Page * const self;
};
