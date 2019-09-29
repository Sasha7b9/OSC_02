#pragma once
#include "Settings/SettingsOsci.h"


/// \brief ��� ������������ ��� �������.
/// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
struct BalanceADC
{
    enum E
    {
        Disable,     ///< ������������ ���������.
        Settings,    ///< ������������ �������� ������������, ������� �������� �������������.
        Hand         ///< ������������ �������� ������������, �������� �������.
    } value;
    explicit BalanceADC(E v) : value(v) {};
};

/// ��� �������� ���
struct StretchADC
{
    enum E
    {
        Disable,
        Real,
        Hand
    } value;
    explicit StretchADC(E v) : value(v) {};
};


#define NRST_RSHIFT_ADD(ch, range, mode)    (set.nrst.rShiftAdd[ch][range][mode])
#define NRST_RSHIFT_ADD_A(range, mode)      (NRST_RSHIFT_ADD(Chan::A, range, mode))
#define NRST_RSHIFT_ADD_B(range, mode)      (NRST_RSHIFT_ADD(Chan::B, range, mode))

#define RSHIFT_ADD_STABLE(ch, range)        (set.nrst.rShiftAddStable[ch][range])
#define RSHIFT_ADD_STABLE_A(range)          (RSHIFT_ADD_STABLE(Chan::A, range))
#define RSHIFT_ADD_STABLE_B(range)          (RSHIFT_ADD_STABLE(Chan::B, range))

#define NRST_STRETCH_ADC_TYPE               (set.nrst.stretchADCtype)
#define NRST_STRETCH_ADC_TYPE_IS_DISABLE    (NRST_STRETCH_ADC_TYPE == StretchADC::Disable)
#define NRST_STRETCH_ADC_TYPE_IS_HAND       (NRST_STRETCH_ADC_TYPE == StretchADC::Hand)

#define NRST_STRETCH_ADC(ch, type)          (set.nrst.stretchADC[ch][type])
#define NRST_STRETCH_ADC_A(type)            (NRST_STRETCH_ADC(Chan::A, type))
#define NRST_STRETCH_ADC_B(type)            (NRST_STRETCH_ADC(Chan::B, type))


struct SettingsNRST
{
    int16           balanceADC[Chan::Size];                 ///< �������� ��������������� �������� ��� ��� ������ ������������.
    BalanceADC::E   balanceADCtype;                         ///< ��� ������������.
    StretchADC::E   stretchADCtype;                         ///< ��� �������� ������.
    int16           stretchADC[Chan::Size][3];              ///< \brief ����������� ����������� ��� �������, �������������� �
                    ///< ������������ �������. ����� �������� � ����� ����, ����� �������� �������� �����������, ����� ��������� �� 1000 �
                    ///< ��������� �������.
    int16           rShiftAddStable[Chan::Size][3];         ///< ���������� �������� ��� ��� ����� �������������� ����������. ������� �������� ��� ���������
    int16           numAveForRand;                          ///< �� �������� ���������� ��������� ������ � ������ �������������.
    int16           numSmoothForRand;                       ///< ����� ����� ��� ����������� ������ � �������������.
    int16           rShiftAdd[Chan::Size][Range::Size][2];  ///< ���������� ��������, ������� ������� ���� ��� ���������� � ������������
    int16           correctionTime;                         ///< ����������� ��������� �������.
};
