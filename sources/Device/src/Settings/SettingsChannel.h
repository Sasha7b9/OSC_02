#pragma once
#include "Utils/String.h"


 /// ��������.
struct Divider
{
    enum E
    {
        _1,
        _10
    } value;
    explicit Divider(E v) : value(v) { };
    explicit Divider(uint v) : value(static_cast<E>(v)) { };
    int ToAbs() const { return (value == _1) ? 1 : 10; };
};

struct DataSettings;

struct Chan
{
    enum E
    {
        A,
        B,
        Math,
        Size
    } value;
    explicit Chan(E v) : value(v) { };
    bool IsA() { return value == A; };
    bool IsB() { return value == B; }
    int PointsInChannel() const;
    /// ���������� ���������� ������, ��������� ��� ���������� ������ ������ ������
    int RequestBytes(DataSettings *ds) const;
    pString Name() const;
};

/// ����� ����������.
struct CalibrationMode
{
    enum E
    {
        x1,
        x10,
        Disable
    } value;
};


