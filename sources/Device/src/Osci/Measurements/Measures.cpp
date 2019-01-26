#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Utils/Math.h"

#include "Osci/Measurements/Measurements.h"
#include "Osci/Measurements/Measurements_Graphics.h"


using namespace Display::Primitives;
using namespace Osci::Measurements;
using namespace Osci::Measurements::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StructMeasure
{
    const char *name;
    const char UGO;
    uint8      notUsed0;
    uint8      notUsed1;
    uint8      notUsed2;
};

#define DEF_STRUCT_MEASURE(name, ugo) {name, ugo, 0, 0, 0}

static const StructMeasure sMeas[Measure::Type::Number] =
{
    DEF_STRUCT_MEASURE("",            '\x00'),
    DEF_STRUCT_MEASURE("U ����",      '\x20'),
    DEF_STRUCT_MEASURE("U ���",       '\x25'),
    DEF_STRUCT_MEASURE("U ���",       '\x2a'),
    DEF_STRUCT_MEASURE("U ���� ���",  '\x40'),
    DEF_STRUCT_MEASURE("U ��� ���",   '\x45'),
    DEF_STRUCT_MEASURE("U ����",      '\x4a'),
    DEF_STRUCT_MEASURE("U ��",        '\x60'),
    DEF_STRUCT_MEASURE("U ���",       '\x65'),
    DEF_STRUCT_MEASURE("������+",     '\x6a'),
    DEF_STRUCT_MEASURE("������-",     '\x80'),
    DEF_STRUCT_MEASURE("������",      '\x85'),
    DEF_STRUCT_MEASURE("�������",     '\x8a'),
    DEF_STRUCT_MEASURE("�� ������",   '\xa0'),
    DEF_STRUCT_MEASURE("�� �����",    '\xa5'),
    DEF_STRUCT_MEASURE("����+",       '\xaa'),
    DEF_STRUCT_MEASURE("����-",       '\xc0'),
    DEF_STRUCT_MEASURE("������+",     '\xc5'),
    DEF_STRUCT_MEASURE("������-",     '\xca'),
    DEF_STRUCT_MEASURE("��������\xa7",'\xe0'),
    DEF_STRUCT_MEASURE("��������\xa6",'\xe5'),
    DEF_STRUCT_MEASURE("����\xa7",    '\xe0'),
    DEF_STRUCT_MEASURE("����\xa6",    '\xe5')
};

int8 Measure::posActive = 0;
bool Measure::pageChoiceIsActive = false;
int8 Measure::posOnPageChoice = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Measure::IsActive()
{
    if(posActive >= Graphics::NumCols() * Graphics::NumRows())
    {
        posActive = 0;
    }
    return (row * Graphics::NumCols() + col) == posActive;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Measure Measure::GetActive()
{
    int row = posActive / Graphics::NumCols();
    int col = posActive - row * Graphics::NumCols();

    return Measure(row, col);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetActive(int row, int col)
{
    posActive = (int8)(row * Graphics::NumCols() + col);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Measure::GetChar(Measure::Type::E measure)
{
    return sMeas[measure].UGO;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::ChangeActive(int delta)
{
    Measure measure = GetActive();

    int row = measure.row;
    int col = measure.col;

    col += Math::Sign(delta);

    if (col < 0)
    {
        col = Graphics::NumCols() - 1;
        row--;
        if (row < 0)
        {
            row = Graphics::NumRows() - 1;
        }
    }
    else if (col == Graphics::NumCols())
    {
        col = 0;
        row++;
        if (row >= Graphics::NumRows())
        {
            row = 0;
        }
    }
    else
    {
        // ����� ������
    }

    SetActive(row, col);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Measure::Name()
{
    return String(sMeas[GetType()].name);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Measure::Type::E Measure::GetType()
{
    return set.meas_measures[row * Graphics::NumCols() + col];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::ShortPressOnSmallButonMarker()
{
    if(set.meas_measures[posActive] == MEAS_MARKED)
    {
        MEAS_MARKED = Measure::Type::None;
    }
    else
    {
        MEAS_MARKED = set.meas_measures[posActive];
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetMarkerVoltage(Chan::E ch, int num, float value)
{
    markerVoltage[ch][num] = (int)value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetMarkerTime(Chan::E ch, int num, int value)
{
    markerTime[ch][num] = value;
}
