#pragma once
#include "Settings/SettingsOsci.h"


class Buffer;


struct PackedTime
{
    unsigned timeMS : 32;   /// \brief ����� � ������������� �� ������ �������. �.�. ��������� ����������� �� ����� ���������� ������ � ���������, �� 
                              /// timeMS == 0 ��������, ��� ������ ������ � ������ ���������� ������ ��� �� ������
    unsigned hours : 5;
    unsigned minutes : 6;
    unsigned seconds : 6;
    unsigned year : 7;
    unsigned month : 4;
    unsigned notUsed0 : 4;
    unsigned day : 5;
    unsigned notUsed1 : 27;
    PackedTime(uint h = 11, uint m = 11, uint s = 11, uint d = 11, uint mo = 11, uint y = 11) :
        timeMS(0), hours(h), minutes(m), seconds(s), year(y), month(mo), notUsed0(0), day(d), notUsed1(0) {};
    /// ��������� �������� ���� �� +/- 1
    void ChangeHours(int delta);
    void ChangeMinutes(int delta);
    void ChangeSeconds(int delta);
    void ChangeDay(int delta);
    void ChangeMonth(int delta);
    void ChangeYear(int delta);
};

struct DataSettings
{
    uint        id;                 ///< �������� id ������. 1-� ����� �������� ������, ��������� ������� ����� ��������� ������� � ������ ��������� ������������� �� 1
    uint8       *dataA;             ///< �� ����� ������ �������� ������ 1-�� ������
    uint8       *dataB;             ///< �� ����� ������ �������� ������ 2-�� ������. ��� �������� � Buffer ������ 2-�� ������ ���� ����� ����� 1-�� ������
    int16       rShift[2];
    int16       trigLev[2];
    int         tShift;             ///< �������� �� �������
    Range::E    range[2];           ///< ������� �� ���������� ����� �������.
    uint        tBase       : 5;    ///< ������� �� �������
    uint        enableA     : 1;    ///< ������� �� ����� A
    uint        enableB     : 1;    ///< ������� �� ����� B
    uint        coupleA     : 2;    ///< ����� ������ �� �����
    uint        coupleB     : 2;
    uint        peackDet    : 2;    ///< ������� �� ������� ��������
    uint        inverseA    : 1;
    uint        inverseB    : 1;
    uint        multiplierA : 1;
    uint        multiplierB : 1;
    uint        enumPoints  : 3;
    uint        numInROM    : 5;    ///< ����� ������ � ������ ROM
    uint        notUsed     : 7;
    PackedTime  time;
    /// ��������� ��������� � ������������ � �������� �����������
    void Fill(uint8 *dataA = 0, uint8 *dataB = 0);
    /// ���������� ������ ���������� ������� ������ ������
    uint BytesInChannel() const;
    /// ���������� �������� �� �����
    uint PointsInChannel() const;
    /// ���������� ��������� ���������� ������ ��� �������� ������ �������
    uint NeedMemoryForData() const;
    bool Equals(const DataSettings &ds) const;
    void Log() const;
};


#define Lval_ENABLED_A(ds)      ((ds)->enableA)
#define Lval_ENABLED_B(ds)      ((ds)->enableB)
#define Lval_ENABLED(ds, ch)    (ch.IsA() ? Lval_ENABLED_A(ds) : Lval_ENABLED_B(ds))
#define ENABLED_A(ds)           ((bool)Lval_ENABLED_A(ds))
#define ENABLED_B(ds)           ((bool)Lval_ENABLED_B(ds))
#define ENABLED(ds, ch)         (((ch) == Chan::A) ? ENABLED_A(ds) : ENABLED_B(ds))

#define ENABLED_DS(ch)          (ENABLED(DS, ch))
#define ENABLED_DS_A            ENABLED_A(DS)
#define ENABLED_DS_B            ENABLED_B(DS)

#define RSHIFT(ds, ch)          ((ds)->rShift[static_cast<int>(ch)])
#define RSHIFT_A(ds)            (RSHIFT(ds, Chan::A))
#define RSHIFT_B(ds)            (RSHIFT(ds, Chan::B))

#define Lval_RANGE(ds, ch)      ((ds)->range[static_cast<int>(ch)])
#define Lval_RANGE_A(ds)        (Lval_RANGE(ds, Chan::A))
#define Lval_RANGE_B(ds)        (Lval_RANGE(ds, Chan::B))
#define RANGE(ds, ch)           ((Range::E)Lval_RANGE(ds, ch))
#define RANGE_A(ds)             (RANGE(ds, Chan::A))
#define RANGE_B(ds)             (RANGE(ds, Chan::B))

#define Lval_TBASE(ds)          ((ds)->tBase)
#define TBASE(ds)               ((TBase::E)(Lval_TBASE(ds)))


#define TIME_TIME(ds)           ((ds)->time)
#define TIME_DAY(ds)            ((ds)->time.day)
#define TIME_HOURS(ds)          ((ds)->time.hours)
#define TIME_MINUTES(ds)        ((ds)->time.minutes)
#define TIME_SECONDS(ds)        ((ds)->time.seconds)
#define TIME_MONTH(ds)          ((ds)->time.month)
#define TIME_YEAR(ds)           ((ds)->time.year)
#define TIME_MS_DS(ds)          ((ds)->time.timeMS)

#define INVERSE_A(ds)           ((ds)->inverseA)
#define INVERSE_B(ds)           ((ds)->inverseB)
#define INVERSE(ds, ch)         (ch.IsA() ? INVERSE_A(ds) : INVERSE_B(ds))

#define TSHIFT(ds)              ((ds)->tShift)

#define Lval_COUPLE_A(ds)       ((ds)->coupleA)
#define Lval_COUPLE_B(ds)       ((ds)->coupleB)
#define COUPLE_A(ds)            ((ModeCouple)Lval_COUPLE_A(ds))
#define COUPLE_B(ds)            ((ModeCouple)Lval_COUPLE_B(ds))
#define COUPLE(ds, ch)          (ch.IsA() ? COUPLE_A(ds) : COUPLE_B(ds))

#define TRIGLEV(ds, ch)         ((ds)->trigLev[ch])
#define TRIGLEV_A(ds)           (TRIGLEV(ds, Chan::A))
#define TRIGLEV_B(ds)           (TRIGLEV(ds, Chan::B))

#define Lval_PEAKDET(ds)        ((ds)->peackDet)
#define PEAKDET(ds)             ((PeakDetMode::E)Lval_PEAKDET(ds))
#define PEAKDET_DISABLED(ds)     (PEAKDET(ds) == PeakDetMode::Disabled)

#define Lval_DIVIDER_A(ds)      ((ds)->multiplierA)
#define Lval_DIVIDER_B(ds)      ((ds)->multiplierB)

#define ENUM_POINTS(ds)         ((ds)->enumPoints)
#define BYTES_IN_CHANNEL(ds)    ((uint)(ds)->BytesInChannel())


struct FrameP2P
{
    uint numPoints;
    DataSettings *ds;
    void Clear();
    void Prepare(DataSettings *ds);
    void FillBufferForDraw(Chan::E ch, Buffer *buffer);
    void AddPoints(BitSet16 dataA, BitSet16 dataB);
};
