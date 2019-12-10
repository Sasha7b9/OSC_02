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
    /// ����������� � ��������� ����� ��� �������� ����� ������ � ������������ � ����������� �� ds
    static void PrepareForNewData(DataSettings *ds);
    /// ������� ���������� � num � ����� ���������� ������. 0 - ��������� ������, 1 - �������������
    static DataSettings *Read(uint num = 0);
    /// ���������� ���������� ������� � ������
    static uint NumberDatas();

    static void PrepareNewFrameP2P();

    static FrameP2P *GetFrameP2P();

private:
    /// ��������� �� ����� ������ ���������� �����. �� ����� ���� ������
    static Packet *oldest;
    /// ��������� �� ��������� ���������� �����. �� ����� ���� ���������
    static Packet *newest;
    /// ����� ����������� ����� ���� ������ ���� � ������ � ����� ��������� ������ �� ��������� ������
    static FrameP2P frameP2P;
    /// ���������� ����� ��� ������ ������ � ������� � ������������ � ds
    static uint AllocateMemoryForPacket(const DataSettings *ds);
    /// ������� ����� ������ ������
    static void RemoveOldest();
    /// ���������� size ���� ������ � ������ ������
    static void AllocateMemoryFromBegin(uint size);
};
