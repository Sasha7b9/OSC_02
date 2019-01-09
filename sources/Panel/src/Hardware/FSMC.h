#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FSMC
{
public:
    /// ���� true, �� ��� ���������������� ������ �� ���� - ������ ������������ ���������� � ����������
    static void SetNowMode(bool nowMode);

    static bool InNowMode();

    static void Init();

    static uint8 ReadByte();
    /// �������� ����� �� ����. ���� ��������� ����� ����, ��������� ������ �� ���������� ��-�� ����, ��� ��� ������� ����
    static void WriteBuffer(const uint8 *data = 0, int length = 0);

private:
    /// ���� ��������� � ����� ������ ������
    static void ConfigToRead();
    /// ���� ��������� � ����� �������� ������
    static void ConfigToWrite();
    /// ���������� true, ���� ������ RD FPGA ��������� � ������� ���������
    static bool inReadState;
};
