#include "defines.h"
#include "Data/Reader.h"
#include "FPGA/FPGA.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Smoother.h"
#include "Utils/Values.h"
#include <cmath>
#include <cstdlib>
#include <cstring>


static float CalculateVoltageMax(Chan::E ch);
static float CalculateVoltageMin(Chan::E ch);
static float CalculateVoltagePic(Chan::E ch);
static float CalculateVoltageMaxSteady(Chan::E ch);
static float CalculateVoltageMinSteady(Chan::E ch);
static float CalculateVoltageAmpl(Chan::E ch);
static float CalculateVoltageAverage(Chan::E ch);
static float CalculateVoltageRMS(Chan::E ch);
static float CalculateVoltageVybrosPlus(Chan::E ch);
static float CalculateVoltageVybrosMinus(Chan::E ch);
static float CalculatePeriod(Chan::E ch);
static int   CalculatePeriodAccurately(Chan::E ch); ///< ����� ��������� ������ ��� ����� ����� �������� � ������ �������.
static float CalculateFreq(Chan::E ch);
static float CalculateTimeNarastaniya(Chan::E ch);
static float CalculateTimeSpada(Chan::E ch);
static float CalculateDurationPlus(Chan::E ch);
static float CalculateDurationMinus(Chan::E ch);
static float CalculateSkvaznostPlus(Chan::E ch);
static float CalculateSkvaznostMinus(Chan::E ch);


static float CalculateMinRel(Chan::E ch);           ///< ���������� ����������� �������� �������������� �������.
static float CalculateMinSteadyRel(Chan::E ch);     ///< ���������� ����������� �������������� �������� �������������� �������.
static float CalculateMaxRel(Chan::E ch);           ///< ���������� ������������ �������� �������������� �������.
static float CalculateMaxSteadyRel(Chan::E ch);     ///< ���������� ������������ �������������� �������� �������������� �������.
static float CalculateAverageRel(Chan::E ch);       ///< ���������� �������� �������� �������������� �������.
static float CalculatePicRel(Chan::E ch);
static float CalculateDelayPlus(Chan::E ch);
static float CalculateDelayMinus(Chan::E ch);
static float CalculatePhazaPlus(Chan::E ch);
static float CalculatePhazaMinus(Chan::E ch);
/// \brief ����� ����� ����������� ������� � �������������� ������, ���������� �� ������ yLine. numItersection - ���������� ����� �����������, 
/// ���������� � 1. downToTop - ���� true, ���� ����������� ������� �� ������� ������ ��� ����������� �� "-" � "+".
static float FindIntersectionWithHorLine(Chan::E ch, int numIntersection, bool downToUp, uint8 yLine);
/// ������ val1 �� val2. ���������� nan, ���� ��������� ����������
static float Divide(float val1, float val2);

static bool isSet = false;          ///< ���� true, �� ������ ��������.

static int firstByte = 0;
static int lastByte = 0;
/// ���������� ����, �� ������� ������������ �������
static int nBytes = 0;


typedef float (*pFuncFCh)(Chan::E);
typedef char *(*pFuncPCFBPC)(float, bool, char*);

typedef struct
{
    const char *name;
    pFuncFCh    FuncCalculate;
    pFuncPCFBPC FucnConvertate;
    bool        showSign;           ///< ���� true, ����� ���������� ����.
    uint8       notUsed0[3];
} MeasureCalculate;

static char* Time2String(float, bool, char buffer[20]);
static char* Voltage2String(float, bool, char buffer[20]);
static char* Phase2String(float, bool, char buffer[20]);


static const MeasureCalculate sMeas[TypeMeasure::Count] =
{
    {"", 0, 0, false, {}},
    {"CalculateVoltageMax",         CalculateVoltageMax,         Voltage2String,                    true,  {}},
    {"CalculateVoltageMin",         CalculateVoltageMin,         Voltage2String,                    true,  {}},
    {"CalculateVoltagePic",         CalculateVoltagePic,         Voltage2String,                    false, {}},
    {"CalculateVoltageMaxSteady",   CalculateVoltageMaxSteady,   Voltage2String,                    true,  {}},
    {"CalculateVoltageMinSteady",   CalculateVoltageMinSteady,   Voltage2String,                    true,  {}},
    {"CalculateVoltageAmpl",        CalculateVoltageAmpl,        Voltage2String,                    false, {}},
    {"CalculateVoltageAverage",     CalculateVoltageAverage,     Voltage2String,                    true,  {}},
    {"CalculateVoltageRMS",         CalculateVoltageRMS,         Voltage2String,                    false, {}},
    {"CalculateVoltageVybrosPlus",  CalculateVoltageVybrosPlus,  Voltage2String,                    false, {}},
    {"CalculateVoltageVybrosMinus", CalculateVoltageVybrosMinus, Voltage2String,                    false, {}},
    {"CalculatePeriod",             CalculatePeriod,             Time2String,                       false, {}},
    {"CalculateFreq",               CalculateFreq,               AutoMeasurements::Freq2String,     false, {}},
    {"CalculateTimeNarastaniya",    CalculateTimeNarastaniya,    Time2String,                       false, {}},
    {"CalculateTimeSpada",          CalculateTimeSpada,          Time2String,                       false, {}},
    {"CalculateDurationPlus",       CalculateDurationPlus,       Time2String,                       false, {}},
    {"CalculateDurationPlus",       CalculateDurationMinus,      Time2String,                       false, {}},
    {"CalculateSkvaznostPlus",      CalculateSkvaznostPlus,      AutoMeasurements::Float2String,    false, {}},
    {"CalculateSkvaznostMinus",     CalculateSkvaznostMinus,     AutoMeasurements::Float2String,    false, {}},
    {"CalculateDelayPlus",          CalculateDelayPlus,          Time2String,                       false, {}},
    {"CalculateDelayMinus",         CalculateDelayMinus,         Time2String,                       false, {}},
    {"CalculatePhazaPlus",          CalculatePhazaPlus,          Phase2String,                      false, {}},
    {"CalculatePhazaMinus",         CalculatePhazaMinus,         Phase2String,                      false, {}}
};


