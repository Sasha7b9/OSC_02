#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"

#include "Osci/Osci.h"


using namespace Osci::Settings;


extern const PageBase pTrig;
extern const PageBase ppSearch;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageTrig::OnChanged_TrigMode(bool)
{
    Osci::Stop(false);
    if(!START_MODE_IS_SINGLE)
    {
        FPGA::OnPressStart();
    }
    
    // ���� ��������� � ������ �������������
    if(Osci::InModeRandomizer())
    /// \todo ��� ������ ������� sTime_RandomizeModeEnabled() ���� ������� ������, ��� � �������� ����� ������������ �����������
    //if (SET_TBASE < TBase_50ns)
    {
        // � ������������� �� ��������� ����� �������, �� ���� ��������� ��������� ��� �������, ����� ������������ ��� ����������� � ����� 
        // ������������� �������������� ��� ������
        if (START_MODE_IS_SINGLE)
        {
            SAMPLE_TYPE_OLD = SAMPLE_TYPE;
            SAMPLE_TYPE = SampleType::Real;
        }
        else if(START_MODE_IS_AUTO)    // ����� ����������� ����� ����������
        {
            SAMPLE_TYPE = SAMPLE_TYPE_OLD;
        }
		else
		{
			// ��� ��������
		}
    }
}

DEF_CHOICE_3( cMode, // -V206                                                                                                                                         //--- ����� - ����� ---
    "�����"
    ,
    "����� ����� �������:\n"
    "1. \"����\" - ������ ���������� �������������.\n"
    "2. \"������\" - ������ ���������� �� ������ �������������.\n"
    "3. \"�����������\" - ������ ���������� �� ���������� ������ ������������� ���� ���. ��� ���������� ��������� ����� ������ ������ ����/����."
    ,
    "���� ",
    "������",
    "�����������",
    START_MODE, pTrig, FuncActive, PageTrig::OnChanged_TrigMode, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Source(bool)
{
    Trig::Source::Load();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- ����� - �������� ---
    "��������",
    "����� ��������� ������� �������������."
    ,
    "����� 1",
    "����� 2",
    TRIG_SOURCE, pTrig, FuncActive, OnChanged_Source, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Polarity(bool)
{
    Trig::Polarity::Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- ����� - ���������� ---
    "����������"
    ,
    "1. \"�����\" - ������ ���������� �� ������ ��������������.\n"
    "2. \"����\" - ������ ���������� �� ����� ��������������."
    ,
    "�����",
    "����",
    TRIG_POLARITY, pTrig, FuncActive, OnChanged_Polarity, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Input(bool)
{
    Trig::Input::Load();
}

DEF_CHOICE_3( cInput, // -V206                                                                                                                                         //--- ����� - ���� ---
    "����"
    ,
    "����� ����� � ���������� �������������:\n"
    "1. \"��\" - ������ ������.\n"
    "2. \"���\" - ������ ������ ������.\n"
    "3. \"���\" - ������ ������� ������."
    ,
    "��",
    "��",
    "��",
    TRIG_INPUT, pTrig, FuncActive, OnChanged_Input, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTrig::pointer = &pTrig;

DEF_PAGE_6( pTrig, // -V641 // -V1027                                                                                                                                         //--- ����� ---
    "�����",
    "�������� ��������� �������������.",
    &cMode,                         ///< ����� - �����
    &cSource,                       ///< ����� - ��������
    &cPolarity,                     ///< ����� - ����������
    &cInput,                        ///< ����� - ����
    PageTrig::PageHoldOff::pointer, ///< ����� - ���������
    PageTrig::PageFind::pointer,    ///< ����� - �����
    Page::Name::Trig, nullptr, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
