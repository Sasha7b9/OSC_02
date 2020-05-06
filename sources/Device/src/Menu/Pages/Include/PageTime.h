#pragma once
#include "Osci/ParametersOsci.h"


struct PeakDetMode
{
    enum E
    {
        Disabled,
        Enabled
    };

    static bool IsEnabled();
};

// ��������� ����� ������������� �� �������.
struct TPos
{
    enum E
    {
        Left,          // �������� � ������ ����.
        Center,        // �������� � ������.
        Right          // �������� � ������� ����.
    };

    // ������ �������� ���������� ������ ������������� � ������
    static int InBytes();

    static void Draw();

    static int PosX();
};

// ��� ������� ��� ������ �������������.
struct SampleType
{
    enum E
    {
        Real,    // �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
        Equal    // ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
    };
};

// ������� ��/���.
struct FunctionTime
{
    enum E
    {
        Time,
        ShiftInMemory
    };
};

// ��� �������� � �������� �� �����������
struct LinkingTShift
{
    enum E
    {
        Time,     // �������� ��������� � �������
        Position  // �������� ��������� � �������
    };
};


#define S_TIME_SHIFT    (set.time._shift)

 struct SettingsTime
{
    int                 _shift;
    TBase::E            _base;
    PeakDetMode::E      peakDet;
    TPos::E             tPos;
    SampleType::E       sampleType;
    FunctionTime::E     _;
    LinkingTShift::E    linkingTShift;  // ��� �������� �������� �� �����������
    SampleType::E       sampleTypeOld;
};



struct PageTime
{
    static void OnChanged_PeakDet(bool active);

    static void OnChanged_TPos(bool active);

    static const Page * const self;
};