int AutoMeasurements::markerTime[Chan::Count][2] = {{Integer::ERROR}, {Integer::ERROR}};
int AutoMeasurements::markerVoltage[Chan::Count][2] = {{Integer::ERROR}, {Integer::ERROR}};
int8 AutoMeasurements::posActive = 0;

typedef struct
{
    float value[2];
} MeasureValue;

static MeasureValue values[TypeMeasure::Count] = {{{0.0F, 0.0F}}};

static bool maxIsCalculating[2] = {false, false};
static bool minIsCalculating[2] = {false, false};
static bool maxSteadyIsCalculating[2] = {false, false};
static bool minSteadyIsCalculating[2] = {false, false};
static bool aveIsCalculating[2] = {false, false};
static bool periodIsCaclulating[2] = {false, false};
static bool periodAccurateIsCalculating[2];
static bool picIsCalculating[2] = {false, false};

#define EXIT_IF_ERROR_FLOAT(x)      if(isnan(x))                return Float::ERROR;
#define EXIT_IF_ERRORS_FLOAT(x, y)  if(isnan(x) || isnan(y))    return Float::ERROR;
#define EXIT_IF_ERROR_INT(x)        if((x) == Integer::ERROR)   return Float::ERROR;

/// ������� ����� ������ ������ ch
#define CHOICE_BUFFER (OUT(ch))

/// ������ �� IN_A, IN_B ����������� � ������� ���������� � �������� � OUT_A, OUT_B
static void CountedToCurrentSettings();
static void CountedToCurrentSettings(Chan::E ch, uint numBytes);
/// �������� �������� ������ ch �� ��������� � ��������
static void CountedToCurrentRShift(Chan::E ch, uint numBytes);
/// ������� �������� ������ � ����������� ��������
static void LimitationData(Chan::E ch, uint numBytes);



void AutoMeasurements::CalculateMeasures()
{
    if(!set.meas.show || !isSet)
    {
        return;
    }
    
    maxIsCalculating[0] = maxIsCalculating[1] = maxSteadyIsCalculating[0] = maxSteadyIsCalculating[1] = false;
    minIsCalculating[0] = minIsCalculating[1] = minSteadyIsCalculating[0] = minSteadyIsCalculating[1] = false;
    aveIsCalculating[0] = aveIsCalculating[1] = false;
    periodIsCaclulating[0] = periodIsCaclulating[1] = false;
    periodAccurateIsCalculating[0] = periodAccurateIsCalculating[1] = false;
    picIsCalculating[0] = picIsCalculating[1] = false;

    for(int str = 0; str < TableMeasures::NumRows(); str++)
    {
        for(int elem = 0; elem < TableMeasures::NumCols(); elem++)
        {
            Measure measure(str, elem);
            TypeMeasure::E type = measure.GetType();

            pFuncFCh func = sMeas[static_cast<int>(type)].FuncCalculate;
            if(func)
            {
                if(type == set.meas.marked || set.meas.marked == TypeMeasure::None)
                {
                    markerTime[Chan::A][0] = markerTime[Chan::A][1] = markerTime[Chan::B][0] = markerTime[Chan::B][1] = Integer::ERROR;
                    markerVoltage[Chan::A][0] = markerVoltage[Chan::A][1] = markerVoltage[Chan::B][0] = markerVoltage[Chan::B][1] = Integer::ERROR;
                }
                
                if(VIEW_MEASURES_A)
                {
                    values[static_cast<int>(type)].value[Chan::A] = func(Chan::A);
                }
                
                if(VIEW_MEASURES_B)
                {
                    values[static_cast<int>(type)].value[Chan::B] = func(Chan::B);
                }
            }
        }
    }
}



float CalculateVoltageMax(Chan::E ch)
{
    float max = CalculateMaxRel(ch);
    EXIT_IF_ERROR_FLOAT(max); //-V2507
    if(set.meas.marked == TypeMeasure::VoltageMax)
    {
        Measure::SetMarkerVoltage(ch, 0, max);      // ����� �� ���������, ������ ��� max ����� ���� ������ �����
    }
    
    Range::E range = RANGE_DS(ch);

    int16 rShift = RSHIFT_DS(ch);

    uint8 value = ROUND(uint8, max);

    return VALUE::ToVoltage(value, range, rShift);
}



