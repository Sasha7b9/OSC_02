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

/// ��� �������� ���
struct StretchADC
{
    enum Type
    {
        Disabled,
        Real,
        Hand,
        Count
    } value;
    static StretchADC &Ref();
    static StretchADC::Type GetType() { return Ref().value;               }
    static void SetTypeDisabled()     { Ref().value = Disabled;           }
    static void SetTypeReal()         { Ref().value = Real;               }
    static bool TypeIsDisabled()      { return (Ref().value == Disabled); }
    static bool TypeIsReal()          { return (Ref().value == Real);     }
    static bool TypeIsHand()          { return (Ref().value == Hand);     }
    static float Value(Chan::E ch)    { return Ref().stretch[ch];         }
    float  stretch[Chan::Count];                                             // �������� � ����� ����, ����� �������� �������� �����������, ����� ��������� �� 1000 � ��������� �������.
};


/// �������������� �������� ���
struct ShiftADC
{
    enum E
    {
        Disable,    ///< �������������� �������� �� ����������
        Real,       ///< ����������� �������������� ��������, ������������ ���� ����������
        Count
    } value;
    static ShiftADC &Ref();
    static void SetDisabled()                               { Ref().value = Disable;                        };
    static void SetReal()                                   { Ref().value = Real;                           };
    static bool IsReal()                                    { return Ref().value == Real;                   };
    static void Set(Chan::E ch, Range::E range, int8 value) { Ref().shift[ch][range] = value;               }
    static int8 Value(Chan::E ch, Range::E range)           { return IsReal() ? Ref().shift[ch][range] : 0; };
    int8  shift[Chan::Count][Range::Count];        ///< ���������� ��������, ������� ������� ���� ��� ���������� � ������������
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
    ShiftADC       shiftADC;                       // ��� ������������ ��� ��������� ��������������� ��������
    StretchADC     stretchADC;                     // ��� �������� ������.
    // ������������� ��� ���������. ��������� ���������� ������� �������� � ROM � �� �������� � ������, ���� ��������� ����. ���� �������� ���� - ������������� ���������� �� ���������
    void Init();
    // ���������� �������� � ROM. ���������� ������ ���� ��������� � setNRST �� ��������� � ����������� � ROM
    void Save();

    bool operator!=(const SettingsNRST &rhs);
};


extern SettingsNRST setNRST;
