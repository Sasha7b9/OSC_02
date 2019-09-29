#pragma once

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

struct TBase
{
    enum E
    {
        _2ns,
        _5ns,
        _10ns,
        _20ns,
        _50ns,
        _100ns,
        _200ns,
        _500ns,
        _1us,
        _2us,
        _5us,
        _10us,
        _20us,
        _50us,
        _100us,
        _200us,
        _500us,
        _1ms,
        _2ms,
        _5ms,
        _10ms,
        _20ms,
        _50ms,
        _100ms,
        _200ms,
        _500ms,
        _1s,
        _2s,
        _5,
        _10s,
        Size
    } value;
    explicit TBase(E v) : value(v) {};

    pString Name() const;

    pString ToString() const;

    static void Load();

    static void Change(int delta);

    /// � ����� �������� ������ ���������� ����� ����������� ������
    static const E MIN_P2P = _50ms;
    /// ����������� ������� �� �������, ��� ������� ��� �������� ��������� ������ �������� ���������
    static const E MIN_PEAK_DET = _200ns;
};
