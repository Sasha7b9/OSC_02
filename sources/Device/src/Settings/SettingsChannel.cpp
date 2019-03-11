#include "defines.h"
#include "Settings.h"

#include "FPGA/FPGA.h"


using namespace Osci::Settings;
using namespace Osci::Settings::Memory;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ �������� �������� ��������� �� ����������.
static const struct RangeStruct
{
    pString nameRU;     // �������� ��������� � ��������� ����, ��������� ��� ������ �� �����.
    pString nameEN;
    RangeStruct(pString nRU, pString nEN) : nameRU(nRU), nameEN(nEN) {};
}
ranges[Range::Size][2] =
{
    {RangeStruct("2\x10��",  "2\x10�V"),  RangeStruct("20\x10��", "20\x10�V")},
    {RangeStruct("5\x10��",  "5\x10mV"),  RangeStruct("50\x10��", "50\x10mV")},
    {RangeStruct("10\x10��", "10\x10mV"), RangeStruct("0.1\x10�", "0.1\x10V")},
    {RangeStruct("20\x10��", "20\x10mV"), RangeStruct("0.2\x10�", "0.2\x10V")},
    {RangeStruct("50\x10��", "50\x10mV"), RangeStruct("0.5\x10�", "0.5\x10V")},
    {RangeStruct("0.1\x10�", "0.1\x10V"), RangeStruct("1\x10�",   "1\x10V")},
    {RangeStruct("0.2\x10�", "0.2\x10V"), RangeStruct("2\x10�",   "2\x10V")},
    {RangeStruct("0.5\x10�", "0.5\x10V"), RangeStruct("5\x10�",   "5\x10V")},
    {RangeStruct("1\x10�",   "1\x10V"),   RangeStruct("10\x10�",  "10\x10V")},
    {RangeStruct("2\x10�",   "2\x10V"),   RangeStruct("20\x10�",  "20\x10V")},
    {RangeStruct("5\x10�",   "5\x10V"),   RangeStruct("50\x10�",  "50\x10V")},
    {RangeStruct("10\x10�",  "10\x10V"),  RangeStruct("100\x10�", "100\x10V")},
    {RangeStruct("20\x10�",  "20\x10V"),  RangeStruct("200\x10�", "200\x10V")}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Range::ToString(int8 _divider)
{
    return (LANG_RU) ? ranges[value][_divider].nameRU : ranges[value][_divider].nameEN;
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
    return FPGA::MAX_NUM_POINTS;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Chan::Name() const
{
    static pString names[Chan::Size][2] =
    {
        "����� 1", "Channel 1",
        "����� 2", "Channel 2"
    };

    return names[value][LANG];
}
