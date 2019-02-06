#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"


using namespace Osci::Settings;


extern const PageBase pageFind;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTrig::PageFind::pointer = &pageFind;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char *hintsSearch_Mode[] = { "������", "Hand", "��������������",  "Auto" };

static const ChoiceBase cMode =                                                                                                                               //--- ����� - ����� - ����� ---
{
    Control::Type::Choice, 2, false, Page::Name::NoPage, &pageFind, FuncActive,
    {
        "�����", "Mode"
        ,
        "����� ������ ��������������� ������ �������������:\n"
        "1. \"������\" - ����� ������������ �� ������� ������ \"�����\" ��� �� ��������� � ������� 0.5� ������ �����, ���� ����������� "
        "\"������\x99��� ���� �����\x99�����������\".\n"
        "2. \"��������������\" - ����� ������������ �������������."
        ,
        "Selecting the automatic search of synchronization:\n"
        "1. \"Hand\" - search is run on pressing of the button \"Find\" or on deduction during 0.5s the ����� button if it is established "
        "\"SERVICE\x99Mode long �����\x99\x41utolevel\".\n"
        "2. \"Auto\" - the search is automatically."
    },
    (int8 *)&TRIG_MODE_FIND, // -V206
    hintsSearch_Mode, Choice::EmptyChange, Choice::EmptyDraw
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Search()
{
    return TRIG_MODE_FIND_HAND;
}

static void OnPress_Search()
{
    Osci::Settings::Trig::Level::Find();
}

DEF_BUTTON( bSearch,                                                                                                                                          //--- ����� - ����� - ����� ---
    "�����", "Search",
    "���������� ����� ������ �������������.",
    "Runs for search synchronization level.",
    pageFind, IsActive_Search, OnPress_Search, Choice::EmptyDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageFind, // -V641 // -V1027                                                                                                                              //--- ����� - ����� ---
    "�����", "SEARCH",
    "���������� �������������� ������� ������ �������������.",
    "Office of the automatic search the trigger level.",
    &cMode,                         ///< ����� - ����� - �����
    &bSearch,                       ///< ����� - ����� - �����
    Page::Name::Trig_Search, PageTrig::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
