#include "defines.h"
#include "Data/DataSettings.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include <cstring>


void DataSettings::Fill(uint8 *_dataA, uint8 * _dataB)
{
    dataA = _dataA;
    dataB = _dataB;

    Lval_ENABLED_A(this) = ChanA.IsEnabled() ? 1U : 0U;
    Lval_ENABLED_B(this) = ChanB.IsEnabled() ? 1U : 0U;
    INVERSE_A(this) = ChanA.IsInversed() ? 1U : 0U;
    INVERSE_B(this) = ChanB.IsInversed() ? 1U : 0U;
    Lval_RANGE_A(this) = Range(Chan::A);
    Lval_RANGE_B(this) = Range(Chan::B);
    RSHIFT_A(this) = RShift(Chan::A);
    RSHIFT_B(this) = RShift(Chan::B);
    Lval_TBASE(this) = TBase();
    TSHIFT(this) = TShift();
    Lval_COUPLE_A(this) = ModeCouple(Chan::A);
    Lval_COUPLE_B(this) = ModeCouple(Chan::B);
    TRIGLEV_A(this) = TrigLevel(Chan::A).Value();
    TRIGLEV_B(this) = TrigLevel(Chan::B).Value();
    Lval_PEAKDET(this) = PeakDetMode();
    Lval_DIVIDER_A(this) = Divider(Chan::A);
    Lval_DIVIDER_B(this) = Divider(Chan::B);
    TIME_MS_DS(this) = 0;                        // ��� ����� ��� ������ ���������� ������. ��������, ��� ������ ������ ��� �� ������
    ENUM_POINTS(this) = ENumPointsFPGA();
}


bool DataSettings::Equals(const DataSettings &ds) const
{
    return
        RANGE_A(this) == Lval_RANGE_A(&ds) &&
        RANGE_B(this) == Lval_RANGE_B(&ds) &&
        RSHIFT_A(this) == RSHIFT_A(&ds) &&
        RSHIFT_B(this) == RSHIFT_B(&ds) &&
        TBASE(this) == TBASE(&ds) &&
        TSHIFT(this) == TSHIFT(&ds) &&
        TRIGLEV_A(this) == TRIGLEV_A(&ds) &&
        TRIGLEV_B(this) == TRIGLEV_B(&ds) &&
        ENUM_POINTS(this) == ENUM_POINTS(&ds);
}


uint DataSettings::BytesInChannel() const
{
    uint result = ENUM_TO_REL_POINTS(ENUM_POINTS(this));

    if (PEAKDET(this) == PeakDetMode::Enabled)
    {
        result *= 2;
    }

    return result;
}


uint DataSettings::PointsInChannel() const
{
    return ENUM_TO_REL_POINTS(ENUM_POINTS(this));
}


uint DataSettings::NeedMemoryForData() const
{
    uint result = 0;

    if (enableA)
    {
        result += BytesInChannel();
    }

    if (enableB)
    {
        result += BytesInChannel();
    }

    return result;
}



#define CHANGE(field, delta, max)                               \
if(delta > 0) { field = (field < (max)) ? ++field : 0; }        \
else          { field = (field > 0)         ? --field : (max); }


void PackedTime::ChangeHours(int delta)
{
    CHANGE(hours, delta, 23);
}


void PackedTime::ChangeMinutes(int delta)
{
    CHANGE(minutes, delta, 59);
}


void PackedTime::ChangeSeconds(int delta)
{
    CHANGE(seconds, delta, 59);
}


void PackedTime::ChangeDay(int delta)
{
    CHANGE(day, delta, 31);
}


void PackedTime::ChangeMonth(int delta)
{
    CHANGE(month, delta, 12);
}


void PackedTime::ChangeYear(int delta)
{
    CHANGE(year, delta, 50);
}


void FrameP2P::Clear()
{
    Prepare(nullptr);
}


void FrameP2P::Prepare(DataSettings *_ds)
{
    ds = _ds;
    numBytes = 0;
}


void FrameP2P::AddPoints(BitSet16 a, BitSet16 b)
{
    uint length = ds->BytesInChannel();

    uint position = numBytes;

    while (position >= length)
    {
        position -= length;
    }

    if (ds->peackDet)
    {
        uint16 *dA = reinterpret_cast<uint16 *>(ds->dataA);
        uint16 *dB = reinterpret_cast<uint16 *>(ds->dataB);

        if (ds->enableA)
        {
            dA[position] = a.halfWord;
        }
        if (ds->enableB)
        {
            dB[position] = b.halfWord;
        }

        numBytes += 2;
    }
    else
    {
        uint8 *dA = ds->dataA;
        uint8 *dB = ds->dataB;

        if (ds->enableA)
        {
            dA[position] = a.byte0;
        }
        if (ds->enableB)
        {
            dB[position] = b.byte0;
        }

        numBytes++;
    }
}


uint FrameP2P::ReadedBytesForChannel() const
{
    if (ds == nullptr)
    {
        return 0;
    }

    return numBytes;
}


void FrameP2P::FillBufferForDraw(Chan::E _ch, Buffer *buffer, bool redraw)
{
    ch = _ch;
    currentByte = 0;

    buffer->Fill(VALUE::NONE);

    if (ds == nullptr || !ENABLED(ds, ch))
    {
        return;
    }

    uint readedBytes = ReadedBytesForChannel();

    if (readedBytes <= buffer->Size())
    {
        FillBufferSimple(buffer);
    }
    else if(redraw)
    {
        FillBufferRedraw(buffer);
    }
    else
    {
        FillBufferNoRedraw(buffer);
    }
}


void FrameP2P::FillBufferSimple(Buffer *buffer)
{
    std::memcpy(buffer->data, ds->dataA, ReadedBytesForChannel());
}


void FrameP2P::FillBufferRedraw(Buffer *buffer)
{
    uint8 data = GetNextByte();

    uint pointer = 0;

    while (data != VALUE::NONE)
    {
        buffer->data[pointer++] = data;
        if (pointer == buffer->Size())
        {
            pointer = 0;
        }
        data = GetNextByte();
    }
}


void FrameP2P::FillBufferNoRedraw(Buffer* buffer)
{
    FillBufferRedraw(buffer);
}


uint8 FrameP2P::GetNextByte()
{
    return (currentByte < numBytes) ? GetByte(currentByte++) : VALUE::NONE;
}


uint8 FrameP2P::GetByte(uint)
{
    return VALUE::NONE;
}


void DataSettings::Log() const
{
    if (enableA)
    {
        uint8 *data = dataA;
        LOG_WRITE("A: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
            data[ 0], data[ 1], data[ 2], data[ 3], data[ 4], data[ 5], data[ 6], data[ 7], data[ 8], data[ 9],
            data[10], data[11], data[12], data[13], data[14], data[15], data[16], data[17], data[18], data[19]);
    }
    else
    {
        LOG_WRITE("A: ����");
    }

    if (enableB)
    {
        uint8 *data = dataB;
        LOG_WRITE("B: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
            data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
            data[10], data[11], data[12], data[13], data[14], data[15], data[16], data[17], data[18], data[19]);
    }
    else
    {
        LOG_WRITE("B: ����");
    }
}
