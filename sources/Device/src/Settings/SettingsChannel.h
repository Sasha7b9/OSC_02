#pragma once
#include "Utils/String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsChannel Settings Chan
 *  @{
 */

 /// ��������.
struct Divider
{
    enum E
    {
        _1,
        _10
    } value;
    explicit Divider(E v) : value(v) { };
    explicit Divider(uint v) : value((E)v) { };
    int ToAbs() const { return (value == _1) ? 1 : 10; };
};

struct DataSettings;

struct Chan
{
    enum E
    {
        A,
        B,
        Ext,
        Math,
        Number
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

#define SET_INVERSE(ch)         (set.chan_inverse[ch])
#define SET_INVERSE_A           (SET_INVERSE(Chan::A))
#define SET_INVERSE_B           (SET_INVERSE(Chan::B))
#define SET_RSHIFT_MATH         (set.math_rShift)
#define SET_RANGE_MATH          (set.math_range)

#define SET_RANGE(ch)           (set.chan_range[ch])
#define SET_RANGE_A             (SET_RANGE(Chan::A))
#define SET_RANGE_B             (SET_RANGE(Chan::B))

#define SET_RECORD_RANGE(ch)    (set.rec_range[ch])
#define SET_RECORD_RANGE_A      (SET_RECORD_RANGE(Chan::A))
#define SET_RECORD_RANGE_B      (SET_RECORD_RANGE(Chan::B))

#define SET_RSHIFT(ch)          (set.chan_shift[ch])
#define SET_RSHIFT_A            (SET_RSHIFT(Chan::A))
#define SET_RSHIFT_B            (SET_RSHIFT(Chan::B))

#define SET_COUPLE(ch)          (set.chan_couple[ch])
#define SET_COUPLE_A            (SET_COUPLE(Chan::A))
#define SET_COUPLE_B            (SET_COUPLE(Chan::B))

#define SET_DIVIDER(ch)         (set.chan_divider[ch])
#define SET_DIVIDER_A           (SET_DIVIDER(Chan::A))
#define SET_DIVIDER_B           (SET_DIVIDER(Chan::B))
#define SET_DIVIDER_10(ch)      (SET_DIVIDER(ch) == Divider::_10)
#define SET_DIVIDER_1(ch)       (SET_DIVIDER(ch) == Divider::_1)

#define SET_ENABLED(ch)         (set.chan_enabled[ch])
#define SET_ENABLED_A           (SET_ENABLED(Chan::A))
#define SET_ENABLED_B           (SET_ENABLED(Chan::B))
#define SET_ENABLED_BOTH        (SET_ENABLED_A && SET_ENABLED_B)

#define SET_BALANCE_ADC(ch)     (set.chan_balanceShiftADC[ch])
#define SET_BALANCE_ADC_A       (SET_BALANCE_ADC(Chan::A))
#define SET_BALANCE_ADC_B       (SET_BALANCE_ADC(Chan::B))

#define SET_BANDWIDTH(ch)       (set.chan_bandwidth[ch])
#define SET_BANDWIDTH_A         (SET_BANDWIDTH(Chan::A))
#define SET_BANDWIDTH_B         (SET_BANDWIDTH(Chan::B))
#define SET_BANDWIDTH_20MHz(ch) (SET_BANDWIDTH(ch) == Bandwidth_20MHz)

#define SET_RESISTANCE(ch)      (set.chan_resistance[ch])
#define SET_RESISTANCE_A        (SET_RESISTANCE(Chan::A))
#define SET_RESISTANCE_B        (SET_RESISTANCE(Chan::B))

#define SET_CALIBR_MODE(ch)     (set.chan_calibrationMode[ch])
#define SET_CALIBR_MODE_A       (SET_CALIBR_MODE(Chan::A))
#define CALIBR_MODE_B           (SET_CALIBR_MODE(Chan::B))


/** @}  @}
 */
