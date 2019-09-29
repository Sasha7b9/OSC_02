#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Display/Painter.h"
#include "Display/Display_Primitives.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Hardware/Beeper.h"
#include <cmath>


using namespace Display::Primitives;
using namespace Osci::Measurements;

using Osci::Measurements::Cursors::Cursor;

const float PageMeasuresCursors::PageSet::MAX_POS_U = 200.0F;
const float PageMeasuresCursors::PageSet::MAX_POS_T = 280.0F;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// ������������ ������� ��������� ��� ����� ������ ���������� ������� ���������.
void CalculateConditions(int16 pos0, int16 pos1, Cursors::Control::E cursCntrl, bool *condTopLeft, bool *condBottomRight)
{
    bool zeroLessFirst = pos0 < pos1;
    *condTopLeft = (cursCntrl == Cursors::Control::_1_2) ||                    // ���� ���������� ����� ��������� ������������
        (cursCntrl == Cursors::Control::_1 && zeroLessFirst) ||     // ��� ���������� ������ �������� � ������� ������� ������, ��� ������� �������
        (cursCntrl == Cursors::Control::_2 && !zeroLessFirst);      // ��� ���������� ������ �������� � ������� ������� ������� ������

    *condBottomRight = (cursCntrl == Cursors::Control::_1_2) ||
        (cursCntrl == Cursors::Control::_1 && !zeroLessFirst) ||
        (cursCntrl == Cursors::Control::_2 && zeroLessFirst);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Draw_ChannelA(int x, int y)
{
    String("1").Draw(x + 7, y + 5);
}

static void Draw_ChannelB(int x, int y)
{
    String("2").Draw(x + 7, y + 5);
}

void PageMeasuresCursors::PageSet::OnPress_Channel()
{
    Chan::E source = CURS_SOURCE_A ? Chan::B : Chan::A;
    SetCursSource(source);
}

void PageMeasuresCursors::PageSet::Draw_Channel(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    static const pFuncDraw func[2] = {Draw_ChannelA, Draw_ChannelB};
    func[CURS_SOURCE](x, y);
}

DEF_GRAPH_BUTTON_HINTS_2( bChannel,                                                                                                        //--- ��������� - ������� - ���������� - ����� ---
    "�����",
    "����� ������ ��� ��������� ���������",
    &PageMeasuresCursors::PageSet::self, Item::Active, PageMeasuresCursors::PageSet::OnPress_Channel, PageMeasuresCursors::PageSet::Draw_Channel,
    Draw_ChannelA, "����� 1",
    Draw_ChannelB, "����� 2"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursTime(int x, int y, bool left, bool right)
{
    x += 11;
    y += 1;

    VLine line(15);

    for (int i = 0; i < (left ? 3 : 1); i++)
    {
        line.Draw(x - 15 + i, y + 1);
    }

    for (int i = 0; i < (right ? 3 : 1); i++)
    {
        line.Draw(x + 15 - i, y + 1);
    }
}

static void Draw_T_disable(int x, int y)
{
    String("T").Draw(x + 7, y + 5);
}

static void Draw_T_disableBoth(int x, int y)
{
    DrawMenuCursTime(x, y, false, false);
}

static void Draw_T_enableLeft(int x, int y)
{
    DrawMenuCursTime(x, y, true, false);
}

static void Draw_T_enableRight(int x, int y)
{
    DrawMenuCursTime(x, y, false, true);
}

static void Draw_T_enableBoth(int x, int y)
{
    DrawMenuCursTime(x, y, true, true);
}

void PageMeasuresCursors::PageSet::OnPress_T()
{
    if (CURS_ACTIVE_T || CURsT_DISABLED)
    {
        IncCursCntrlT(CURS_SOURCE);
    }
    CURS_ACTIVE = Cursors::Active::T;
}

static void Draw_T(int x, int y)
{
    if (CURsT_DISABLED)
    {
        Draw_T_disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_T)
        {
            Draw_T_disableBoth(x, y);
        }
        else
        {
            bool condLeft = false, condDown = false;
            Chan::E source = CURS_SOURCE;
            CalculateConditions((int16)CURsT_POS(source, 0), (int16)CURsT_POS(source, 1), CURsT_CNTRL, &condLeft, &condDown);
            if (condLeft && condDown)
            {
                Draw_T_enableBoth(x, y);
            }
            else if (condLeft)
            {
                Draw_T_enableLeft(x, y);
            }
            else
            {
                Draw_T_enableRight(x, y);
            }
        }
    }
}

DEF_GRAPH_BUTTON_HINTS_5( bT,                                                                                                          //--- ��������� - ������� - ���������� - ������� � ---
    "������� T",
    "����� �������� ������� ��� ��������� � ����������",
    &PageMeasuresCursors::PageSet::self, Item::Active, PageMeasuresCursors::PageSet::OnPress_T, Draw_T,
    Draw_T_disable,     "������� ������� ���������",
    Draw_T_disableBoth, "������� ������� ��������",
    Draw_T_enableLeft,  "������� ������� ��������, ���������� ����� ��������",
    Draw_T_enableRight, "������� ������� ��������, ���������� ������ ��������",
    Draw_T_enableBoth,  "������� ������� ��������, ���������� ������ ���������"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursVoltage(int x, int y, bool top, bool bottom)
{
    x -= 10;
    y += 6;

    HLine line(40);

    for (int i = 0; i < (top ? 3 : 1); i++)
    {
        line.Draw(x, y + i);
    }

    for (int i = 0; i < (bottom ? 3 : 1); i++)
    {
        line.Draw(x, y + 8 - i);
    }
}

static void Draw_U_disable(int x, int y)
{
    String('U').Draw(x + 7, y + 5);
}

static void Draw_U_disableBoth(int x, int y)
{
    DrawMenuCursVoltage(x, y, false, false);
}

static void Draw_U_enableUpper(int x, int y)
{
    DrawMenuCursVoltage(x, y, true, false);
}

static void Draw_U_enableLower(int x, int y)
{
    DrawMenuCursVoltage(x, y, false, true);
}

static void Draw_U_enableBoth(int x, int y)
{
    DrawMenuCursVoltage(x, y, true, true);
}

void PageMeasuresCursors::PageSet::OnPress_U()
{
    if (CURS_ACTIVE_U || CURsU_DISABLED)
    {
        IncCursCntrlU(CURS_SOURCE);
    }
    CURS_ACTIVE = Cursors::Active::U;
}

static void Draw_U(int x, int y)
{
    Chan::E source = CURS_SOURCE;
    if (CURsU_DISABLED)
    {
        Draw_U_disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_U)
        {
            Draw_U_disableBoth(x, y);
        }
        else
        {
            bool condTop = false, condDown = false;
            CalculateConditions((int16)Cursor::PosU(source, 0), (int16)Cursor::PosU(source, 1), CURsU_CNTRL, &condTop, &condDown);
            if (condTop && condDown)
            {
                Draw_U_enableBoth(x, y);
            }
            else if (condTop)
            {
                Draw_U_enableUpper(x, y);
            }
            else
            {
                Draw_U_enableLower(x, y);
            }
        }
    }
}

// ����� ������� ���������� - ������ 1, ������ 2, ��� ������� ��� ���������.
DEF_GRAPH_BUTTON_HINTS_5( bU,                                                                                                          //--- ��������� - ������� - ���������� - ������� U ---
    "������� U",
    "����� �������� ���������� ��� ��������� � ����������",
    &PageMeasuresCursors::PageSet::self, Item::Active, PageMeasuresCursors::PageSet::OnPress_U, Draw_U,
    Draw_U_disable,     "������� ���������� ���������",
    Draw_U_disableBoth, "������� ���������� ��������",
    Draw_U_enableUpper, "������� ���������� ��������, ���������� ������� ��������",
    Draw_U_enableLower, "������� ���������� ��������, ���������� ������ ��������",
    Draw_U_enableBoth,  "������� ���������� ��������, ���������� ������ ���������"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_100()
{
    PageMeasuresCursors::PageSet::SetCursPos100(CURS_SOURCE);
}

static void Draw_100(int x, int y)
{
    String("100%%").Draw(x + 1, y + 5);
}

// ��������� 100 ��������� � ������� ����� ��������.
DEF_GRAPH_BUTTON( b100,                                                                                                                     //--- ��������� - ������� - ���������� - 100% ---
    "100\x83",
    "������������ ��� ���������� ���������. ������� �������� ���������� ����� ��������� ��������� ��� 100%",
    &PageMeasuresCursors::PageSet::self, Item::Active, OnPress_100, Draw_100
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Movement_Percents(int x, int y)
{
    String('\x83').Draw(x + 6, y + 5);
}

static void Draw_Movement_Points(int x, int y)
{
    String("���").Draw(x + 2, y + 5);
}

static void OnPress_Movement()
{
    Math::CircleIncrease<int8>((int8 *)&CURS_MOVEMENT, 0, 1);
}

static void Draw_Movement(int x, int y)
{
    if (CURS_MOVEMENT_IN_PERCENTS)
    {
        Draw_Movement_Percents(x, y);
    }
    else
    {
        Draw_Movement_Points(x, y);
    }
}

DEF_GRAPH_BUTTON_HINTS_2( bMovement,                                                                                                 //--- ��������� - ������� - ���������� - ����������� ---
    "�����������",
    "����� ���� ����������� �������� - �������� ��� �����",
    &PageMeasuresCursors::PageSet::self, Item::Active, OnPress_Movement, Draw_Movement,
    Draw_Movement_Percents, "��� ����������� �������� ������ ������ ��������",
    Draw_Movement_Points,   "��� ����������� ������� ������ ������ �������"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PageMeasuresCursors::PageSet::OnArrows(const KeyEvent &event) //-V2506
{
    TypePress::E type = event.type;

    if (type == TypePress::Release || type == TypePress::Long)
    {
        return true;
    }

    Key::E key = event.key;

    float value = event.IsAboveZero() ? 1.0F : -1.0F;

    if (CURS_ACTIVE_U && (key == Key::Up || key == Key::Down))
    {
        if (CURS_MOVEMENT_IN_PERCENTS)
        {
            value *= dUperc(CURS_SOURCE) / 100.0F;
        }

        if (CURsU_CNTRL_1 || CURsU_CNTRL_1_2)
        {
            SetShiftCursPosU(CURS_SOURCE, 0, value);
        }
        if (CURsU_CNTRL_2 || CURsU_CNTRL_1_2)
        {
            SetShiftCursPosU(CURS_SOURCE, 1, value);
        }
        UpdateCursorsForLook();
    }
    else if(CURS_ACTIVE_T && (key == Key::Left || key == Key::Right))
    {
        if (CURS_MOVEMENT_IN_PERCENTS)
        {
            value *= dTperc(CURS_SOURCE) / 100.0F;
        }

        if (CURsT_CNTRL_1 || CURsT_CNTRL_1_2)
        {
            SetShiftCursPosT(CURS_SOURCE, 0, value);
        }
        if (CURsT_CNTRL_2 || CURsT_CNTRL_1_2)
        {
            SetShiftCursPosT(CURS_SOURCE, 1, value);
        }
        UpdateCursorsForLook();
    }
    else
    {
        // ������ �� ������
    }

    Beeper::RegulatorShiftRotate();

    return true;
}

static bool IsActive_Set()
{
    return CURS_SHOW;
}

DEF_PAGE_5( pSet, // -V641 // -V1027                                                                                                              //--- ��������� - ������� - ���������� ---
    "����������",
    "������� � ����� ��������� ���������",
    &bChannel,          ///< ��������� - ������� - ���������� - �����
    &bU,                ///< ��������� - ������� - ���������� - ������� U
    &bT,                ///< ��������� - ������� - ���������� - ������� �
    &b100,              ///< ��������� - ������� - ���������� - 100%
    &bMovement,         ///< ��������� - ������� - ���������� - �����������
    PageName::Measures_Cursors_Set, &PageMeasuresCursors::self, IsActive_Set, Page::Changed, Page::BeforeDraw, PageMeasuresCursors::PageSet::OnArrows
)

const Page * const PageMeasuresCursors::PageSet::self = (const Page *)&pSet;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::SetCursSource(Chan::E ch)
{
    CURS_SOURCE = ch;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::IncCursCntrlU(Chan::E ch)
{
    Math::CircleIncrease<int8>((int8 *)&CURsU_CNTRL_CH(ch), 0, 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::IncCursCntrlT(Chan::E ch)
{
    Math::CircleIncrease<int8>((int8 *)&CURsT_CNTRL_CH(ch), 0, 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::SetCursPos100(Chan::E ch)
{
    dUperc(ch) = (float)std::fabsf(CURsU_POS(ch, 0) - CURsU_POS(ch, 1));
    dTperc(ch) = (float)std::fabsf(CURsT_POS(ch, 0) - CURsT_POS(ch, 1));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::SetShiftCursPosU(Chan::E ch, int numCur, float delta)
{
    CURsU_POS(ch, numCur) = Math::LimitationRet(CURsU_POS(ch, numCur) - delta, 0.0F, MAX_POS_U);

    if (CURS_MOVEMENT_IN_PIXELS)                        // ���� ����������� �� ��������, �� ����� �������� � ���������� ����� ������
    {
        /// \todo
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::SetShiftCursPosT(Chan::E ch, int numCur, float delta)
{
    /// \todo ���������� �����
    // CURsT_POS(ch, numCur) = LimitationFloat(CURsT_POS(ch, numCur) + delta, 0, MAX_POS_T);   
    Cursor::SetCursPosT_temp(ch, numCur, Math::LimitationRet(CURsT_POS(ch, numCur) + delta, 0.0F, MAX_POS_T));

    if (CURS_MOVEMENT_IN_PIXELS)                        // ���� ����������� �� ��������, �� ����� �������� � ���������� ����� ������
    {
        /// \todo
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::UpdateCursorsForLook()
{
    Chan::E source = CURS_SOURCE;

    if (CURS_ACTIVE_T && (CURS_LOOK_U(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        SetCursorU(source, 0, Measure::CalculateCursorU(source, CURsT_POS(source, 0)));
    }
    if (CURS_ACTIVE_T && (CURS_LOOK_U(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        SetCursorU(source, 1, Measure::CalculateCursorU(source, CURsT_POS(source, 1)));
    }
    if (CURS_ACTIVE_U && (CURS_LOOK_T(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        SetCursorT(source, 0, Measure::CalculateCursorT(source, CURsU_POS(source, 0), 0));
    }
    if (CURS_ACTIVE_U && (CURS_LOOK_T(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        SetCursorT(source, 1, Measure::CalculateCursorT(source, CURsU_POS(source, 1), 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::SetCursorU(Chan::E ch, int numCur, float pos)
{
    CURsU_POS(ch, numCur) = Math::LimitationRet(pos, 0.0F, MAX_POS_U);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasuresCursors::PageSet::SetCursorT(Chan::E ch, int numCur, float pos)
{
    /// \todo ���������� �����
    // CURsT_POS(ch, numCur) = LimitationFloat(pos, 0, MAX_POS_T);      
    Cursor::SetCursPosT_temp(ch, numCur, Math::LimitationRet(pos, 0.0F, MAX_POS_T));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PageMeasuresCursors::PageSet::IsRegSetActiveOnCursors()
{
    return ((Menu::GetNameOpenedPage() == PageName::Measures_Cursors_Set) &&
        ((CURS_ACTIVE_U && CURsU_ENABLED) || (CURS_ACTIVE_T && CURsT_ENABLED)));
}
