#include "defines.h"
#include "Settings.h"


using FPGA::SET::Range;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ �������� �������� ��������� �� ����������.
static const struct RangeStruct
{
    pString nameRU;     // �������� ��������� � ��������� ����, ��������� ��� ������ �� �����.
    pString nameEN;
    RangeStruct(pString nRU, pString nEN) : nameRU(nRU), nameEN(nEN) {};
}
ranges[Range::Number][2] =
{
    {RangeStruct("2\x10��",  "20\x10��"), RangeStruct("2\x10�V",  "20\x10�V")},
    {RangeStruct("5\x10��",  "50\x10��"), RangeStruct("5\x10mV",  "50\x10mV")},
    {RangeStruct("10\x10��", "0.1\x10�"), RangeStruct("10\x10mV", "0.1\x10V")},
    {RangeStruct("20\x10��", "0.2\x10�"), RangeStruct("20\x10mV", "0.2\x10V")},
    {RangeStruct("50\x10��", "0.5\x10�"), RangeStruct("50\x10mV", "0.5\x10V")},
    {RangeStruct("0.1\x10�", "1\x10�"),   RangeStruct("0.1\x10V", "1\x10V")},
    {RangeStruct("0.2\x10�", "2\x10�"),   RangeStruct("0.2\x10V", "2\x10V")},
    {RangeStruct("0.5\x10�", "5\x10�"),   RangeStruct("0.5\x10V", "5\x10V")},
    {RangeStruct("1\x10�",   "10\x10�"),  RangeStruct("1\x10V",   "10\x10V")},
    {RangeStruct("2\x10�",   "20\x10�"),  RangeStruct("2\x10V",   "20\x10V")},
    {RangeStruct("5\x10�",   "50\x10�"),  RangeStruct("5\x10V",   "50\x10V")},
    {RangeStruct("10\x10�",  "100\x10�"), RangeStruct("10\x10V",  "100\x10V")},
    {RangeStruct("20\x10�",  "200\x10�"), RangeStruct("20\x10V",  "200\x10V")}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Range::ToString(Divider::E divider)
{
    return (LANG == Language::RU) ? ranges[value][divider].nameRU : ranges[value][divider].nameEN;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Chan::PointsInChannel() const
{
    DEF_STRUCT(StructNumPoints, int) numPoints[ENumPointsFPGA::Size] =
    {
        512,
        1024,
        2048,
        4096,
        8192
    };

    return numPoints[(int)FPGA_ENUM_POINTS].val;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Chan::RequestBytes(DataSettings *) const
{
    return FPGA_MAX_NUM_POINTS;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString ModeCouple::UGO() const
{
    static pString couple[] = { "\x92", "\x91", "\x90" };
    return couple[value];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Chan::Name() const
{
    static pString names[Chan::Number][2] =
    {
        "����� 1", "Channel 1",
        "����� 2", "Channel 2"
    };

    return names[value][LANG];
}
