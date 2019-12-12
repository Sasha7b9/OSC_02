#pragma once


struct DataSettings;
struct FrameP2P;
struct Packet;


class RAM
{
public:
    /// ����� �������� � ����� �������. 0 - ��������� ��������� ������, 1 - ������������� ��������� ������.
    static int16 currentSignal;
    /// �������������� �������������� ��������� ������
    static void Init();
    /// ����������� � ��������� ����� ��� �������� ����� ������
    static void PrepareForNewData(DataSettings *ds);
    /// ������� ���������� � num � ����� ���������� ������. 0 - ��������� ������, 1 - �������������
    static DataSettings *Get(uint num = 0);
    /// ���������� ���������� ������� � ������
    static uint NumberDatas();

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