float CalculateVoltageMin(Chan::E ch)
{
    float min = CalculateMinRel(ch);
    EXIT_IF_ERROR_FLOAT(min); //-V2507
    if(set.meas.marked == TypeMeasure::VoltageMin)
    {
        Measure::SetMarkerVoltage(ch, 0, min);             // ����� �� ���������, ������ ��� min ����� ���� ������ �����
    }
    
    return VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch),RSHIFT_DS(ch));
}



float CalculateVoltagePic(Chan::E ch)
{
    float max = CalculateVoltageMax(ch);
    float min = CalculateVoltageMin(ch);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2507

    if(set.meas.marked == TypeMeasure::VoltagePic)
    {
        Measure::SetMarkerVoltage(ch, 0, CalculateMaxRel(ch));
        Measure::SetMarkerVoltage(ch, 1, CalculateMinRel(ch));
    }
    return max - min;
}



float CalculateVoltageMinSteady(Chan::E ch)
{
    float min = CalculateMinSteadyRel(ch);
    EXIT_IF_ERROR_FLOAT(min); //-V2507
    if(set.meas.marked == TypeMeasure::VoltageMinSteady)
    {
        Measure::SetMarkerVoltage(ch, 0, ROUND(float, min));
    }

    return VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch), RSHIFT_DS(ch));
}



float CalculateVoltageMaxSteady(Chan::E ch)
{
    float max = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERROR_FLOAT(max); //-V2507

    if(set.meas.marked == TypeMeasure::VoltageMaxSteady)
    {
        Measure::SetMarkerVoltage(ch, 0, max);
    }

    return VALUE::ToVoltage(ROUND(uint8, max), RANGE_DS(ch), RSHIFT_DS(ch));
}



float CalculateVoltageVybrosPlus(Chan::E ch)
{
    float max = CalculateMaxRel(ch);
    float maxSteady = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady); //-V2507

    if (set.meas.marked == TypeMeasure::VoltageVybrosPlus)
    {
        Measure::SetMarkerVoltage(ch, 0, max);
        Measure::SetMarkerVoltage(ch, 1, maxSteady);
    }

    int16 rShift = RSHIFT_DS(ch);
    return std::fabsf(VALUE::ToVoltage(ROUND(uint8, maxSteady), RANGE_DS(ch), rShift) - VALUE::ToVoltage(ROUND(uint8, max), RANGE_DS(ch), rShift));
}



float CalculateVoltageVybrosMinus(Chan::E ch)
{
    float min = CalculateMinRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);
    EXIT_IF_ERRORS_FLOAT(min, minSteady); //-V2507

    if (set.meas.marked == TypeMeasure::VoltageVybrosMinus)
    {
        Measure::SetMarkerVoltage(ch, 0, min);
        Measure::SetMarkerVoltage(ch, 1, minSteady);
    }

    int16 rShift = RSHIFT_DS(ch);
    return std::fabsf(VALUE::ToVoltage(ROUND(uint8, minSteady), RANGE_DS(ch), rShift) - VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch), rShift));
}



float CalculateVoltageAmpl(Chan::E ch)
{
    float max = CalculateVoltageMaxSteady(ch);
    float min = CalculateVoltageMinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2507

    if(set.meas.marked == TypeMeasure::VoltageAmpl)
    {
        Measure::SetMarkerVoltage(ch, 0, CalculateMaxSteadyRel(ch));
        Measure::SetMarkerVoltage(ch, 1, CalculateMinSteadyRel(ch));
    }
    return max - min;
}



float CalculateVoltageAverage(Chan::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period); //-V2507

    int sum = 0;

    uint8 *data = &CHOICE_BUFFER[firstByte];

    for(int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = static_cast<uint8>(static_cast<float>(sum) / period);

    if(set.meas.marked == TypeMeasure::VoltageAverage)
    {
        Measure::SetMarkerVoltage(ch, 0, aveRel);
    }

    return VALUE::ToVoltage(aveRel, RANGE_DS(ch), RSHIFT_DS(ch));
}



float CalculateVoltageRMS(Chan::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period); //-V2507

    float rms = 0.0F;

    Range::E range = RANGE_DS(ch);
    int16 rShift = RSHIFT_DS(ch);

    uint8 *dataIn = &CHOICE_BUFFER[firstByte];

    for(int i = firstByte; i < firstByte + period; i++)
    {
        float volts = VALUE::ToVoltage(dataIn[i], range, rShift);
        rms +=  volts * volts;
    }

    rms = std::sqrtf(rms / period);

    if(set.meas.marked == TypeMeasure::VoltageRMS)
    {
        Measure::SetMarkerVoltage(ch, 0, VALUE::FromVoltage(rms, range, rShift));
    }

    return rms;
}



