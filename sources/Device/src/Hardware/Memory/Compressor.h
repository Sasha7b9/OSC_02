#pragma once
#include "Hardware/Memory/Memory.h"


struct Compressor
{
    /// ���������� ���������� ����, ����������� ��� �������� ������
    static uint GetPackedSize(const DataSettings *ds);
    /// ����������� ������ � ���������� ds �� ������ address
    static void Pack(const DataSettings *ds, Address address);
    /// ���������� ��������� ds �� ������������ ������ �� ������ address
    static bool UnPack(Address address, DataSettings **ds);
};
