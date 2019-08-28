#include "defines.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/MenuItems.h"
#include "Keyboard/Keyboard.h"
#include "Settings/Settings.h"
#include "Utils/CommonFunctions.h"
#include "Hardware/Timer.h"
#include "Osci/Osci.h"
#include "Settings/SettingsChannel.h"

#include <cstring>


extern const Page pChanA;
extern const Page pChanB;

const Page * const PageChannelA::self = (const Page *)&pChanA;
const Page * const PageChannelB::self = (const Page *)&pChanB;

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
    SET_ENABLED_A, &PageChannelA::self, E_BtV, PageChannelA::OnChanged_Input, E_VII
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
    SET_COUPLE_A, &PageChannelA::self, E_BtV, PageChannelA::OnChanged_Couple, E_VII
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
    SET_BANDWIDTH_A, &PageChannelA::self, E_BtV, OnChanged_ChanA_Bandwidth, E_VII
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
    &PageChannelA::self, E_BtV, OnPress_BalanceA
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerA,                                                                                                                                         //--- ����� 1 - �������� ---
    "��������",
    "",
    "1X",
    "10X",
    divider[0], &PageChannelA::self, E_BtV, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInverseA,                                                                                                                                         //--- ����� 1 - �������� ---
    "��������",
    "����������� ������ ������������ ������ 0�",
    "����",
    "���",
    SET_INVERSE_A, &PageChannelA::self, E_BtV, E_VB, E_VII
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
    PageName::ChannelA, nullptr, E_BtV, E_VB, E_VV, E_BfKE
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
    SET_ENABLED_B, &PageChannelB::self, E_BtV, PageChannelB::OnChanged_Input, E_VII
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
    SET_COUPLE_B, &PageChannelB::self, E_BtV, PageChannelB::OnChanged_Couple, E_VII
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
    SET_BANDWIDTH_B, &PageChannelB::self, E_BtV, OnChanged_BandwidthB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BalanceB()
{
    Balance(Chan::B);
}

DEF_BUTTON( bBalanceB,                                                                                                                                       //--- ����� 2 - ������������ ---
    "������������",
    "",
    &PageChannelB::self, E_BtV, OnPress_BalanceB
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerB,                                                                                                                                         //--- ����� 2 - �������� ---
    "��������",
    "",
    "1X",
    "10X",
    divider[1], &PageChannelB::self, E_BtV, E_VB, E_VII
    )

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInverseB,                                                                                                                                         //--- ����� 2 - �������� ---
    "��������",
    "����������� ������ ������������ ������ 0�",
    "����",
    "���",
    SET_INVERSE_B, &PageChannelB::self, E_BtV, E_VB, E_VII
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
    PageName::ChannelB, nullptr, E_BtV, E_VB, E_VV, E_BfKE
)
