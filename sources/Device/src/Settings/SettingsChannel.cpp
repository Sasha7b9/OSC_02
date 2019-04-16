#include "defines.h"
#include "Settings.h"

#include "FPGA/FPGA.h"


using namespace Osci::Settings;
using namespace Osci::Settings::Memory;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ �������� �������� ��������� �� ����������.
static const struct RangeStruct
{
    pString name;     // �������� ��������� � ��������� ����, ��������� ��� ������ �� �����.
    RangeStruct(pString nRU) : name(nRU) {};
}
ranges[Range::Size][2] =
{
    {RangeStruct("2\x10��"),  RangeStruct("20\x10��")},
    {RangeStruct("5\x10��"),  RangeStruct("50\x10��")},
    {RangeStruct("10\x10��"), RangeStruct("0.1\x10�")},
    {RangeStruct("20\x10��"), RangeStruct("0.2\x10�")},
    {RangeStruct("50\x10��"), RangeStruct("0.5\x10�")},
    {RangeStruct("0.1\x10�"), RangeStruct("1\x10�")},
    {RangeStruct("0.2\x10�"), RangeStruct("2\x10�")},
    {RangeStruct("0.5\x10�"), RangeStruct("5\x10�")},
    {RangeStruct("1\x10�"),   RangeStruct("10\x10�")},
    {RangeStruct("2\x10�"),   RangeStruct("20\x10�")},
    {RangeStruct("5\x10�"),   RangeStruct("50\x10�")},
    {RangeStruct("10\x10�"),  RangeStruct("100\x10�")},
    {RangeStruct("20\x10�"),  RangeStruct("200\x10�")}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Range::ToString(int8 _divider)
{
    return ranges[value][_divider].name;
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
    static pString names[Chan::Size] =
    {
        "����� 1",
        "����� 2"
    };

    return names[value];
}
