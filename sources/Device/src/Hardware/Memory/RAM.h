#pragma once


struct FrameP2P;
struct DataSettings;


struct RAM
{
    /// ����� �������� � ����� �������. 0 - ��������� ��������� ������, 1 - ������������� ��������� ������.
    static int16 currentSignal;
    /// �������������� �������������� ��������� ������
    static void Init();
    /// ����������� � ��������� ����� ��� �������� ����� ������ � ������������ � ����������� �� ds
    static void PrepareForNewData(DataSettings *ds);
    /// �������� ����� ��������� ������
    static void Save(const DataSettings *ds);
    /// ������� ���������� � num � ����� ���������� ������. 0 - ��������� ������, 1 - �������������
    static DataSettings *Read(uint num = 0);
    /// ���������� ���������� ������� � ������
    static uint NumberDatas();

    static void PrepareNewFrameP2P();

    static FrameP2P *GetFrameP2P();
};
