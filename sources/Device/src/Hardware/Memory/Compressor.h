#pragma once
#include "Hardware/Memory/Memory.h"


struct Compressor
{
    /// ���������� ���������� ����, ����������� ��� �������� ������
    static uint GetPackedSize(const DataSettings *ds);
    /// ����������� ������ � ���������� ds �� ������ address
    static void PackToROM(const DataSettings *ds, Address address);
    /// ���������� ��������� ds �� ������������ ������ �� ������ address
    static bool UnPack(Address address, DataSettings **ds);
    /// ���������� ����� ���������� ������. ���� 
    static Address Next(Address address);
};
