#pragma once
#include "Settings/SettingsOsci.h"


struct PeakDetMode
{
    enum E
    {
        Disabled,
        Enabled
    };
    PeakDetMode() {}
    static PeakDetMode::E &Ref();
    operator PeakDetMode::E() { return Ref(); }
    static bool IsEnabled();
};

/// ��������� ����� ������������� �� �������.
struct TPos
{
    enum E
    {
        Left,          ///< �������� � ������ ����.
        Center,        ///< �������� � ������.
        Right          ///< �������� � ������� ����.
    };

    TPos() {};

    operator TPos::E();
    /// ������ �������� ���������� ������ ������������� � ������
    int InBytes() const;

    void Draw() const;

    int PosX() const;
};

/// ��� ������� ��� ������ �������������.
struct SampleType
{
    enum E
    {
        Real,    ///< �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
        Equal    ///< ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
    };

    SampleType() {};

    void Set(SampleType::E type);

    operator SampleType::E();
};

/// ������� ��/���.
struct FunctionTime
{
    enum E
    {
        Time,
        ShiftInMemory
    };
};

/// ��� �������� � �������� �� �����������
struct LinkingTShift
{
    enum E
    {
        Time,     ///< �������� ��������� � �������
        Position  ///< �������� ��������� � �������
    };
};


 struct SettingsTime
{
    int                 shift;
    TBase::E            base;
    PeakDetMode::E      peakDet;
    TPos::E             tPos;
    SampleType::E       sampleType;
    FunctionTime::E     timeDivXPos;
    LinkingTShift::E    linkingTShift;  ///< ��� �������� �������� �� �����������
    SampleType::E       sampleTypeOld;
};



struct PageTime
{
    static void OnChanged_PeakDet(bool active);

    static void OnChanged_TPos(bool active);

    static const Page * const self;
};
