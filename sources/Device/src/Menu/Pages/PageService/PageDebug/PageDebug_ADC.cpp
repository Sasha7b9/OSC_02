#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Settings/SettingsNRST.h"


void ExtraShift::SetTypeReal()
{
    setNRST.exShift.type = ExtraShift::Real;
}


void ExtraShift::SetTypeDisabled()
{
    setNRST.exShift.type = ExtraShift::Disabled;
}


void ExtraShift::SetValue(Chan::E ch, Range::E range, int8 value)
{
    setNRST.exShift.value[ch][range] = value;
}


int8 ExtraShift::GetValue(Chan::E ch, Range::E range)
{
    return setNRST.exShift.value[ch][range];
}


void ExtraStretch::SetTypeReal()
{
    setNRST.exStretch.type = ExtraStretch::Real;
}


void ExtraStretch::SetTypeDisabled()
{
    setNRST.exStretch.type = ExtraStretch::Disabled;
}


bool ExtraStretch::TypeIsDisabled()
{
    return (setNRST.exStretch.type == ExtraStretch::Disabled);
}


float ExtraStretch::GetValue(Chan::E ch)
{
    return setNRST.exStretch.value[ch];
}


void ExtraStretch::SetValue(Chan::E ch, float value)
{
    setNRST.exStretch.value[ch] = value;
}


bool ExtraStretch::TypeIsHand()
{
    return (setNRST.exStretch.type == ExtraStretch::Hand);
}


static int16 shiftADCA;
static int16 shiftADCB;


static void Draw_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, setNRST.balanceShiftADC[Chan::A], static_cast<int8>(BalanceADC::Value(Chan::A))},
        {0, setNRST.balanceShiftADC[Chan::B], static_cast<int8>(BalanceADC::Value(Chan::B))}
    };

    shiftADCA = shift[Chan::A][BalanceADC::Ref().value];
    shiftADCB = shift[Chan::B][BalanceADC::Ref().value];
}

static void OnChanged_Balance_Mode(bool)
{
    Draw_Balance_Mode(0, 0);
}

DEF_CHOICE_3(cBalance_Mode,                                                                                                                          //--- ������� - ��� - ������ - ����� ---
    "�����",
    "",
    DISABLE_RU,
    "��������",
    "������",
    BalanceADC::Ref(), &PageDebug::PageADC::PageBalance::self, Item::Active, OnChanged_Balance_Mode, Draw_Balance_Mode
)


static bool IsActive_ShiftAB()
{
    return BalanceADC::IsHand();
}

static void OnChanged_ShiftA()
{
    BalanceADC::Value(Chan::A) = shiftADCA;
}

DEF_GOVERNOR(gShiftA,                                                                                                                           //--- ������� - ��� - ������ - �������� 1 ---
    "�������� 1",
    "",
    shiftADCA, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftA
)


static void OnChanged_ShiftB()
{
    BalanceADC::Value(Chan::B) = shiftADCB;
}

DEF_GOVERNOR(gShiftB,                                                                                                                           //--- ������� - ��� - ������ - �������� 2 ---
    "�������� 2",
    "",
    shiftADCB, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftB
)


DEF_PAGE_3(pBalance,                                                                                                                                         //--- ������� - ��� - ������ ---
    "������",
    "",
    &cBalance_Mode,
    &gShiftA,
    &gShiftB,
    PageName::Debug_ADC_Balance,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


DEF_CHOICE_2(cStretch_Mode,                                                                                                                        //--- ������� - ��� - �������� - ����� ---
    "�����",
    "",
    DISABLE_RU,
    "��������",
    setNRST.exStretch.type, &PageDebug::PageADC::PageStretch::self, Item::Active, PageDebug::PageADC::PageStretch::OnChanged_Mode, Choice::AfterDraw
)


static int16 stretchA;
static int16 stretchB;


void PageDebug::PageADC::PageStretch::OnChanged_Mode(bool)
{
    if (ExtraStretch::TypeIsDisabled())
    {
    }
    else
    {
        stretchA = static_cast<int16>(ExtraStretch::GetValue(Chan::A));
        stretchB = static_cast<int16>(ExtraStretch::GetValue(Chan::B));
    }
}


static bool IsActive_StretchAB()
{
    return ExtraStretch::TypeIsHand();
}

static void OnChanged_Stretch_A()
{
    //set.dbg.nrst.stretchADC[Chan::A] = stretchA;
}

DEF_GOVERNOR(gStretch_A,                                                                                                                     //--- ������� - ��� - �������� - �������� 1� ---
    "�������� 1�",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    stretchA, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_A
)


static void OnChanged_Stretch_B()
{
    //set.dbg.nrst.stretchADC[Chan::B][set.dbg.nrst.stretchADCtype] = stretchB;
}

DEF_GOVERNOR(gStretch_B,                                                                                                                     //--- ������� - ��� - �������� - �������� 2� ---
    "�������� 2�",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    stretchB, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_B
)


DEF_PAGE_3(pStretch,                                                                                                                                       //--- ������� - ��� - �������� ---
    "��������",
    "������������� ����� � �������� �������� (��� ������� ������)",
    &cStretch_Mode,
    &gStretch_A,
    &gStretch_B,
    PageName::Debug_ADC_Stretch,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


static void OnChanged_ShiftType(bool)
{
    RShift(Chan::A).Load(true);
    RShift(Chan::B).Load(true);
}


DEF_CHOICE_2(cAddRShift,
    "�����",
    "",
    DISABLE_RU,
    "��������",
    setNRST.exShift.type, &PageDebug::PageADC::PageShift::self, Item::Active, OnChanged_ShiftType, Choice::AfterDraw
)


DEF_PAGE_1(pShift,
    "���. ��.",
    "��������� ������� ��������������� ������� �� ���������",
    &cAddRShift,
    PageName::Debug_ADC_Shift,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


DEF_PAGE_2(pADC,  //-V1027
    "���",
    "",
//    &pBalance,
    &pStretch,
    &pShift,
    PageName::Debug_ADC,
    &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDebug::PageADC::self = static_cast<const Page *>(&pADC);
const Page *const PageDebug::PageADC::PageStretch::self = static_cast<const Page *>(&pStretch);
const Page *const PageDebug::PageADC::PageBalance::self = static_cast<const Page *>(&pBalance);
const Page *const PageDebug::PageADC::PageShift::self = static_cast<const Page *>(&pShift);
