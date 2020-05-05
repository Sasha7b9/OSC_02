#pragma once
#include "Utils/String.h"


#define ChanA Chan::A
#define ChanB Chan::B


struct DataSettings;
struct SettingsChannel;


struct Chan
{
    enum E
    {
        A,
        B,
        Count
    };

    static pString Name(Chan::E ch);
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

    static pString Name(TBase::E tBase);

    static pString ToString(TBase::E tBase);

    static void Set(E v = Count);

    static void Change(int delta);

    // � ����� �������� ������ ���������� ����� ����������� ������
    static const E MIN_P2P = _10ms;
    
    // ����������� ������� �� �������, ��� ������� ��� �������� ��������� ������ �������� ���������
    static const E MIN_PEAK_DET = _5us;

    // ���������� ����� ������� � ������ �������������
    static int DeltaPoint();

    // ����� ����� �������
    static float TimePoint(TBase::E base);
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

    static void Set(Chan::E ch, E range);

    static pString Name(Chan::E ch);

    static pString ToString(Chan::E ch, int8 divider);

    static pString ToString(Range::E range, int8 divider);
    // ���������� ����������, ��������������� ������� ������� �����
    static float MaxVoltageOnScreen(Range::E range);

    static void Change(Chan::E ch, int16 delta);
    // ����������� ��� ��������� ����� ������ ��� ����� ������� ������������
    static void LoadBoth();
};


struct RShift
{
public:

    // ���� ������������� ����� ���������� ��������
    static void Set(Chan::E ch, int16 rShift);

    // �������� �� delta
    static void Change(Chan::E ch, int16 delta);

    // ��������� � ����������
    static void Load(Chan::E ch);

    // ���������� ��� �� ������
    static void DrawBoth();

    // ������������� � ������
    static String ToString(int16 rShiftRel, Range::E range, int8 divider);

    static float ToAbs(int16 rShift, Range::E range);

    static int16 ToRel(float rShiftAbs, Range::E range);

    static bool ChangeMath(int delta);

private:

    // ���������� ������ ������������� �������� �� �����
    static void Draw(Chan::E ch);

    static const float absStep[Range::Count];

    // ��� �������� ������������� �������� ��������
    static const int16 MIN = -480;

    // ��� �������� ������������ ��������� ��������
    static const int16 MAX = 480;

    // ��� �������, ������� ����� �������� � ���������� �����, ����� �������� �������� "0"
    static const int16 HARDWARE_ZERO = 500;

    // �� ������� ������ ����� �������� �������� ��������, ����� ������ �������� �� ���������� ������������ �� ���� �����.
    static const int16 STEP = (((MAX - MIN) / 24) / 20);
};

// ����� ������ �� �����.
struct ModeCouple
{
    enum E
    {
        DC,      // �������� ����
        AC,      // �������� ����
        GND,     // ���� �������.
        Count
    };

    static void Set(Chan::E ch, ModeCouple::E couple);

    static ModeCouple::E &Ref(Chan::E);

    static pString UGO(ModeCouple::E v);
};

struct Bandwidth
{
    enum E
    {
        Full,     // ���� ��� �������� ������� � ���� �����, �� ��� ���� ��������� ��������������� ������ �� �������-������-������.
        _20MHz,
    };

    Bandwidth(Chan::E c) : ch(c) {}
    static Bandwidth::E &Ref(Chan::E);
    void Load();
    bool Is20MHz() { return Ref(ch) == _20MHz; };

private:
    Chan::E ch;
};

struct TShift
{
    static void Set(int);

    static int Min();
    static int Max();

    static void Load();

    static void Change(const int delta);

    static String ToString(const TBase::E tBase = TBase::Count);

    static float ToAbs(const int shift, const TBase::E tBase);

    static void Reset();

    static void Draw();

private:
    static void LoadReal();

    static void LoadRandomize();
    // ���������� "����������" ����������� ������� ��������, ����� ������� �������� ��������� �� ������
    static void DrawNormal(const int x, const int y);
    // ���������� ������ ��������, ����� ������� �������� ��������� �� ����� �������� ������
    static void DrawLeft();
    // ���������� ������ ��������, ����� ������� �������� ��������� �� ������ �������� ������
    static void DrawRight();

    static const float absStep[TBase::Count];

    TShift() { };
};

struct Trig
{
    // ���������� true � ������ ������� ��������������
    static bool SyncPulse();

    static bool pulse;
};

struct TrigLevel
{
    static void Set(Chan::E ch, int16 level);

    // ��������� ������� ������������� � ���������� �����
    static void Load();

    // �������� ������� ������������� �������� �������� ������������� �� delta ������
    static void Change(int16 delta);

    // ���������� ����� ������� �������������
    static void Find();

    static void Draw();

private:
    static const int16 MIN = -480;
    static const int16 MAX = 480;
    static const int16 HARDWARE_ZERO = 500;
    static const int16 STEP = (((MAX - MIN) / 24) / 20);
};


struct VALUE
{
    static const uint8 AVE = 127;
    static const uint8 MIN = AVE - 125;
    static const uint8 MAX = AVE + 125;
    // ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������
    static const uint8 NONE = 0;

    static void PointsToVoltage(const uint8 *points, uint numPoints, Range::E range, int16 rShift, float *voltage);

    static void PointsFromVoltage(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    static uint8 FromVoltage(float voltage, Range::E range, int16 rShift);

    static float ToVoltage(uint8 value, Range::E range, int16 rShift);

private:
    static const float voltsInPoint[Range::Count];
};

