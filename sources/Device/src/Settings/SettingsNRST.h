#pragma once
#include "Osci/ParametersOsci.h"


/// \brief ��� ������������ ��� �������.
/// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
struct BalanceADC
{
    enum E
    {
        Disabled,   ///< ������������ ���������.
        Settings,   ///< ������������ �������� ������������, ������� �������� �������������.
        Hand        ///< ������������ �������� ������������, �������� �������.
    } value;
    static BalanceADC &Ref();
    static bool IsHand()            { return Ref().value == Hand; }
    static int16 &Value(Chan::E ch) { return Ref().balance[ch];   }
    int16  balance[Chan::Count];                    ///< �������� ��������������� �������� ��� ��� ������ ������������.
};

// ��� �������� ���
struct StretchADC
{
    enum Type
    {
        Disabled,
        Real,
        Hand,
        Count
    } type;
    static void SetTypeDisabled();
    static void SetTypeReal();
    static bool TypeIsDisabled();
    static bool TypeIsHand();
    static float GetValue(Chan::E ch);
    static void SetValue(Chan::E ch, float value);
    float  value[Chan::Count];                                             // �������� � ����� ����, ����� �������� �������� �����������, ����� ��������� �� 1000 � ��������� �������.
};


// �������������� �������� ���
struct ExtraShift
{
    enum E
    {
        Disabled,   // �������������� �������� �� ����������
        Real,       // ����������� �������������� ��������, ������������ ���� ����������
        Count
    } type;
    static void SetTypeDisabled();
    static void SetTypeReal();
    static void SetValue(Chan::E ch, Range::E range, int8 value);
    static int8 GetValue(Chan::E ch, Range::E range);
    int8  value[Chan::Count][Range::Count];        // ���������� ��������, ������� ������� ���� ��� ���������� � ������������
};


// ����� �������� ���������, ������� ������������ ��� ���������� � �� ���������������� ��� ���������� �������

struct SettingsNRST
{
    /*
        �������� !!! ��� ���������� ������ ������ ��������� ������ ���� ���������
    */

    uint           size;                           // ������ ���������. �� ���� ��� �������� �� ������ ����� ����������, ��� ��������� ����� - ����� ����� �������� 0xFFFFFFFF
    int8           balanceShiftADC[Chan::Count];   // ���������� �������� ��� ������������ ���
    int16          numAveForRand;                  // �� �������� ���������� ��������� ������ � ������ �������������.
    int16          numSmoothForRand;               // ����� ����� ��� ����������� ������ � �������������.
    int16          correctionTime;                 // ����������� ��������� �������.
    int16          enum_gate_max;                  // ����������� ����� � ������������� ������
    int16          enum_gate_min;                  // ����������� ����� � ������������� �����
    BalanceADC     balanceADC;                     // ��� ������������.
    ExtraShift     exShift;                        // �������������� ��������
    StretchADC     stretchADC;                     // ��� �������� ������.
    // ������������� ��� ���������. ��������� ���������� ������� �������� � ROM � �� �������� � ������, ���� ��������� ����. ���� �������� ���� - ������������� ���������� �� ���������
    void Init();
    // ���������� �������� � ROM. ���������� ������ ���� ��������� � setNRST �� ��������� � ����������� � ROM
    void Save();

    bool operator!=(const SettingsNRST &rhs);
};


extern SettingsNRST setNRST;
