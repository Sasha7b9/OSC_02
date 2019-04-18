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
    DDetectFlashDrive,  // "���������� ������������ ����������"
    DMode,              // "�����"
    D1ch,               // "1�"
    D2ch,               // "2�"
    DSaveFirmware,      // "�������� ��������"
    DStoredInMemory,    // "��������� � ������"
    DNumWords
};


#define DICT(word)      (dictWords[word])


extern pString dictWords[DNumWords];
