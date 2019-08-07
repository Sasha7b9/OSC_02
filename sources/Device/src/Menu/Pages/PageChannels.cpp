#include "defines.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/MenuItems.h"
#include "Keyboard/Keyboard.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Hardware/Timer.h"

#include "Osci/Osci.h"
#include "Settings/SettingsChannel.h"
#include <cstring>


extern const PageBase pChanA;
extern const PageBase pChanB;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char chanInput[] =   "1. \"���\" - �������� ������ �� �����.\n"
                                  "2. \"����\" - �� �������� ������ �� �����.";

static const char chanCouple[] =  "����� ��� ����� � ���������� �������.\n"
                                  "1. \"����\" - �������� ����.\n"
                                  "2. \"�����\" - �������� ����.\n"
                                  "3. \"�����\" - ���� ������� � �����.";

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelA::OnChanged_Input(bool)
{

}

DEF_CHOICE_2( cInputA,                                                                                                                                               //--- ����� 1 - ���� ---
    "����",
    chanInput,
    DISABLE_RU,
    ENABLE_RU,
    SET_ENABLED_A, pChanA, Choice::FuncActive, PageChannelA::OnChanged_Input, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelA::OnChanged_Couple(bool)
{
    FPGA::Settings::ModeCouple::Set(Chan::A, SET_COUPLE_A);
}

DEF_CHOICE_3( cCoupleA,                                                                                                                                             //--- ����� 1 - ����� ---
    "�����",
    chanCouple,
    "����",
    "�����",
    "�����",
    SET_COUPLE_A, pChanA, Choice::FuncActive, PageChannelA::OnChanged_Couple, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanA_Bandwidth(bool)
{
    SET_BANDWIDTH_A.Load();
}

DEF_CHOICE_2( cBandwidthA,                                                                                                                                         //--- ����� 1 - ������ ---
    "������",
    "����� ������ ����������� ������",
    "������",
    "20���",
    SET_BANDWIDTH_A, pChanA, Choice::FuncActive, OnChanged_ChanA_Bandwidth, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Balance(Chan::E ch)
{
    Display::FuncOnWaitStart(ch == Chan::A ? "������������ ������ 1" : "������������ ������ 2", false);

    Settings old = set;

    Osci::Balance(ch);

    std::memcpy(&old.addRShift[0][0], &set.addRShift[0][0], sizeof(int8) * 2 * Osci::Settings::Range::Size);

    set = old;

    Osci::Init();

    Display::FuncOnWaitStop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BalanceA()
{
    Balance(Chan::A);
}

DEF_BUTTON( bBalanceA,                                                                                                                                      //--- ����� 1 - ������������� ---
    "������������",
    "",
    pChanA, Button::EmptyActive, OnPress_BalanceA, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cDividerA,
    "��������",
    "",
    "1X",
    "10X",
    divider[0], pChanA, Choice::FuncActive, Choice::FuncChange, Choice::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageChannelA::pointer = &pChanA;

DEF_PAGE_5( pChanA, // -V641 // -V1027                                                                                                                                      //--- ����� 1 ---
    "����� 1",
    "�������� ��������� ������ 1.",
    &cInputA,           ///< ����� 1 - ����
    &cCoupleA,          ///< ����� 1 - �����
    &cBandwidthA,       ///< ����� 1 - ������
    &cDividerA,         ///< ����� 1 - ��������
    &bBalanceA,         ///< ����� 1 - ������������
    Page::Name::ChannelA, nullptr, 0, Page::FuncPress, Page::FuncDraw, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelB::OnChanged_Input(bool active)
{
    if (!active)
    {
        Display::ShowWarning(Warning::TooLongMemory);
        return;
    }
}

DEF_CHOICE_2( cInputB,                                                                                                                                               //--- ����� 2 - ���� ---
    "����",
    chanInput,
    DISABLE_RU,
    ENABLE_RU,
    SET_ENABLED_B, pChanB, Choice::FuncActive, PageChannelB::OnChanged_Input, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelB::OnChanged_Couple(bool)
{
    FPGA::Settings::ModeCouple::Set(Chan::B, SET_COUPLE_B);
}

DEF_CHOICE_3( cCoupleB,                                                                                                                                             //--- ����� 2 - ����� ---
    "�����",
    chanCouple,
    "����",
    "�����",
    "�����",
    SET_COUPLE_B, pChanB, Choice::FuncActive, PageChannelB::OnChanged_Couple, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_BandwidthB(bool)
{
    SET_BANDWIDTH_B.Load();
}

DEF_CHOICE_2( cBandwidthB,                                                                                                                                         //--- ����� 2 - ������ ---
    "������",
    "",
    "������",
    "20���",
    SET_BANDWIDTH_B, pChanB, Choice::FuncActive, OnChanged_BandwidthB, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BalanceB()
{
    Balance(Chan::B);
}

DEF_BUTTON( bBalanceB,                                                                                                                                       //--- ����� 2 - ������������ ---
    "������������",
    "",
    pChanB, Button::EmptyActive, OnPress_BalanceB, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerB,
    "��������",
    "",
    "1X",
    "10X",
    divider[1], pChanB, Choice::FuncActive, Choice::FuncChange, Choice::FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageChannelB::pointer = &pChanB;

DEF_PAGE_5( pChanB, // -V641 // -V1027                                                                                                                                      //--- ����� 2 ---
    "����� 2",
    "�������� ��������� ������ 2.",
    &cInputB,           ///< ����� 2 - ����
    &cCoupleB,          ///< ����� 2 - �����
    &cBandwidthB,       ///< ����� 2 - ������
    &cDividerB,
    &bBalanceB,         ///< ����� 2 - ������������
    Page::Name::ChannelB, nullptr, 0, Page::FuncPress, Page::FuncDraw, FuncRegSetPage
)
