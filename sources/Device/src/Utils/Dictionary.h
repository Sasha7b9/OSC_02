#pragma once
#include "Settings/Settings.h"


enum DictWord
{
    DHz,                // "��"
    DkHz,               // "���"
    DMHz,               // "���"
    Ds,                 // "�"
    Dns,                // "��"
    Dus,                // "���"
    Dms,                // "��"
    DBalanceChA,        // "������������ ������ 1"
    DBalanceChB,        // "������������ ������ 2"
    DCalibrateChA,      // "���������� ������ 1"
    DCalibrateChB,      // "���������� ������ 2"
    DTrigLev,           // "�� ����� = "
    DTrig,              // "��"
    DNumWords
};


#define DICT(word)      (dictWords[word])


extern pString dictWords[DNumWords];