float CalculatePeriod(Chan::E ch)
{
    static float period[2] = {0.0F, 0.0F};

    static float firstIntersection = 0.0F;
    static float secondIntersection = 0.0F;

    if(!periodIsCaclulating[static_cast<int>(ch)])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == Uint8::ERROR) //-V550 //-V2550
        {
            period[static_cast<int>(ch)] = Float::ERROR;
        }
        else
        {
            float intersectionDownToTop = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));
            float intersectionTopToDown = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));

            EXIT_IF_ERRORS_FLOAT(intersectionDownToTop, intersectionTopToDown); //-V2507

            bool firstDownToTop = intersectionDownToTop < intersectionTopToDown;
            firstIntersection = firstDownToTop ? intersectionDownToTop : intersectionTopToDown;
            secondIntersection = FindIntersectionWithHorLine(ch, 2, firstDownToTop, static_cast<uint8>(aveValue));

            EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

            float per = TShift::ToAbs(ROUND(uint16, secondIntersection - firstIntersection), set.time.base);

            period[static_cast<int>(ch)] = per;

            periodIsCaclulating[static_cast<int>(ch)] = true;
        }
    }

    if ((set.meas.marked == TypeMeasure::Period || set.meas.marked == TypeMeasure::Freq) && periodIsCaclulating[static_cast<int>(ch)])
    {
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - firstByte);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - firstByte);
    }

    return period[static_cast<int>(ch)];
}



#define EXIT_FROM_PERIOD_ACCURACY                               \
    period[static_cast<int>(ch)] = Integer::ERROR;              \
    periodAccurateIsCalculating[static_cast<int>(ch)] = true;   \
    std::free(sums);                                            \
    return period[static_cast<int>(ch)];



int CalculatePeriodAccurately(Chan::E ch)
{
    static int period[2];

    int *sums = static_cast<int *>(std::malloc(static_cast<uint>(nBytes)));

    if (sums == 0)
    {
        LOG_ERROR("��� ������");
        return 0;
    }

    uint8 *dataIn = CHOICE_BUFFER;

    if(!periodAccurateIsCalculating[static_cast<int>(ch)])
    {
        period[static_cast<int>(ch)] = 0;

        float pic = CalculatePicRel(ch);

        if(pic == Float::ERROR) //-V550 //-V2550
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = static_cast<int>(pic * 5.0F);
        sums[firstByte] = dataIn[firstByte];

        int i = firstByte + 1;
        int *pSum = &sums[i];
        uint8 *data = &dataIn[i];
        uint8 *end = &dataIn[lastByte];

        while (data < end)
        {
            uint8 point = *data++;
            if(point < VALUE::MIN || point >= VALUE::MAX)
            {
                EXIT_FROM_PERIOD_ACCURACY
            }
            *pSum = *(pSum - 1) + point;
            pSum++;
        }

        int addShift = firstByte - 1;
        int maxPeriod = static_cast<int>(nBytes * 0.95F);

        for(int nextPeriod = 10; nextPeriod < maxPeriod; nextPeriod++)
        {
            int sum = sums[addShift + nextPeriod];

            int maxDelta = 0;
            int maxStart = nBytes - nextPeriod;

            int *pSums = &sums[firstByte + 1];

            for(int start = 1; start < maxStart; start++)
            {
                int nextSum = *(pSums + nextPeriod) - (*pSums);
                pSums++;

                int nextDelta = nextSum - sum;

                if (nextSum < sum)
                {
                    nextDelta = -nextDelta;
                }

                if(nextDelta > delta)
                {
                    maxDelta = delta + 1;
                    break;
                }
                else if(nextDelta > maxDelta)
                {
                    maxDelta = nextDelta;
                }
                else
                {
                    // ����� ������ �� ������
                }
            }

            if(maxDelta < delta)
            {
                delta = maxDelta;
                period[static_cast<int>(ch)] = nextPeriod;
            }
        }

        if(period[static_cast<int>(ch)] == 0)
        {
            period[static_cast<int>(ch)] = Integer::ERROR;
        }
        periodAccurateIsCalculating[static_cast<int>(ch)] = true;
    }

    std::free(sums);

    return period[static_cast<int>(ch)];
}



float CalculateFreq(Chan::E ch)
{
    float period = CalculatePeriod(ch);

    return (period == Float::ERROR) ? Float::ERROR : 1.0F / period; //-V550 //-V2550
}



float FindIntersectionWithHorLine(Chan::E ch, int numIntersection, bool downToUp, uint8 yLine)
{
    int num = 0;
    int x = firstByte;
    int compValue = lastByte - 1;
    int step = (set.time.peakDet == PeakDetMode::Enabled) ? 2 : 1;

    uint8 *data = &CHOICE_BUFFER[0];

    if(downToUp)
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] < yLine && data[x + step] >= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    else
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] > yLine && data[x + step] <= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    x -= step;

    if (num < numIntersection)
    {
        return Float::ERROR;
    }
    
    return ::Math::GetIntersectionWithHorizontalLine(x, data[x], x + step, data[x + step], yLine);
}



float CalculateDurationPlus(Chan::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2507

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, static_cast<uint8>(aveValue));
    }

    if (set.meas.marked == TypeMeasure::DurationPlus)
    {
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - firstByte);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), (TBase::E)TBASE_DS);
}



