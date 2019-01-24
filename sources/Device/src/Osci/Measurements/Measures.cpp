#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Hardware/Sound.h"
#include "Utils/Math.h"

#include "Osci/Measurements/Processing.h"
#include "Osci/Measurements/Measurements_Graphics.h"


using namespace Display::Primitives;
using namespace Osci::Processing;
using namespace Osci::Measurements;
using namespace Osci::Measurements::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������� ���������� y ��������� ������� �������������� ���������
static int top = 0;

static int GetTopTable();
/// ���������� �������� ������ ���������
static void DrawPageChoice();


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
int Osci::Measurements::Graphics::DY()
{
    if(VIEW_MEASURES_BOTH)
    {
        return 30;
    }
    return 21;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::DX()
{
    return Grid::Width() / 5; 
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
static int GetTopTable()
{
    if(NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2)
    {
        return Grid::Bottom() - Osci::Measurements::Graphics::DY() * 6;
    }

    int y = Grid::Bottom() - Graphics::NumRows() * Osci::Measurements::Graphics::DY();

    if(Menu::IsShown())
    {
        y -= 3;
    }

    return y;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::NumCols()
{
    const int cols[] = {1, 2, 5, 5, 5, 1, 2};
    return cols[NUM_MEASURES];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::NumRows()
{
    int rows[] = {1, 1, 1, 2, 3, 6, 6};
    return rows[NUM_MEASURES];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::GetDeltaGridLeft()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(NUM_MEASURES_IS_6_1)
        {
            return Graphics::DX();
        }
        else if(NUM_MEASURES_IS_6_2)
        {
            return DX() * 2;
        }
        else
        {
            // ������ ��������� ���
        }
    }
    return 0;
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
static void DrawPageChoice()
{
    if(!Measure::pageChoiceIsActive)
    {
        return;
    }
    int x = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? (Grid::Right() - 3 * Grid::Width() / 5) : Grid::Left();
    int y = Grid::Top();
    int dX = Grid::Width() / 5;
    int dY = 22;
    int maxRow = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? 8 : 5;
    int maxCol = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? 3 : 5;
    Measure::Type::E meas = Measure::Type::None;
    Font::SetCurrent(Font::Type::_UGO);
    for(int row = 0; row < maxRow; row++)
    {
        for(int col = 0; col < maxCol; col++)
        {
            if(meas >= Measure::Type::Number)
            {
                break;
            }
            int x0 = x + col * dX;
            int y0 = y + row * dY;
            bool active = (meas == Measure::posOnPageChoice);
            Rectangle(dX, dY).Draw(x0, y0, Color::WHITE);
            Region(dX - 2, dY - 2).Fill(x0 + 1, y0 + 1, (active ? Color::FLASH_10 : Color::BACK));
            Color::SetCurrent(active ? Color::FLASH_01 : Color::FILL);
            Char(Measure::GetChar(meas)).Draw10SymbolsInRect(x0 + 2, y0 + 1);
            Font::SetCurrent(Font::Type::_5);
            Text(sMeas[meas].name).DrawRelativelyRight(x0 + dX, y0 + 12, active ? Color::FLASH_01 : Color::FILL);
            Font::SetCurrent(Font::Type::_UGO);
            meas = (Measure::Type::E)((int)meas + 1);    // meas++;
        }
    }
    Font::SetCurrent(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::Draw()
{
    if (!SHOW_MEASURES)
    {
        return;
    }

    top = Grid::Bottom();

    Processing::CalculateMeasures();

    int x0 = Grid::Left() - GetDeltaGridLeft();
    int dX = DX();
    int dY = DY();
    int y0 = GetTopTable();

    int numRows = NumRows();
    int numCols = NumCols();

    for (int str = 0; str < numRows; str++)
    {
        for (int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            Measure measure = Measure::Get(str, elem);

            bool active = measure.IsActive() && Menu::GetNameOpenedPage() == Page::Name::Measures_Auto_Tune;

            Measure::Type::E type = measure.GetType();
            if (type != Measure::Type::None)
            {
                Region(dX, dY).Fill(x, y, Color::BACK);
                Rectangle(dX, dY).Draw(x, y, Color::FILL);
                top = Math::Min(top, y);
            }
            if (active)
            {
                Region(dX - 4, dY - 4).Fill(x + 2, y + 2, Color::FILL);
            }
            if (type != Measure::Type::None)
            {
                Color color = active ? Color::BACK : Color::FILL;

#define SIZE_BUFFER 20
                char buffer[SIZE_BUFFER];

                measure.Name().Draw(x + 4, y + 2, color);

                if (type == MEAS_MARKED)
                {
                    Region(dX - 2, 9).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
                    measure.Name().Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
                }
                if(VIEW_MEASURES_BOTH)
                {
                    Processing::GetStringMeasure(type, Chan::A, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::A));
                    Processing::GetStringMeasure(type, Chan::B, buffer, SIZE_BUFFER).Draw(x + 2, y + (SET_ENABLED_A ? 20 : 11), Color::Channel(Chan::B));
                }
                else if (VIEW_MEASURES_A)
                {
                    Processing::GetStringMeasure(type, Chan::A, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::A));
                }
                else if (VIEW_MEASURES_B)
                {
                    Processing::GetStringMeasure(type, Chan::B, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::B));
                }
                else
                {
                    // ������ ��������� ���
                }
            }
        }
    }

    if (Menu::GetNameOpenedPage() == Page::Name::Measures_Auto_Tune)
    {
        DrawPageChoice();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetMarkerVoltage(Chan::E ch, int num, float value)
{
    Processing::markerVoltage[ch][num] = (int)value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetMarkerTime(Chan::E ch, int num, int value)
{
    Processing::markerTime[ch][num] = value;
}
