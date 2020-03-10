#pragma once
#include "Osci/ParametersOsci.h"


/// \brief ��� ������������ ��� �������.
/// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
#pragma pack(push, 1)
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
    enum E
    {
        Disabled,
        Real,
        Hand,
        Count
    } value;
    static StretchADC &Ref();
    static StretchADC::E Type()    { return Ref().value;               }
    static void SetDisabled()      { Ref().value = Disabled;           }
    static void SetReal()          { Ref().value = Real;               }
    static bool IsDisabled()       { return (Ref().value == Disabled); }
    static bool IsReal()           { return (Ref().value == Real);     }
    static bool IsHand()           { return (Ref().value == Hand);     }
    static float Value(Chan::E ch) { return Ref().stretch[ch];         }
    float  stretch[Chan::Count];            ///< �������� � ����� ����, ����� �������� �������� �����������, ����� ��������� �� 1000 � ��������� �������.
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

    uint        size;                           ///< ������ ���������. �� ���� ��� �������� �� ������ ����� ����������, ��� ��������� ����� - ����� ����� �������� 0xFFFFFFFF
    int8        balanceShiftADC[Chan::Count];   ///< ���������� �������� ��� ������������ ���
    int16       numAveForRand;                  ///< �� �������� ���������� ��������� ������ � ������ �������������.
    int16       numSmoothForRand;               ///< ����� ����� ��� ����������� ������ � �������������.
    int16       correctionTime;                 ///< ����������� ��������� �������.
    int16       enum_gate_max;                  ///< ����������� ����� � ������������� ������
    int16       enum_gate_min;                  ///< ����������� ����� � ������������� �����
    BalanceADC  balanceADC;                     ///< ��� ������������.
    ShiftADC    shiftADC;                       ///< ��� ������������ ��� ��������� ��������������� ��������
    StretchADC  stretchADC;                     ///< ��� �������� ������.
    // ������������� ��� ���������. ��������� ���������� ������� �������� � ROM � �� �������� � ������, ���� ��������� ����. ���� �������� ���� - ������������� ���������� �� ���������
    void Init();
    // ���������� �������� � ROM. ���������� ������ ���� ��������� � setNRST �� ��������� � ����������� � ROM
    void Save();

    bool operator!=(const SettingsNRST &rhs);
};

#pragma pack(pop)


extern SettingsNRST setNRST;