float CalculateDurationMinus(Chan::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2507

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, static_cast<uint8>(aveValue));
    }

    if (set.meas.marked == TypeMeasure::DurationMinus)
    {
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - firstByte);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float CalculateTimeNarastaniya(Chan::E ch)   /** \todo �����, ��������, ����� ��������� �������� - ����� �� ����� ������� ���������� ����� 
                                              ��������� �� �������, � ������������ ����������� �����. */
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady); //-V2507

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(min01));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(max09));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507
    
    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, static_cast<uint8>(max09));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    float retValue = TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);

    if (set.meas.marked == TypeMeasure::TimeNarastaniya)
    {
        Measure::SetMarkerVoltage(ch, 0, max09);
        Measure::SetMarkerVoltage(ch, 1, min01);
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - firstByte);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - firstByte);
    }

    return retValue;
}



float CalculateTimeSpada(Chan::E ch)        /// \todo ���������� ������� ����������
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady); //-V2507

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(max09));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(min01));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, static_cast<uint8>(min01));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    float retValue = TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);

    if (set.meas.marked == TypeMeasure::TimeSpada)
    {
        Measure::SetMarkerVoltage(ch, 0, max09);
        Measure::SetMarkerVoltage(ch, 1, min01);
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - SHIFT_IN_MEMORY);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - SHIFT_IN_MEMORY);
    }

    return retValue;
}



float CalculateSkvaznostPlus(Chan::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationPlus(ch);
    
    EXIT_IF_ERRORS_FLOAT(period, duration); //-V2507

    return Divide(period, duration);
}



float CalculateSkvaznostMinus(Chan::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationMinus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration); //-V2507

    return Divide(period, duration);
}



float CalculateMinSteadyRel(Chan::E ch)
{
    static float min[2] = {255.0F, 255.0F};

    uint8 *dataIn = CHOICE_BUFFER;

    if(!minSteadyIsCalculating[static_cast<int>(ch)])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == Float::ERROR) //-V550 //-V2550
        {
            min[static_cast<int>(ch)] = Float::ERROR;
        }
        else
        {
            int sum = 0;
            int numSums = 0;

            uint8 *data = &dataIn[firstByte];
            const uint8 * const end = &dataIn[lastByte];
            while(data <= end)
            {
                uint8 d = *data++;
                if(d < aveValue)
                {
                    sum += d; //-V127
                    numSums++; //-V127
                }
            }
            min[static_cast<int>(ch)] = static_cast<float>(sum) / numSums;
            int numMin = numSums;

            int numDeleted = 0;

            float pic = CalculatePicRel(ch);
            if (pic == Float::ERROR) //-V550 //-V2550
            {
                min[static_cast<int>(ch)] = Float::ERROR;
            }
            else
            {
                float value = pic / 9.0F;

                data = &dataIn[firstByte];
                float _min = min[static_cast<int>(ch)];
                while (data <= end)
                {
                    uint8 d = *data++;

                    if (d < aveValue)
                    {
                        if (d < _min)
                        {
                            if (_min - d > value)
                            {
                                sum -= d;
                                --numSums;
                                ++numDeleted; //-V127
                            }
                        }
                        else if (d - _min > value)
                        {
                            sum -= d;
                            --numSums;
                            ++numDeleted; //-V127
                        }
                        else
                        {
                            // ����� ������ ������ �� �����
                        }
                    }
                }
                min[static_cast<int>(ch)] = (numDeleted > numMin / 2.0F) ? CalculateMinRel(ch) : static_cast<float>(sum) / numSums;
            }
        }
        minSteadyIsCalculating[static_cast<int>(ch)] = true;
    }

    return min[static_cast<int>(ch)];
}



float CalculateMaxSteadyRel(Chan::E ch)
{
    static float max[2] = {255.0F, 255.0F};

    if(!maxSteadyIsCalculating[static_cast<int>(ch)])
    {
        uint8 *dataIn = CHOICE_BUFFER;

        float aveValue = CalculateAverageRel(ch);
        
        if(aveValue == Float::ERROR) //-V550 //-V2550
        {
            max[static_cast<int>(ch)] = Float::ERROR;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[firstByte];
            const uint8 * const end = &dataIn[lastByte];
            while (data <= end)
            {
                uint8 d = *data++;
                if(d > aveValue)
                {
                    sum += d; //-V127
                    numSums++; //-V127
                }
            }
            max[static_cast<int>(ch)] = static_cast<float>(sum) / numSums;
            int numMax = numSums;

            int numDeleted = 0;

            float pic = CalculatePicRel(ch);

            if (pic == Float::ERROR) //-V550 //-V2550
            {
                max[static_cast<int>(ch)] = Float::ERROR;
            }
            else
            {
                float value = pic / 9.0F;

                data = &dataIn[firstByte];
                uint8 _max = static_cast<uint8>(max[static_cast<int>(ch)]);

                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d > aveValue)
                    {
                        if (d > _max)
                        {
                            if (d - _max > value)
                            {
                                sum -= d;
                                numSums--;
                                numDeleted++; //-V127
                            }
                        }
                        else if (_max - d > value)
                        {
                            sum -= d;
                            numSums--;
                            numDeleted++; //-V127
                        }
                        else
                        {
                            // ����� ������ ������ �� �����
                        }
                    }
                }
                max[static_cast<int>(ch)] = (numDeleted > numMax / 2) ? CalculateMaxRel(ch) : static_cast<float>(sum) / numSums;
            }
        }
        maxSteadyIsCalculating[static_cast<int>(ch)] = true;
    }

    return max[static_cast<int>(ch)];
}



