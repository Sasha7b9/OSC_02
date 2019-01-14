#pragma once
#include "Data/DataSettings.h"


/// ����� �������� ������ 
class DataBuffer
{
public:
    static void Init();

    static uint Size();
    /// ���������� ��������� �� ������, ���������� � ����� ����������
    static DataSettings *Top();

private:
    /// �������� ������, ��������� � DataSettings, ���������� � �����. ���� ����� �� �������, �� ����� ������ ���������� ������ ���������
    static void Push(DataSettings *ds);
    /// ��������������� ����� ��� �������� ������ � �������� DataSettings
    class Stack
    {
    public:

        static uint Size();
        /// � ����������� ��������� ����� ������� �������� � ���������� ���������
        static void Clear();

        static void Push(DataSettings *ds);

        static DataSettings *Top();
    };
};
