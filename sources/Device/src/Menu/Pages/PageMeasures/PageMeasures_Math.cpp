#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Osci/Osci_Settings.h"
#include "FPGA/FPGA.h"
#include "Hardware/Beeper.h"
#include "FPGA/FPGA_Math.h"


using namespace Display::Primitives;
using namespace Osci::Settings;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Screen()
{
    if (set.fft.enabled)
    {
        Display::ShowWarning(Warning::ImpossibleEnableMathFunction);
    }
    else
    {
        Math::CircleIncrease<int8>((int8 *)&set.math.modeDraw, 0, 2);
    }
}

static void Draw_Screen_Disable(int x, int y)
{
    String("��x").Draw(x + 2, y + 5);
}

static void Draw_Screen_Separate(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
    HLine line(13);
    line.Draw(x + 3, y + 9);
    line.Draw(x + 3, y + 10);
}

static void Draw_Screen_Together(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
}

static void Draw_Screen(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    const pFuncDraw funcs[3] =
    {
        Draw_Screen_Disable,
        Draw_Screen_Separate,
        Draw_Screen_Together
    };
    funcs[set.math.modeDraw](x, y);
}

DEF_GRAPH_BUTTON_HINTS_3( bScreen,                                                                                                                         //--- ������ - ������� - ����� ---
    "�����",
    "�������� ����� ����������� ��������������� �������",
    &PageMeasuresMath::self, Item::Active, OnPress_Screen, Draw_Screen,
    Draw_Screen_Disable, "����� �������������� ������� ��������",
    Draw_Screen_Separate, "������� � �������������� ������� ��������� � ������ �����",
    Draw_Screen_Together, "������� � �������������� ������� ��������� � ����� ����"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Type()
{
    Math::CircleIncrease<int8>((int8 *)&set.math.function, 0, 1);
}

static void Draw_Type_Sum(int x, int y)
{
    HLine(10).Draw(x + 4, y + 9);
    VLine(10).Draw(x + 9, y + 4);
}

static void Draw_Type_Mul(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_MATH_FUNC_MUL).Draw4SymbolsInRect(x + 4, y + 3);
    Font::SetCurrent(Font::Type::_8);
}

static void Draw_Type(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    const pFuncDraw funcs[2] = { Draw_Type_Sum, Draw_Type_Mul };
    funcs[set.math.function](x, y);
}

static bool IsActive_Type()
{
    return (set.math.modeDraw != FuncModeDraw::Disable);
}

