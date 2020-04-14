#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/SettingsNRST.h"
#include <cstring>


SettingsNRST setNRST;


static const SettingsNRST defaultNRST =
{
    0,                                                                 // size
    {0, 0},                                                            // balanceShiftADC
    0,                                                                 // numAveForRand
    0,                                                                 // numSmoothForRand
    0,                                                                 // correctionTime
    0,                                                                 // enum_gate_max
    0,                                                                 // enum_gate_min
    { BalanceADC::Disabled, {0,    0}    },                            // balanceADC
    { ExtraShift::Disabled,    {{0},  {0}}  },                         // shiftADC
    { {1.0F, 1.0F}, {ExtraStretch::Disabled, ExtraStretch::Disabled} } // stretchADCtype
};


void SettingsNRST::Init()
{
    SettingsNRST *saved = ROM::NRST::GetSaved();

    if(!saved ||                                    // ���� ��� ��������� ��������
       saved->size == static_cast<uint>(-1))        // ��� � ������ �� ����� ������ ��� �� ���� ������ ��������
    {
        setNRST = defaultNRST;                      // �� ��������� �������� �� ���������
    }
    else if(saved->size != sizeof(*this))           // ���� ������ ���������� �������� �� ����� ������� ����� ��������� (� �� �� ����� ���� ������)
    {
        setNRST = defaultNRST;                      // �� ��������� �������� �� ���������
        std::memcpy(&setNRST, saved, saved->size);  // �� ��������� �� ������ �� ��������, ��� ���
        size = sizeof(*this);
    }
    else
    {
        setNRST = *saved;                           // � ���� ����� ������� ��������� ��������� � ����� ���������
    }
}


void SettingsNRST::Save()
{
    SettingsNRST *saved = ROM::NRST::GetSaved();

    size = sizeof(*this);

    if(!saved ||                        // ���� ��� ���������� ��������
       saved->size != size ||           // ��� ������� �������� �� ���������
       *saved != setNRST)               // ��� ��������� ���������� � ������� �� ���������
    {
        ROM::NRST::Save(&setNRST);      // �� ��������� ���������
    }
}


void SettingsNRST::Reset()
{
    setNRST = defaultNRST;
}


bool SettingsNRST::operator!=(const SettingsNRST &rhs)
{
    return std::memcmp(this, &rhs, sizeof(*this)) != 0;
}