float CalculateMaxRel(Chan::E ch)
{
    static float max[2] = {0.0F, 0.0F};

    if(!maxIsCalculating[static_cast<int>(ch)])
    {
        uint8 val = Math::MaxFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        max[static_cast<int>(ch)] = (val == Uint8::ERROR) ? Float::ERROR : val;
        maxIsCalculating[static_cast<int>(ch)] = true;
    }

    return max[static_cast<int>(ch)];
}



float CalculateMinRel(Chan::E ch)
{
    static float min[2] = {255.0F, 255.0F};

    if (!minIsCalculating[static_cast<int>(ch)])
    {
        uint8 val = Math::MinFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        min[static_cast<int>(ch)] = (val == Uint8::ERROR) ? Float::ERROR : val;
        minIsCalculating[static_cast<int>(ch)] = true;
    }

    return min[static_cast<int>(ch)];
}



float CalculateAverageRel(Chan::E ch)
{
    static float _ave[2] = {0.0F, 0.0F};

    if(!aveIsCalculating[static_cast<int>(ch)])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        _ave[static_cast<int>(ch)] = (min == Float::ERROR || max == Float::ERROR) ? Float::ERROR : (min + max) / 2.0F; //-V550 //-V2550
        aveIsCalculating[static_cast<int>(ch)] = true;
    }
    return _ave[static_cast<int>(ch)];
}



float CalculatePicRel(Chan::E ch)
{
    static float pic[2] = {0.0F, 0.0F};

    if(!picIsCalculating[static_cast<int>(ch)])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        pic[static_cast<int>(ch)] = (min == Float::ERROR || max == Float::ERROR) ? Float::ERROR : max - min; //-V550 //-V2550
        picIsCalculating[static_cast<int>(ch)] = true;
    }

    return pic[static_cast<int>(ch)];
}



float CalculateDelayPlus(Chan::E ch)
{
    float periodA = CalculatePeriod(Chan::A);
    float periodB = CalculatePeriod(Chan::B);

    EXIT_IF_ERRORS_FLOAT(periodA, periodB); //-V2507
    if(!Math::FloatsIsEquals(periodA, periodB, 1.05F))
    {
        return Float::ERROR;
    }

    float averageA = CalculateAverageRel(Chan::A);
    float averageB = CalculateAverageRel(Chan::B);

    EXIT_IF_ERRORS_FLOAT(averageA, averageB); //-V2507

    float averageFirst = Chan(ch).IsA() ? averageA : averageB;
    float averageSecond = Chan(ch).IsA() ? averageB : averageA;
    Chan::E firstChannel = ch;
    Chan::E secondChannel = Chan(ch).IsA() ? Chan::B : Chan::A;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, true, static_cast<uint8>(averageFirst));
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, true, static_cast<uint8>(averageSecond));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, true, static_cast<uint8>(averageSecond));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float CalculateDelayMinus(Chan::E ch)
{
    float period0 = CalculatePeriod(Chan::A);
    float period1 = CalculatePeriod(Chan::B);

    EXIT_IF_ERRORS_FLOAT(period0, period1); //-V2507

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return Float::ERROR;
    }

    float average0 = CalculateAverageRel(Chan::A);
    float average1 = CalculateAverageRel(Chan::B);

    EXIT_IF_ERRORS_FLOAT(average0, average1); //-V2507

    float averageFirst = Chan(ch).IsA() ? average0 : average1;
    float averageSecond = Chan(ch).IsA() ? average1 : average0;
    Chan::E firstChannel = ch;
    Chan::E secondChannel = Chan(ch).IsA() ? Chan::B : Chan::A;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, false, static_cast<uint8>(averageFirst));
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, false, static_cast<uint8>(averageSecond));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, false, static_cast<uint8>(averageSecond));
    }

    /*
    if (MEAS_MARKED == Measure_DelayMinus)
    {
        markerTime[ch][0] = static_cast<int>((int16)firstIntersection - firstByte);
        markerTime[ch][1] = static_cast<int>((int16)secondIntersection - firstByte);
    }
    */

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float CalculatePhazaPlus(Chan::E ch)
{
    float delay = CalculateDelayPlus(ch);
    float period = CalculatePeriod(ch);
    if(delay == Float::ERROR || period == Float::ERROR) //-V550 //-V2550
    {
        return Float::ERROR;
    }
    return delay / period * 360.0F;
}



