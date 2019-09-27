#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_RangesVoltageDC()
{
    return MULTI_MEASURE == Multimeter::Measure::VoltageDC;
}

static void OnChange_VoltageDC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3( cRangesVoltageDC,                                                                                                                                 //--- ���������� - ������ ---
    "������",
    "�������� ���������",
    "2 �",
    "20 �",
    "500 �",
    MULTI_RANGE_VOLTAGE_DC, &PageMultimeter::self, IsActive_RangesVoltageDC, OnChange_VoltageDC, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_RnagesVoltageAC()
{
    return MULTI_MEASURE == Multimeter::Measure::VoltageAC;
}

static void OnChange_VoltageAC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3( cRangesVoltageAC,                                                                                                                                 //--- ���������� - ������ ---
    "������",
    "�������� ���������",
    "2 �",
    "20 �",
    "400 �",
    MULTI_RANGE_VOLTAGE_AC, &PageMultimeter::self, IsActive_RnagesVoltageAC, OnChange_VoltageAC, E_VII
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_CurrentDC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentDC,
    "������",
    "�������� ���������",
    "20��",
    "2�",
    MULTI_RANGE_CURRENT_DC, &PageMultimeter::self, Item::EmptyActive, OnChange_CurrentDC, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_CurrentAC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentAC,
    "������",
    "�������� ���������",
    "20��",
    "2�",
    MULTI_RANGE_CURRENT_AC, &PageMultimeter::self, Item::EmptyActive, OnChange_CurrentAC, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_RangesReistance()
{
    return MULTI_MEASURE == Multimeter::Measure::Resistance;
}

static void OnChange_Resistance(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_4( cRangesResistance,                                                                                                                                //--- ���������� - ������ ---
    "������",
    "�������� ���������",
    "2 ���",
    "20 ���",
    "200 ���",
    "10 ���",
    MULTI_RANGE_RESISTANCE, &PageMultimeter::self, IsActive_RangesReistance, OnChange_Resistance, E_VII
)

DEF_CHOICE_7( cMode,
    "�����"
    ,
    "����� ���� ����������\n"
    "U= - ���������� ����������\n"
    "U~ - ���������� ����������\n"
    "I= - ���������� ���\n"
    "I~ - ���������� ���\n"
    "R - ������������� ����������� ����\n"
    "VD - ���� ������\n"
    "BELL - ���������"
    ,
    "U=",
    "U~",
    "I=",
    "I~",
    "R",
    "VD",
    "���������",
    MULTI_MEASURE, &PageMultimeter::self, Item::EmptyActive, PageMultimeter::OnChanged_Mode, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_AVP(bool)
{
    Multimeter::ChangeAVP();
}

DEF_CHOICE_2 ( cAVP,
    "���",
    "��������� �������",
    DISABLE_RU,
    ENABLE_RU,
    MULTI_AVP, &PageMultimeter::self, Item::EmptyActive, OnChanged_AVP, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Zero(bool)
{
    //Multimeter::LoadZero();
}

static int8 zero = 0;

DEF_CHOICE_2( cZero,
    "����",
    "",
    DISABLE_RU,
    ENABLE_RU,
    zero, &PageMultimeter::self, Item::EmptyActive, OnChanged_Zero, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

void PageMultimeter::OnChanged_Mode(bool)
{
    Page *page = (Page *)PageMultimeter::self;

    Item **items = (Item **)page->OwnData()->items;

    if (MULTI_MEASURE == Multimeter::Measure::VoltageDC)
    {
        items[1] = (Item *)&cRangesVoltageDC; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::VoltageAC)
    {
        items[1] = (Item *)&cRangesVoltageAC; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentDC)
    {
        items[1] = (Item *)&cRangesCurrentDC;  // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentAC)
    {
        items[1] = (Item *)&cRangesCurrentAC;    // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::Resistance)
    {
        items[1] = (Item *)&cRangesResistance; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::TestDiode)
    {
        items[1] = nullptr;
    }
    else
    {
        // ����� ������
    }

    Multimeter::ChangeMode();
}

static void OnOpenClose_Multimeter(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Multimeter : Device::Mode::Osci);
}

DEF_PAGE_5_VAR( pMultimeter, // -V641 //-V1027 //-V641
    "����������",
    "���������� �������� � ������ �����������",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    &cZero,
    &Item::empty,
    PageName::Function_Multimeter, &PageFunction::self, Item::EmptyActive, OnOpenClose_Multimeter, Page::EmptyBeforeDraw, E_BfKE
)

const Page * const PageMultimeter::self = (const Page *)&pMultimeter;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate0()
{
    Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    "������ 0",
    "",
    &PageMultimeter::PageCalibration::self, Item::EmptyActive, OnPress_Calibrate0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    Multimeter::Calibrate(1);
}

DEF_BUTTON( bCalibrate1,
    "������ 1",
    "",
    &PageMultimeter::PageCalibration::self, Item::EmptyActive, OnPress_Calibrate1
)

DEF_PAGE_2( pCalibration, //-V641 //-V1027
    "����������",
    "���������� �����������",
    &bCalibrate0,
    &bCalibrate1,
    PageName::Function_Multimeter_Cal, &PageMultimeter::self, Item::EmptyActive, E_VB, Page::EmptyBeforeDraw, E_BfKE
)

const Page * const PageMultimeter::PageCalibration::self = (const Page *)&pCalibration;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMultimeter::DecodePassword(const KeyEvent &event)
{
#define NUM_SYMBOLS 8
    /// ������
    static const Key::E password[NUM_SYMBOLS] = { Key::F2, Key::F2, Key::F3, Key::F3, Key::F4, Key::F4, Key::F5, Key::F5 };
    /// ����� ��������� ��������
    static int charsMatch = 0;


    if (!Device::State::InModeMultimeter())         // ������������� ���������� ������ � ������ �����������
    {
        charsMatch = 0;
        return;
    }

    if (Menu::OpenedItem() != (Item *)&cMode)       // � ����������� ��� ��������� ���� "�����" //-V641
    {
        charsMatch = 0;
        return;
    }

    if (event.type != TypePress::Release)
    {
        return;
    }

    if (password[charsMatch++] == event.key)
    {
        if (charsMatch == NUM_SYMBOLS)
        {
            Page *page = (Page *)&pMultimeter;

            Item **items = (Item **)page->OwnData()->items;

            items[4] = (Item *)PageMultimeter::PageCalibration::self;

            Menu::CloseOpenedItem();
        }
    }
    else
    {
        charsMatch = 0;
    }
}
