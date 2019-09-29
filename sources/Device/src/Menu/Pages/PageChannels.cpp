#include "defines.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Keyboard/Keyboard.h"
#include "Settings/Settings.h"
#include "Utils/CommonFunctions.h"
#include "Hardware/Timer.h"
#include "Osci/Osci.h"
#include <cstring>


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
    set.ch[Chan::A].enabled, &PageChannelA::self, Item::Active, PageChannelA::OnChanged_Input, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelA::OnChanged_Couple(bool)
{
    ModeCouple::Set(Chan::A, set.ch[Chan::A].couple);
}

DEF_CHOICE_3( cCoupleA,                                                                                                                                             //--- ����� 1 - ����� ---
    "�����",
    chanCouple,
    "����",
    "�����",
    "�����",
    set.ch[Chan::A].couple, &PageChannelA::self, Item::Active, PageChannelA::OnChanged_Couple, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_BandwidthA(bool)
{
    set.ch[Chan::A].bandwidth.Load();
}

DEF_CHOICE_2( cBandwidthA,                                                                                                                                         //--- ����� 1 - ������ ---
    "������",
    "����� ������ ����������� ������",
    "������",
    "20���",
    set.ch[Chan::A].bandwidth, &PageChannelA::self, Item::Active, OnChanged_BandwidthA, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Balance(Chan::E ch)
{
    Display::FuncOnWaitStart(ch == Chan::A ? "������������ ������ 1" : "������������ ������ 2", false);

    Settings old = set;

    Osci::Balance(ch);

    std::memcpy(&old.dbg.addRShift[0][0], &set.dbg.addRShift[0][0], sizeof(int8) * 2 * Range::Size);

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
    &PageChannelA::self, Item::Active, OnPress_BalanceA
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerA,                                                                                                                                         //--- ����� 1 - �������� ---
    "��������",
    "",
    "1X",
    "10X",
    set.ch[Chan::A].divider, &PageChannelA::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInverseA,                                                                                                                                         //--- ����� 1 - �������� ---
    "��������",
    "����������� ������ ������������ ������ 0�",
    "����",
    "���",
    set.ch[Chan::A].inverse, &PageChannelA::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_6( pChanA, // -V641 // -V1027                                                                                                                                      //--- ����� 1 ---
    "����� 1",
    "�������� ��������� ������ 1.",
    &cInputA,           ///< ����� 1 - ����
    &cCoupleA,          ///< ����� 1 - �����
    &cBandwidthA,       ///< ����� 1 - ������
    &cDividerA,         ///< ����� 1 - ��������
    &bBalanceA,         ///< ����� 1 - ������������
    &cInverseA,         ///< ����� 1 - ��������
    PageName::ChannelA, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageChannelA::self = (const Page *)&pChanA;

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
    set.ch[Chan::B].enabled, &PageChannelB::self, Item::Active, PageChannelB::OnChanged_Input, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelB::OnChanged_Couple(bool)
{
    ModeCouple::Set(Chan::B, set.ch[Chan::B].couple);
}

DEF_CHOICE_3( cCoupleB,                                                                                                                                             //--- ����� 2 - ����� ---
    "�����",
    chanCouple,
    "����",
    "�����",
    "�����",
    set.ch[Chan::B].couple, &PageChannelB::self, Item::Active, PageChannelB::OnChanged_Couple, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_BandwidthB(bool)
{
    set.ch[Chan::B].bandwidth.Load();
}

DEF_CHOICE_2( cBandwidthB,                                                                                                                                         //--- ����� 2 - ������ ---
    "������",
    "",
    "������",
    "20���",
    set.ch[Chan::B].bandwidth, &PageChannelB::self, Item::Active, OnChanged_BandwidthB, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BalanceB()
{
    Balance(Chan::B);
}

DEF_BUTTON( bBalanceB,                                                                                                                                       //--- ����� 2 - ������������ ---
    "������������",
    "",
    &PageChannelB::self, Item::Active, OnPress_BalanceB
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerB,                                                                                                                                         //--- ����� 2 - �������� ---
    "��������",
    "",
    "1X",
    "10X",
    set.ch[Chan::B].divider, &PageChannelB::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInverseB,                                                                                                                                         //--- ����� 2 - �������� ---
    "��������",
    "����������� ������ ������������ ������ 0�",
    "����",
    "���",
    set.ch[Chan::B].inverse, &PageChannelB::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_6( pChanB, // -V641 // -V1027                                                                                                                                      //--- ����� 2 ---
    "����� 2",
    "�������� ��������� ������ 2.",
    &cInputB,           ///< ����� 2 - ����
    &cCoupleB,          ///< ����� 2 - �����
    &cBandwidthB,       ///< ����� 2 - ������
    &cDividerB,         ///< ����� 2 - ��������
    &bBalanceB,         ///< ����� 2 - ������������
    &cInverseB,         ///< ����� 2 - ��������
    PageName::ChannelB, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageChannelB::self = (const Page *)&pChanB;
