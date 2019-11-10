#pragma once


struct DataSettings;


struct PacketRAM
{
    /*
        ������ �������� ����� �������
    */
    uint addrNext;      /// ����� ���������� ������.
                        /// addrNext == 0x00000000 - � ������ ������ �� ��������
                        /// addrNext == 0xffffffff - � ������ �������� ������, �� ��� ��������� �����
    /// ��������� ������ �� ������ this
    void Pack(const DataSettings *ds);
    /// ��������� ������ ����� ������� ������. ���������� ��������� �� ���������� �����, ������� ���������� ����� �����
    PacketRAM *PackNewest(const DataSettings *ds);
    /// ���������� ��������� �� ��������� �����
    PacketRAM *Next() const;
    /// ���������� true, ���� ����� ������ (size == 0x0000)
    bool IsEmpty() const;
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
