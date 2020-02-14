#pragma once
#include "Hardware/Memory/ExtRAM.h"


//#define VALIDATE_RAM_ ::RAM::VerifyOnValid(__FILE__, __LINE__);
//#define VALIDATE_RAM

//#define TEST_RAM VALIDATE_RAM_; TEST_EXT_RAM_
//#define TEST_RAM


struct DataSettings;
struct FrameP2P;
struct Packet;


class RAM
{
public:

    static bool canTrace;

    /// ����� �������� � ����� �������. 0 - ��������� ��������� ������, 1 - ������������� ��������� ������.
    static int16 currentSignal;
    /// �������������� �������������� ��������� ������
    static void Init();
    /// ����������� � ��������� ����� ��� �������� ����� ������
    static DataSettings *PrepareForNewData();
    /// ������� ���������� � num � ����� ���������� ������. 0 - ��������� ������, 1 - �������������
    static DataSettings *Get(uint num = 0);
    /// ���������� ���������� ������� � ������
    static uint NumberDatas();
    /// ���������� true, ���� ��������� ���������� ����������� ������� ������������� �������
    static bool LastSettingsEqualsCurrent();

    static void VerifyOnValid(char *file, int line);

private:
    /// ��������� �� ����� ������ ���������� �����. �� ����� ���� ������
    static Packet *oldest;
    /// ��������� �� ��������� ���������� �����. �� ����� ���� ���������
    static Packet *newest;
    /// ���������� ����� ��� ������ ������ � ������� � ������������ � ds
    static uint AllocateMemoryForPacket(const DataSettings *ds);
    /// ������� ����� ������ ������
    static void RemoveOldest();
    /// ���������� size ���� ������ � ������ ������
    static void AllocateMemoryFromBegin(uint size);
};
