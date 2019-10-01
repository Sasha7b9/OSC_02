#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"



static bool IsActive_RangesVoltageDC()
{
    return (set.mult.meas == MultimeterMeasure::VoltageDC);
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
    set.mult.rangeVoltageDC, &PageMultimeter::self, IsActive_RangesVoltageDC, OnChange_VoltageDC, Choice::AfterDraw
)


static bool IsActive_RnagesVoltageAC()
{
    return (set.mult.meas == MultimeterMeasure::VoltageAC);
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
    set.mult.rangeVoltageAC, &PageMultimeter::self, IsActive_RnagesVoltageAC, OnChange_VoltageAC, Choice::AfterDraw
)



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
    set.mult.rangeCurrentDC, &PageMultimeter::self, Item::Active, OnChange_CurrentDC, Choice::AfterDraw
)


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
    set.mult.rangeCurrentAC, &PageMultimeter::self, Item::Active, OnChange_CurrentAC, Choice::AfterDraw
)


static bool IsActive_RangesReistance()
{
    return (set.mult.meas == MultimeterMeasure::Resistance);
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
    set.mult.rangeResist, &PageMultimeter::self, IsActive_RangesReistance, OnChange_Resistance, Choice::AfterDraw
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
    set.mult.meas, &PageMultimeter::self, Item::Active, PageMultimeter::OnChanged_Mode, Choice::AfterDraw
)


static void OnChanged_AVP(bool)
{
    Multimeter::ChangeAVP();
}

DEF_CHOICE_2 ( cAVP,
    "���",
    "��������� �������",
    DISABLE_RU,
    ENABLE_RU,
    set.mult.avp, &PageMultimeter::self, Item::Active, OnChanged_AVP, Choice::AfterDraw
)


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
    zero, &PageMultimeter::self, Item::Active, OnChanged_Zero, Choice::AfterDraw
)


void PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

void PageMultimeter::OnChanged_Mode(bool)
{
    Page *page = (Page *)PageMultimeter::self;

    Item **items = (Item **)page->OwnData()->items;

    if (set.mult.meas == MultimeterMeasure::VoltageDC)
    {
        items[1] = (Item *)&cRangesVoltageDC; //-V641
    }
    else if (set.mult.meas == MultimeterMeasure::VoltageAC)
    {
        items[1] = (Item *)&cRangesVoltageAC; //-V641
    }
    else if (set.mult.meas == MultimeterMeasure::CurrentDC)
    {
        items[1] = (Item *)&cRangesCurrentDC;  // -V641
    }
    else if (set.mult.meas == MultimeterMeasure::CurrentAC)
    {
        items[1] = (Item *)&cRangesCurrentAC;    // -V641
    }
    else if (set.mult.meas == MultimeterMeasure::Resistance)
    {
        items[1] = (Item *)&cRangesResistance; //-V641
    }
    else if (set.mult.meas == MultimeterMeasure::TestDiode)
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
    PageName::Multimeter, &PageFunction::self, Item::Active, OnOpenClose_Multimeter, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageMultimeter::self = (const Page *)&pMultimeter;


static void OnPress_Calibrate0()
{
    Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    "������ 0",
    "",
    &PageMultimeter::PageCalibration::self, Item::Active, OnPress_Calibrate0
)


static void OnPress_Calibrate1()
{
    Multimeter::Calibrate(1);
}

DEF_BUTTON( bCalibrate1,
    "������ 1",
    "",
    &PageMultimeter::PageCalibration::self, Item::Active, OnPress_Calibrate1
)

DEF_PAGE_2( pCalibration, //-V641 //-V1027
    "����������",
    "���������� �����������",
    &bCalibrate0,
    &bCalibrate1,
    PageName::Multimeter_Cal, &PageMultimeter::self, Item::Active, Choice::Changed, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageMultimeter::PageCalibration::self = (const Page *)&pCalibration;


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
