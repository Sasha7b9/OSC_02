#pragma once


struct DataSettings;


struct PacketRAM
{
    /*
        ������ �������� ����� �������
    */
    uint16 size;        /// ������ ������ ������ � ������������ �������. ���� size == 0x0000, �� ���� ����� ��������
    /// ���������� ��������� �� ����� ������ �����. �� ����� ����������� ������
    static PacketRAM *Oldest();
    /// ���������� ��������� �� ����� ����� �����. �� ����� ����������� ���������
    static PacketRAM *Newest();
    /// ��������� ������ �� ������ this
    void Pack(const DataSettings *ds);
    /// ��������� ������ ����� ������� ������. ���������� ��������� �� ���������� �����, ������� ���������� ����� �����
    PacketRAM *PackNewest(const DataSettings *ds);
};


struct MemoryRAM
{
    /// ����� �������� � ����� �������. 0 - ��������� ��������� ������, 1 - ������������� ��������� ������.
    static int16 currentSignal;
    /// �������������� �������������� ��������� ������
    static void Init();
    /// �������� ����� ��������� ������
    static void Save(const DataSettings *ds);
    /// ������� ���������� � num � ����� ���������� ������. 0 - ��������� ������, 1 - �������������
    static bool Read(DataSettings **ds, uint num = 0);
    /// ���������� ���������� ������� � ������
    static uint NumberDatas();
};
