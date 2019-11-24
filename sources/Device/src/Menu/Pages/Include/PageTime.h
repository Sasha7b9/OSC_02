#pragma once
#include "Settings/SettingsOsci.h"


struct PeakDetMode
{
    enum E
    {
        Disabled,
        Enabled
    } value;

    PeakDetMode();

    operator PeakDetMode::E() const { return value; }

    bool IsEnabled() const;
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
    int InBytes();
};

/// ��� ������� ��� ������ �������������.
struct SampleType
{
    enum E
    {
        Real,    ///< �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
        Equal    ///< ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
    } value;
    explicit SampleType(E v) : value(v) {};
};

/// ������� ��/���.
struct FunctionTime
{
    enum E
    {
        Time,
        ShiftInMemory
    } value;
};

/// ��� �������� � �������� �� �����������
struct LinkingTShift
{
    enum E
    {
        Time,     ///< �������� ��������� � �������
        Position  ///< �������� ��������� � �������
    } value;
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
