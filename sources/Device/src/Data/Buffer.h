#pragma once
#include "Data/DataSettings.h"


/// ����� �������� ������ 
class Buffer
{
    friend class Storage;

public:
    static void Init();

    static void Update();

private:
    /// �������� ������, ��������� � DataSettings, ���������� � �����. ���� ����� �� �������, �� ����� ������ ���������� ������ ���������
    static void Push(DataSettings *ds);
    /// ���������� ��������� �� ������, ���������� � ����� ����������
    static DataSettings *Top();

    /// ��������������� ����� ��� �������� ������ � �������� DataSettings
    class Stack
    {
    public:
        /// � ����������� ��������� ����� ������� �������� � ���������� ���������
        static void Clear();

        static void Push(DataSettings *ds);
    };
};
