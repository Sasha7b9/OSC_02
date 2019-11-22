#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageTime.h"


/// ����������� ������������ � ���������� �����
#define ENUM_TO_REL_POINTS(enum)    ((uint)(1 << (static_cast<int>(enum) + 9)))
#define FPGA_NUM_POINTS             ENUM_TO_REL_POINTS(set.mem.enumPoints)


/// ����� ������.
struct ModeWork
{
    enum E
    {
        Dir,        ///< �������� �����.
        RAM,        ///< � ���� ������ ����� ����������� ��������� ���������� ���������.
        ROM,        ///< � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������.
        Count       ///< ������������ � ������ Data.c. �����, ����� �������, ��� ������ �� �������� �� �� ����� �����.
    };
};

/// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     ///< ����� ������������ ��������������� �������� ����.
        Save      ///< ���������� ����������� ������ �� ������.
    };
};

/// ��� ��������� ������ �� ������.
struct ModeSaveSignal
{
    enum E
    {
        BMP,
        TXT
    };
};

/// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  ///< ���������� ������ ��������� �������.
        Saved,   ///< ���������� ���������� ������.
        Both     ///< ���������� ��� �������.
    };
};

/// ����� ������������ ������.
struct FileNamingMode
{
    enum E
    {
        Mask,        ///< ��������� �� �����.
        Manually     ///< ��������� �������.
    };
};

/// ����� ����� �������, � ������� ��� ������.
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
    } value;
    ENumPointsFPGA();
    uint BytesInChannel(PeakDetMode::E mode) const;
    uint PointsInChannel() const;
    static const uint MAX_NUM = 8192;
};

/// ����� ������ ���������� � ������ ������ - ����� ��
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


struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    ENumPointsFPGA::E   enumPoints;                             ///< ����� �����
    ModeBtnMemory::E    modeBtnMemory;
    TypeSignalROM::E    typeSignalROM;                          ///< ����� ������ �������� � ������ ������-����� ��
    ModeWork::E         modeWork;                               ///< ����� ������.
    bool                flashAutoConnect;                       ///< ���� true, ��� ����������� ������ ������������� ��������� ����-��������.
    int8                indexCurSymbolNameMask;                 ///< ������ �������� ������� � ������ ������� ����� ��� ������ �����.
    ModeSaveSignal::E   modeSaveSignal;                         ///< � ����� ���� ��������� ������.
    char                fileName[MAX_SYMBOLS_IN_FILE_NAME];     ///< ��� ����� ��� ������ ������� �������.
    ModeShowIntMem::E   modeShowIntMem;                         ///< ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
    FileNamingMode::E   fileNamingMode;                         ///< ����� ���������� ������.
    char                fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; ///< \brief ����� ����� ��� ��������������� ���������� ������.
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

ENumPointsFPGA::E NumPoints_2_ENumPoints(int numPoints);
int ENumPoints_2_NumPoints(ENumPointsFPGA::E numPoints);



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


    struct PageManager
    {
        static const Page * const self;
    };


    struct PageMask
    {
        static const Page * const self;
    };
};


struct PageSetName
{
    static const Page * const self;
};
