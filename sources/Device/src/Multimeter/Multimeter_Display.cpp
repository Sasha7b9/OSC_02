#include "defines.h"
#include "Multimeter/Multimeter.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>

#include "Display/Display_Primitives.h"
#include "Hardware/Beeper.h"


using namespace Multimeter::Settings;

using Display::Primitives::Text;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// � ���� ������ ������� � ������ ����������
#define SIZE_OUT 15
char out[SIZE_OUT];

/// ���� �������� �������� ���������
/// ���� ������� ������� == 0, �� �������� ������ �� �����
static char buffer[13] = { 0 };

static void PrepareBell();
static void PrepareConstantVoltage();
static void PrepareVariableVoltage();
static void PrepareConstantCurrent();
static void PrepareVariableCurrent();
static void PrepareResistance();
static void PrepareTestDiode();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multimeter::Display::Update()
{
    static const struct Func
    {
        pFuncVV func;
        Func(pFuncVV f) : func(f) {};
    }
    funcs[Multimeter::Measure::Size] =
    {
        PrepareConstantVoltage,
        PrepareVariableVoltage,
        PrepareConstantCurrent,
        PrepareVariableCurrent,
        PrepareResistance,
        PrepareTestDiode,
        PrepareBell
    };

    Painter::BeginScene(Color::BACK);

    std::memset(out, 0, SIZE_OUT);

    Measure::E meas = Measure::GetCode(buffer);
    if (meas == Measure::Size)
    {
        meas = MULTI_MEASURE;
    }

    funcs[meas].func();

    Font::SetCurrent(Font::Type::_Big64);

    // ��������� ���������

    Text(out).Draw(30, 30, (buffer[0] == '8') ? Color::GRAY_50 : Color::FILL);

    Font::Pop();

    Color::SetCurrent(Color::FILL);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::ChangedMode()
{
    std::memset(buffer, '8', 10); //-V512
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::SetMeasure(const uint8 buf[13])
{
    std::memcpy(buffer, buf, 13);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareTestDiode()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, " V=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantVoltage() //-V524
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, " V=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableVoltage()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, " V~");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantCurrent()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, (buffer[10] == '1') ? " A=" : " mA=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableCurrent()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, (buffer[10] == '1') ? " A~" : " mA~");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PrepareResistance()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    out[7] = ' ';
    out[8] = buffer[8];
    out[9] = 0x1b;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ResistanceLess100()
{
    return ((out[1] == '0') && (out[3] == '0'));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareBell()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, " kQ");
    out[9] = 0x1b;

    if (ResistanceLess100())
    {
        Beeper::Bell::On();
    }
    else
    {
        Beeper::Bell::Off();
    }
}
