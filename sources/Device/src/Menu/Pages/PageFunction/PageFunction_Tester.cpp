#include "defines.h"
#include "Tester/Tester.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"
#include "Tester/Tester_Settings.h"


extern const PageDef pageTester;

const Page * const PageTester::self = (const Page *)&pageTester;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Polarity(bool)
{
    Tester::LoadPolarity();
}


DEF_CHOICE_2( cPolarity,                                                                                                                              //--- ������-��������� - ���������� ---
    "����������",
    "���������� �������������� �����������",
    "+",
    "-",
    TESTER_POLARITY, &PageTester::self, 0, OnChanged_Polarity, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cControl,                                                                                                                             //--- ������-��������� - ���������� ---
    "����������",
    "��� �������������� �����������",
    "����������",
    "���",
    TESTER_CONTROL, &PageTester::self, 0, PageTester::OnChanged_Control, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Step(bool)
{
    Tester::LoadStep();
}

DEF_CHOICE_2(   cStepU,                                                                                                                                      //--- ������-��������� - ��� ---
    "���",
    "��� �������������� ����������",
    "600 ��",
    "3 �",
    TESTER_STEP_U, &PageTester::self, 0, OnChanged_Step, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cStepI,
    "���",
    "��� ��������������� ����",
    "4 ���",
    "20 ���",
    TESTER_STEP_I, &PageTester::self, 0, OnChanged_Step, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cViewMode,
    "���������",
    "",
    "�����",
    "�����",
    TESTER_VIEW_MODE, &PageTester::self, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_9( cAveraging,
    "����������",
    "����� ���������� ����������� ���������, �� ������� ����� ���������� �������� ���������. ��� ���� ��� ��������, ��� ����� ���������� ���������� ���������, �� ������ �����",
    "1",
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "256",
    TESTER_ENUM_AVERAGE, &PageTester::self, 0, 0, 0
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageTester::Init()
{
    OnChanged_Control(true);
}

static void OnEnterExit_Tester(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
}


DEF_PAGE_5( pageTester, // -V641
    "������",
    "",
    &cControl,
    &cStepU,
    &cPolarity,
    &cViewMode,
    &cAveraging,
    Page::Name::Function_Tester, &PageFunction::self, 0, OnEnterExit_Tester, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageTester::OnChanged_Control(bool)
{
    Page *page = (Page *)&pageTester;

    Control **items = (Control **)page->items;

    if (TESTER_CONTROL_IS_U)
    {
        items[1] = (Control *)&cStepU; //-V641
    }
    else
    {
        items[1] = (Control *)&cStepI; //-V641
    }

    Tester::LoadStep();
}
