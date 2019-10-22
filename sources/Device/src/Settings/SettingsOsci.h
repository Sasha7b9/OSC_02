#pragma once


#define SET_RSHIFT(ch)  (RShift::Value(ch))
#define SET_RSHIFT_A    (SET_RSHIFT(Chan::A))
#define SET_RSHIFT_B    (SET_RSHIFT(Chan::B))

/// �� ������� ������ ����� �������� �������� ��������, ����� ������ �������� �� ���������� ������������ �� ���� �����.
#define STEP_RSHIFT     (((RShift::MAX - RShift::MIN) / 24) / 20)
#define STEP_TRIGLEV    STEP_RSHIFT



struct DataSettings;


struct Chan
{
    enum E
    {
        A,
        B,
        Count
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
        Count
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


struct Range
{
    static void Change(Chan::E ch, int delta);
    /// ����������� ��� ��������� ����� ������ ��� ����� ������� ������������
    static void LoadBoth();

    enum E
    {
        _2mV,
        _5mV,
        _10mV,
        _20mV,
        _50mV,
        _100mV,
        _200mV,
        _500mV,
        _1V,
        _2V,
        _5V,
        _10V,
        _20V,
        Count
    } value;
    explicit Range(E v) : value(v) {};
    pString Name() const;

    static void Set(Chan::E ch, E range);

    pString ToString(int8 divider);

};


struct RShift
{
    /// ��� �������� ������������� �������� ��������
    static const int MIN = 20;
    /// ��� �������� ������������ ��������� ��������
    static const int MAX = 980;
    /// ��� �������� ������������� �������� ������
    static const int ZERO = 500;
    /// �������� �� delta
    static void Change(Chan::E ch, int delta);
    /// ���������� ��������
    static void Set(Chan::E ch, uint16 rShift);
    /// ��������� � ����������
    static void Load(Chan::E ch);
    /// ���������� ��� �� ������
    static void DrawBoth();
    /// ������������� � ������
    static String ToString(uint16 rShiftRel, Range::E range, int8 divider);
    /// ���������� ������ �� ��������
    static uint16 &Value(Chan::E ch);
private:
    /// ���������� ������ ������������� �������� �� �����
    static void Draw(Chan::E ch);
};

/// ����� ������ �� �����.
struct ModeCouple
{
    enum E
    {
        DC,      ///< �������� ����
        AC,      ///< �������� ����
        GND,     ///< ���� �������.
        Count
    } value;
    explicit ModeCouple(E v) : value(v) {};
    pString UGO() const;

    static void Set(Chan::E ch, ModeCouple::E couple);
};

struct Bandwidth
{
    enum E
    {
        Full,     ///< ���� ��� �������� ������� � ���� �����, �� ��� ���� ��������� ��������������� ������ �� �������-������-������.
        _20MHz,
    } value;

    explicit Bandwidth(E v) : value(v) {};
    void Load();

private:
    /// ���������� �����, ��� �������� �������� ����������� ��� ���������
    Chan::E GetChannel() const;
};

struct TShift
{
    static int Min();
    static int Zero();
    static int Max();

    static void Set(int tShift);

    static void Load();

    static void Change(int delta);

    static String ToString(TBase::E tBase);

private:
    static void LoadReal();
    static void LoadRandomize();
};

struct Trig
{
    /// ���������� true � ������ ������� ��������������
    static bool SyncPulse();
    /// ������� ������� ������������� ������ �����
    static void DrawOnGrid();
    /// ����� ������ ���� ������� ������� ������������� ����� �������������� ������ ������� � ������� timMS �����������
    static void NeedForDraw();

    static bool pulse;
};

struct TrigLevel
{
    static const int MIN = 20;
    static const int MAX = 980;
    static const int ZERO = 500;
    /// ��������� ������� ������������� � ���������� �����
    static void Load();
    /// �������� ������� ������������� �� delta ������
    static void Change(int delta);
    /// ���������� �������� ������� �������������
    static void Set(int level);
    /// ���������� ����� ������� �������������
    static void Find();
};