float CalculatePhazaMinus(Chan::E ch)
{
    float delay = CalculateDelayMinus(ch);
    float period = CalculatePeriod(ch);

    float result = Float::ERROR;

    if(delay != Float::ERROR && period != Float::ERROR) //-V550 //-V2550
    {
        result = delay / period * 360.0F;
    }
    
    return result;
}


float Measure::CalculateCursorU(Chan::E ch, float posCurT)
{
    if(!CHOICE_BUFFER)
    {
        return 0;
    }
    
    BitSet64 points = DisplayOsci::PainterData::PointsOnDisplay();

    int rel = static_cast<int>((CHOICE_BUFFER)[static_cast<int>(points.word0) + ROUND(int, posCurT)]) - VALUE::MIN;

#define SCALE (200.0F / (VALUE::MAX - VALUE::MIN))

    float value = 200.0F - rel * SCALE;
    LIMITATION(value, 0.0F, 200.0F);
    return value;
}



float Measure::CalculateCursorT(Chan::E ch, float posCurU, int numCur)
{
    uint8 *dataIn = CHOICE_BUFFER;

    if(!dataIn)
    {
        return 0;
    }

#define FIRST_POINT (points.sword0)
#define LAST_POINT  (points.sword1)
    
    BitSet64 points = DisplayOsci::PainterData::PointsOnDisplay();

    int prevData = 200 - dataIn[FIRST_POINT] + VALUE::MIN;

    int numIntersections = 0;

    for(int i = FIRST_POINT + 1; i < LAST_POINT; i++)
    {
        int curData = 200 - (dataIn)[i] + VALUE::MIN;

        if(curData <= posCurU && prevData > posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }

        if(curData >= posCurU && prevData < posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }
        prevData = curData;
    }
    return 0;

#undef LAST_POINT
#undef FIRST_POINT
}



void InterpolationSinX_X(uint8 *data, int numPoints, TBase::E tBase)
{
/*
     ������������������ x � sin(x)   // ���, ��������, �����������
2    1. 20�� : pi/2, -pi/2 ...
8    2. 10�� : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
18   3. 5��  : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
38   4. 2��  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
98   5. 1��  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
*/

#define MUL_SIN 1e7F
#define MUL     1e6F
#define KOEFF   (MUL / MUL_SIN)

    static const int deltas[5] = {100, 50, 20, 10, 5};
    int delta = deltas[static_cast<int>(tBase)];

    uint8 *signedData = static_cast<uint8 *>(std::malloc(static_cast<uint>(numPoints) / 2U));
    if (signedData == 0)
    {
        LOG_ERROR("��� ������");
        return;
    }
    int numSignedPoints = 0;
    
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            signedData[numSignedPoints++] = data[pos];
        }
    }

    // ����� �������� ������ �������� �����

    int shift = 0;
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            shift = pos;
            break;
        }
    }

    float deltaX = Math::PI_F;
    float stepX0 = Math::PI_F / static_cast<float>(delta);
    float x0 = Math::PI_F - stepX0;
    int num = 0;
    
    for(int i = 0; i < numPoints; i++)
    {
        x0 += stepX0;
        if((i % delta) == 0)
        {
            data[i] = signedData[i / delta];
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ? std::sinf(Math::PI_F / delta * (part + 1)) : std::sinf(Math::PI_F / delta * (part - (delta - 1) * 2));

            if (tBase > TBase::_5ns)                 // ����� ���������� ����� �������, �� ����� ������������ ���������� ������ �����
            {
                int sinXint = static_cast<int>(sinX * MUL_SIN);
                int value = 0;
                int x = static_cast<int>((x0 - deltaX) * MUL);
                int deltaXint = static_cast<int>(deltaX * MUL);

                for (int n = 0; n < numSignedPoints; n++)
                {
                    value += signedData[n] * sinXint / (x - n * deltaXint);
                    sinXint = -sinXint;
                }
                data[i] = (uint8)(value * KOEFF);
            }
            else                                    // �� ���� ��������� ���������� � ��������� ������� ��� ���������� ��������������
            {
                float value = 0.0F;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x;
                    sinX = -sinX;
                }
                data[i] = static_cast<uint8>(value);
            }
        }
    }
    
    int pos = numPoints - 1;
    while (pos > shift)
    {
        data[pos] = data[pos - shift];
        pos--;
    }

    std::free(signedData);
}



String Measure::GetStringMeasure(Chan::E ch, char* buffer, int lenBuf)
{
    TypeMeasure::E type = GetType();

    if (!set.ch[static_cast<int>(ch)].enabled)
    {
        return String("");
    }

    buffer[0] = '\0';
    std::strcpy(buffer, Chan(ch).IsA() ? "1: " : "2: ");

    if(!isSet || values[static_cast<int>(type)].value[static_cast<int>(ch)] == Float::ERROR) //-V550 //-V2550
    {
        std::strcat(buffer, "-.-");
    }
    else if(sMeas[static_cast<int>(type)].FuncCalculate)
    {
        char bufferForFunc[20];
        pFuncPCFBPC func = sMeas[static_cast<int>(type)].FucnConvertate;
        float value = values[static_cast<int>(type)].value[static_cast<int>(ch)];
       
        if ((set.ch[static_cast<int>(ch)].divider == Divider::_10) && (func == Voltage2String))
        {
            value *= 10.0F;                         // ���������, ���� ������� ��������
        }
               
        char *text = func(value, sMeas[static_cast<int>(type)].showSign, bufferForFunc);
        int len = static_cast<int>(std::strlen(text)) + static_cast<int>(std::strlen(buffer)) + 1;
        if (len + 1 <= lenBuf)
        {
            std::strcat(buffer, text);
        }
        else
        {
            LOG_ERROR("����� ������� ���");
        }
    }
    else
    {
        return String(buffer);
    }

    return String(buffer);
}


