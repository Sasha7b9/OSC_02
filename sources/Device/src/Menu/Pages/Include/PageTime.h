#pragma once
#include "Settings/ParametersOsci.h"


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

    TPos() {}

    static TPos::E &Ref();

    operator TPos::E() { return Ref(); }
    /// ������ �������� ���������� ������ ������������� � ������
    static int InBytes();

    static void Draw();

    static int PosX();
};

/// ��� ������� ��� ������ �������������.
struct SampleType
{
    enum E
    {
        Real,    ///< �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
        Equal    ///< ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
    };

    static SampleType::E &Ref();

    operator SampleType::E() { return Ref(); }

    static void Set(SampleType::E type) { Ref() = type; }
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

    static LinkingTShift::E &Ref();
};


 struct SettingsTime
{
    int                 shift;
    TBase::E            base;
    PeakDetMode::E      peakDet;
    TPos::E             tPos;
    SampleType::E       sampleType;
    FunctionTime::E     _;
    LinkingTShift::E    linkingTShift;  ///< ��� �������� �������� �� �����������
    SampleType::E       sampleTypeOld;
};



struct PageTime
{
    static void OnChanged_PeakDet(bool active);

    static void OnChanged_TPos(bool active);

    static const Page * const self;
};
