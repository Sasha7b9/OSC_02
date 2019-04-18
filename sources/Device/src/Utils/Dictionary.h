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
    DNumWords
};


#define DICT(word)      (dictWords[word])


extern pString dictWords[DNumWords];