DEF_GRAPH_BUTTON_HINTS_2( bType,                                                                                                                             //--- ������ - ������� - ��� ---
    "���",
    "����� �������������� �������",
    &PageMeasuresMath::self, IsActive_Type, OnPress_Type, Draw_Type,
    Draw_Type_Sum, "��������",
    Draw_Type_Mul, "���������"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeArrows()
{
    Math::CircleIncrease<int8>((int8 *)&set.math.modeRegSet, 0, 1);
}

static void Draw_ModeArrows_Range(int x, int y)
{
    Char('M').Draw(x + 7, y + 5);
}

static void Draw_ModeArrows_RShift(int x, int y)
{
    String("��").Draw(x + 5, y + 5);
}

static void Draw_ModeArrows(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    static const pFuncDraw funcs[2] = { Draw_ModeArrows_Range, Draw_ModeArrows_RShift };
    funcs[set.math.modeRegSet](x, y);
}

static bool IsActive_ModeArrows()
{
    return (set.math.modeDraw != FuncModeDraw::Disable);
}

DEF_GRAPH_BUTTON_HINTS_2( bModeArrows,                                                                                                     //--- ������ - ������� - ����� ����� ��������� ---
    "����� ����� ���������",
    "����� ������ ����� ��������� - ���������� ��������� ��� ���������",
    &PageMeasuresMath::self, IsActive_ModeArrows, OnPress_ModeArrows, Draw_ModeArrows,
    Draw_ModeArrows_Range, "���������� ���������",
    Draw_ModeArrows_RShift, "���������� ���������"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_RangeA()
{
    set.math.range = set.ch[Chan::A].range;
    set.math.divider = (int8)set.ch[Chan::A].divider;
}

static void Draw_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}

static bool IsActive_RangeA()
{
    return (set.math.modeDraw != FuncModeDraw::Disable);
}

DEF_GRAPH_BUTTON( bRangeA,                                                                                                                   //--- ������ - ������� - ������� 1-�� ������ ---
    "������� 1-�� ������",
    "���������� ������� ������� ������ ��� ����������� ����������",
    &PageMeasuresMath::self, IsActive_RangeA, OnPress_RangeA, Draw_RangeA
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_RangeB()
{
    set.math.range = set.ch[Chan::B].range;
    set.math.divider = (int8)set.ch[Chan::B].divider;
}

static void Draw_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}

static bool IsActive_RangeB()
{
    return (set.math.modeDraw != FuncModeDraw::Disable);
}

DEF_GRAPH_BUTTON( bRangeB,                                                                                                                   //--- ������ - ������� - ������� 2-�� ������ ---
    "������� 2-�� ������",
    "���������� ������� ������� ������ ��� ����������� ����������",
    &PageMeasuresMath::self, IsActive_RangeB, OnPress_RangeB, Draw_RangeB
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Math()
{
    return !set.fft.enabled;
}

static void OnOpenClose_Math(bool)
{
    if (set.fft.enabled)
    {
        Display::ShowWarning(Warning::ImpossibleEnableMathFunction);
    }
}

static bool OnArrows_Function(const KeyEvent &event) // -V2506
{
    if (set.math.modeDraw == FuncModeDraw::Disable)
    {
        return false;
    }

    Key::E key = event.key;

    int delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    if (set.math.modeRegSet == ModeRegSet::RShift)
    {
        uint16 prevRShift = set.math.rShift;
        uint16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShift::MAX)
            {
                rShift += 4 * STEP_RSHIFT;  // -V112
                LIMIT_ABOVE(rShift, RShift::MAX);
                if (prevRShift < RShift::ZERO && rShift > RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Beeper::RegulatorShiftRotate();
                set.math.rShift = rShift;
            }
        }
        else
        {
            if (rShift > RShift::MIN)
            {
                rShift -= 4 * STEP_RSHIFT; // -V112
                LIMIT_BELOW(rShift, RShift::MIN);
                if (prevRShift > RShift::ZERO && rShift < RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Beeper::RegulatorShiftRotate();
                set.math.rShift = rShift;
            }
        }
    }
    if (set.math.modeRegSet == ModeRegSet::Range)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = FPGA::Math::RShift2Abs(set.math.rShift, set.math.range);

        if (sum > 2)
        {
            if (set.math.range < Range::Size - 1)
            {
                set.math.range = (Range::E)((uint8)(set.math.range + 1));  // SET_RANGE_MATH++;
                set.math.rShift = (uint16)FPGA::Math::RShift2Rel(rShiftAbs, set.math.range);
                Beeper::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (set.math.range > 0)
            {
                set.math.range = (Range::E)((uint8)(set.math.range - 1));  // SET_RANGE_MATH--;
                set.math.rShift = (uint16)FPGA::Math::RShift2Rel(rShiftAbs, set.math.range);
                Beeper::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else
        {
            // ������ ������ �� �����
        }
    }

    return true;
}

DEF_PAGE_5( pMath, // -V641                                                                                                                                        //--- ������ - ������� ---
    "����������",
    "��������� � ����� �������������� ������� - �������� ��� ���������",
    &bScreen,
    &bType,
    &bModeArrows,
    &bRangeA,
    &bRangeB,
    PageName::Measures_Math, &PageMeasures::self, IsActive_Math, OnOpenClose_Math, Page::BeforeDraw, OnArrows_Function
)

const Page * const PageMeasuresMath::self = (const Page *)&pMath;
