#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageTime.h"


/// ����������� ������������ � ���������� �����
#define ENUM_TO_REL_POINTS(enum)    ((uint)(1 << (static_cast<int>(enum) + 9)))


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

    ModeWork() {}
    static ModeWork::E &Ref();
    operator ModeWork::E() { return Ref(); }
    static bool IsRAM()  { return Ref() == RAM; }
    static bool IsDir()  { return Ref() == Dir; }
    static bool IsROM()  { return Ref() == ROM; }
    static void SetRAM() { Ref() = RAM; }
    static void SetDir() { Ref() = Dir; }
    static void Set(E v) { Ref() = v;   }
};

/// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     ///< ����� ������������ ��������������� �������� ����.
        Save      ///< ���������� ����������� ������ �� ������.
    };

    static ModeBtnMemory::E &Ref();
};

/// ��� ��������� ������ �� ������.
struct ModeSaveSignal
{
    enum E
    {
        BMP,
        TXT
    };
    static ModeSaveSignal::E &Ref();
    static bool IsBMP() { return Ref() == BMP; }
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

    static FileNamingMode::E &Ref();
    static bool IsManually() { return Ref() == Manually; }
    static bool IsMask()     { return Ref() == Mask; }
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
    };
    ENumPointsFPGA() {};
    static ENumPointsFPGA::E &Ref();
    operator ENumPointsFPGA::E() { return Ref(); }
    static uint BytesInChannel() { uint result = PointsInChannel(); if (PeakDetMode().IsEnabled()) { result *= 2; } return result; };
    static uint PointsInChannel() { return ENUM_TO_REL_POINTS(Ref()); };
    static ENumPointsFPGA::E FromNumPoints(int numPoints);
    static const uint MAX_NUM = 8192;
};

int ENumPoints_2_NumPoints(ENumPointsFPGA::E numPoints);

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

    static TypeSignalROM::E &Ref();
    static bool IsRecorded() { return Ref() == Recorded; }
    static bool IsCurrent()  { return Ref() == Current; }
    static bool IsBoth()     { return Ref() == Both; }
};

#define MAX_SYMBOLS_IN_FILE_NAME 35

struct SettingsMemory
{
    ENumPointsFPGA::E   enumPoints;                             ///< ����� �����
    ModeBtnMemory::E    modeBtnMemory;
    TypeSignalROM::E    typeSignalROM;                          ///< ����� ������ �������� � ������ ������-����� ��
    ModeWork::E         modeWork;                               ///< ����� ������.
    bool                flashAutoConnect;                       ///< ���� true, ��� ����������� ������ ������������� ��������� ����-��������.
    int8                indexCurSymbolNameMask;                 ///< ������ �������� ������� � ������ ������� ����� ��� ������ �����.
    ModeSaveSignal::E   modeSaveSignal;                         ///< � ����� ���� ��������� ������.
    char                fileName[MAX_SYMBOLS_IN_FILE_NAME];     ///< ��� ����� ��� ������ ������� �������.
    ModeShowIntMem::E   _;                                      ///< ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
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
