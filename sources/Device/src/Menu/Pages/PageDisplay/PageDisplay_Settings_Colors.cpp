#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageColors;

const PageBase *PageDisplay::PageSettings::PageColors::pointer = &pageColors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_ResetColors()
{
}

DEF_BUTTON( bReset,                                                                                                                          //--- ������� - ��������� - ����� - �������� ---
    "��������",
    "����� ���� ������ �� �������� �� ���������",
    pageColors, EmptyFuncBtV, OnPress_ResetColors, EmptyFuncVII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cScheme,                                                                                                                 //--- ������� - ��������� - ����� - �������� ����� ---
    "���� �����",
    "��������� �������� �����",
    "����� 1",
    "����� 2",
    set.serv_colorScheme, pageColors, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeA = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::Channel(Chan::A))

DEF_GOVERNOR_COLOR( gcChannelA,                                                                                                               //--- ������� - ��������� - ����� - ����� 1 ---
    "����� 1",
    "����� ����� ������ 1",
    PageDisplay::PageSettings::PageColors::colorTypeA, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeB = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::Channel(Chan::B))

DEF_GOVERNOR_COLOR( gcChannelB,                                                                                                               //--- ������� - ��������� - ����� - ����� 2 ---
    "����� 2",
    "����� ����� ������ 1",
    PageDisplay::PageSettings::PageColors::colorTypeB, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeGrid = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID)

DEF_GOVERNOR_COLOR( gcGrid,                                                                                                                     //--- ������� - ��������� - ����� - ����� ---
    "�����",
    "������������� ���� �����",
    PageDisplay::PageSettings::PageColors::colorTypeGrid, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_Colors_Background(bool)
{
    Color::InitGlobalColors();

    gcChannelA.ct->color = Color::Channel(Chan::A);
    gcChannelB.ct->color = Color::Channel(Chan::B);
    gcGrid.ct->color = Color::GRID;

    gcChannelA.ct->ReInit();
    gcChannelB.ct->ReInit();
    gcGrid.ct->ReInit();
}

DEF_CHOICE_2( cBackground,                                                                                                                        //--- ������� - ��������� - ����� - ��� ---
    "���",
    "����� ����� ����",
    "׸����",
    "�����",
    BACKGROUND, pageColors, FuncActive, OnChanged_Settings_Colors_Background, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageDisplay::PageSettings::PageColors::Init()
{
    OnChanged_Settings_Colors_Background(true);
}


/// \todo �������� �������������� ����� 1-�� � 2-�� �������
DEF_PAGE_6( pageColors, // -V641 // -V1027                                                                                                              //--- ������� - ��������� - ����� ---
    "�����",
    "����� ������ �������",
    &bReset,              ///< ������� - ��������� - ����� - ��������
    &cScheme,             ///< ������� - ��������� - ����� - �������� �����
    &gcChannelA,          ///< ������� - ��������� - ����� - ����� 1
    &gcChannelB,          ///< ������� - ��������� - ����� - ����� 2
    &gcGrid,              ///< ������� - ��������� - ����� - �����
    &cBackground,         ///< ������� - ��������� - ����� - ���
    Page::Name::Display_Settings_Colors, PageDisplay::PageSettings::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
