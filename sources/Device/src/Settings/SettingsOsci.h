#pragma once


struct DataSettings;

struct Chan
{
    enum E
    {
        A,
        B,
        Count
    } value;
    Chan(E v) : value(v) { };
    bool IsA() { return value == A; };
    bool IsB() { return value == B; }
    int PointsInChannel() const;
    /// ���������� ���������� ������, ��������� ��� ���������� ������ ������ ������
    int RequestBytes(DataSettings *ds) const;
    pString Name() const;
    operator int() { return static_cast<int>(value); };
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
    };
    /// ���� ����������� ����� ��� ������ ��������
    TBase() {};
    /// ���� ������������ ����� ���������� ����� ��������
    explicit TBase(E v);

    operator TBase::E();

    pString Name() const;

    pString ToString() const;

    void Load();

    void Change(int delta);

    /// � ����� �������� ������ ���������� ����� ����������� ������
    static const E MIN_P2P = _50ms;
    /// ����������� ������� �� �������, ��� ������� ��� �������� ��������� ������ �������� ���������
    static const E MIN_PEAK_DET = _200ns;

    uint RandK();
};


struct Range
{
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
    };
    /// ���� ����������� - ����� ������ ������� ��������� Range
    Range(Chan::E _ch) : ch(_ch) {}
    /// ���� ����������� ������������� ������ ��������Range
    Range(Chan::E ch, E range);

    operator Range::E();
    
    pString Name() const;

    pString ToString(int8 divider);

    static pString ToString(Range::E range, int8 divider);
    /// ���������� ����������, ��������������� ������� ������� �����
    static float MaxVoltageOnScreen(Range::E range);

    void Change(int16 delta);
    /// ����������� ��� ��������� ����� ������ ��� ����� ������� ������������
    static void LoadBoth();

private:
    Chan::E ch;
};


struct RShift
{
public:
    /// ���� ������������� ����� ������ ��������
    RShift(Chan::E _ch) : ch(_ch) {};
    /// ���� ������������� ����� ���������� ��������
    RShift(Chan::E ch, int16 rShift);

    operator int16();

    /// �������� �� delta
    void Change(int16 delta);
    /// ��������� � ����������
    void Load();
    /// ���������� ��� �� ������
    static void DrawBoth();
    /// ������������� � ������
    static String ToString(int16 rShiftRel, Range::E range, int8 divider);

    static float ToAbs(int16 rShift, Range::E range);

    static int16 ToRel(float rShiftAbs, Range::E range);

    static bool ChangeMath(int delta);

private:
    /// ���������� ������ ������������� �������� �� �����
    void Draw();

    static const float absStep[Range::Count];
    /// ��� �������� ������������� �������� ��������
    static const int16 MIN = -480;
    /// ��� �������� ������������ ��������� ��������
    static const int16 MAX = 480;
    /// ��� �������, ������� ����� �������� � ���������� �����, ����� �������� �������� "0"
    static const int16 HARDWARE_ZERO = 500;
    /// �� ������� ������ ����� �������� �������� ��������, ����� ������ �������� �� ���������� ������������ �� ���� �����.
    static const int16 STEP = (((MAX - MIN) / 24) / 20);

    Chan::E ch;
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
    };
    /// � ������� ����� ����� ������ ��������
    ModeCouple(Chan::E _ch) : ch(_ch) {};
    /// � ������� ����� ����� ���������� ����� ��������
    ModeCouple(Chan::E ch, ModeCouple::E couple);

    operator ModeCouple::E();
    
    pString UGO() const;

private:
    Chan::E ch;
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
    TShift();
    explicit TShift(int tShift);

    operator int() { return shift; }

    int Min();
    int Zero();
    int Max();

    void Load();

    void Change(int delta);

    static String ToString(TBase::E tBase);

    static float ToAbs(int shift, TBase::E tBase);

    void Reset();

private:
    static void LoadReal();
    static void LoadRandomize();
    static const float absStep[TBase::Count];

    int shift;
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
    TrigLevel(Chan::E _ch = Chan::Count);

    TrigLevel(Chan::E ch, int16 level);

    operator int16();
    /// ��������� ������� ������������� � ���������� �����
    void Load();
    /// �������� ������� ������������� �� delta ������
    void Change(int16 delta);
    /// ���������� ����� ������� �������������
    void Find();

    void Draw();

private:
    static const int16 MIN = -480;
    static const int16 MAX = 480;
    static const int16 HARDWARE_ZERO = 500;
    static const int16 STEP = (((MAX - MIN) / 24) / 20);

    Chan::E ch;
};


struct VALUE
{
    static const uint8 AVE = 127;
    static const uint8 MIN = AVE - 125;
    static const uint8 MAX = AVE + 125;
    /// ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������
    static const uint8 NONE = 0;

    static void PointsToVoltage(const uint8 *points, uint numPoints, Range::E range, int16 rShift, float *voltage);

    static void PointsFromVoltage(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    static uint8 FromVoltage(float voltage, Range::E range, int16 rShift);

    static float ToVoltage(uint8 value, Range::E range, int16 rShift);

private:
    static const float voltsInPoint[Range::Count];
};

