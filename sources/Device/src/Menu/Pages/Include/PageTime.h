#pragma once
#include "Osci/ParametersOsci.h"


#define S_PEAK_DET          (set.time._peakDet)
#define S_PEAK_DET_ENABLED  (S_PEAK_DET == PeakDetMode::Enabled)
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
#define S_TPOS              (set.time._tPos)
#define S_TPOS_IS_LEFT      (S_TPOS == TPos::Left)
#define S_TPOS_IS_CENTER    (S_TPOS == TPos::Center)
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
    PeakDetMode::E      _peakDet;
    TPos::E             _tPos;
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