char* AutoMeasurements::Freq2String(float freq, bool, char buffer[20])
{
    std::strcpy(buffer, Frequency(freq).ToString().CString());
    return buffer;
}


char* Time2String(float time, bool always, char buffer[20])
{
    std::strcpy(buffer, Time(time).ToString(always).CString());
    return buffer;
}


char* Voltage2String(float voltage, bool always, char buffer[20])
{
    std::strcpy(buffer, Voltage(voltage).ToString(always).CString());
    return buffer;
}


char* Phase2String(float phase, bool, char buffer[20])
{
    return Phase(phase).ToString(buffer);
}


char* AutoMeasurements::Float2String(float value, bool always, char buffer[20])
{
    std::strcpy(buffer, Float(value).ToString(always, 4).CString());
    return buffer;
}


static float Divide(float val1, float val2)
{
    float result = val1 / val2;

    if(result == std::numeric_limits<float>::infinity())
    {
        result = Float::ERROR;
    }
    else if(isnan(result))
    {
        result = Float::ERROR;
    }
    else
    {
        // ����� ������
    }

    return result;
}


bool AutoMeasurements::DataIsSetting()
{
    return isSet;
}


void AutoMeasurements::SetData()
{
    isSet = (DS != nullptr);

    if (isSet)
    {
        CountedToCurrentSettings();

        BitSet64 points = DisplayOsci::PainterData::BytesOnDisplay();
        firstByte = points.sword0;
        lastByte = points.sword1;
        nBytes = lastByte - firstByte;

        if (TBASE_DS >= TBase::MIN_P2P)             // ���� ��������� � ���������� �����, �� ����� ����� ��������� ��������� ����� ��� ���������� ���������
        {
            for (int i = static_cast<int>(BYTES_IN_CHANNEL_DS - 1); i >= 0; --i)
            {
                if (IN_A[i] != VALUE::NONE)                // ���� ��� �������� �������
                {
                    lastByte = i;
                    firstByte = lastByte - nBytes;
                    if (firstByte < 0)
                    {
                        firstByte = 0;
                        lastByte = nBytes;
                    }
                    break;
                }
            }
        }
    }
}


Measure AutoMeasurements::GetActiveMeasure()
{
    int row = posActive / TableMeasures::NumCols();
    int col = posActive - row * TableMeasures::NumCols();

    return Measure(row, col);
}


static void CountedToCurrentSettings()
{
    const uint NUM_BYTES = BYTES_IN_CHANNEL_DS;

    if (ENABLED_DS_A)
    {
        Smoother::Run(IN_A, OUT_A, NUM_BYTES, set.disp.ENumSmoothing.ToNumber());
        std::memcpy(DS->dataA, OUT_A, NUM_BYTES);
        CountedToCurrentSettings(Chan::A, NUM_BYTES);
        LimitationData(Chan::A, NUM_BYTES);
    }

    if (ENABLED_DS_B)
    {
        Smoother::Run(IN_B, OUT_B, NUM_BYTES, set.disp.ENumSmoothing.ToNumber());
        std::memcpy(DS->dataB, OUT_B, NUM_BYTES);
        CountedToCurrentSettings(Chan::B, NUM_BYTES);
        LimitationData(Chan::B, NUM_BYTES);
    }
}


static void CountedToCurrentSettings(Chan::E ch, uint numBytes)
{
    CountedToCurrentRShift(ch, numBytes);
}


static void LimitationData(Chan::E ch, uint numBytes)
{
    uint8 *data = OUT(ch);

    for (uint i = 0; i < numBytes; i++)
    {
        Math::Limitation<uint8>(data, VALUE::MIN, VALUE::MAX);
        data++;
    }
}


static void CountedToCurrentRShift(Chan::E ch, uint numBytes)
{
    int16 shiftDS = RSHIFT_DS(ch);
    Range::E rangeDS = RANGE_DS(ch);

    int16 shiftSET = RShift::Get(ch);
    Range::E rangeSET = Range(ch);

    if((shiftDS == shiftSET) && (rangeDS == rangeSET))
    {
        std::memcpy(OUT(ch), IN(ch), numBytes);
    }
    else
    {
        for(uint i = 0; i < numBytes; i++)
        {
            float voltage = VALUE::ToVoltage(IN(ch)[i], rangeDS, shiftDS);
            OUT(ch)[i] = VALUE::FromVoltage(voltage, rangeSET, shiftSET);
        }
    }
}
