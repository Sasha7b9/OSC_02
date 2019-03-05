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
    "�����", "Mode",
    "����� ����� �������:\n"
    "1. \"����\" - ������ ���������� �������������.\n"
    "2. \"������\" - ������ ���������� �� ������ �������������.\n"
    "3. \"�����������\" - ������ ���������� �� ���������� ������ ������������� ���� ���. ��� ���������� ��������� ����� ������ ������ ����/����.",
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to press the START/STOP.",
    "���� ",       "Auto",
    "������",      "Wait",
    "�����������", "Single",
    START_MODE, pTrig, FuncActive, PageTrig::OnChanged_TrigMode, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Source(bool)
{
    Trig::Source::Load();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- ����� - �������� ---
    "��������", "Source",
    "����� ��������� ������� �������������.",
    "Synchronization signal source choice.",
    "����� 1", "Chan 1",
    "����� 2", "Chan 2",
    TRIG_SOURCE, pTrig, FuncActive, OnChanged_Source, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Polarity(bool)
{
    Trig::Polarity::Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- ����� - ���������� ---
    "����������", "Polarity",
    "1. \"�����\" - ������ ���������� �� ������ ��������������.\n"
    "2. \"����\" - ������ ���������� �� ����� ��������������.",
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut.",
    "�����", "Front",
    "����",  "Back",
    TRIG_POLARITY, pTrig, FuncActive, OnChanged_Polarity, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Input(bool)
{
    Trig::Input::Load();
}

DEF_CHOICE_3( cInput, // -V206                                                                                                                                         //--- ����� - ���� ---
    "����", "Input",
    "����� ����� � ���������� �������������:\n"
    "1. \"��\" - ������ ������.\n"
    "2. \"���\" - ������ ������ ������.\n"
    "3. \"���\" - ������ ������� ������.",
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"LPF\" - low-pass filter.\n"
    "3. \"HPF\" - high-pass filter frequency.",
    "��", "Full",
    "��", "LP",
    "��", "HP",
    TRIG_INPUT, pTrig, FuncActive, OnChanged_Input, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTrig::pointer = &pTrig;

DEF_PAGE_6( pTrig, // -V641 // -V1027                                                                                                                                         //--- ����� ---
    "�����", "TRIG",
    "�������� ��������� �������������.",
    "Contains synchronization settings.",
    &cMode,                         ///< ����� - �����
    &cSource,                       ///< ����� - ��������
    &cPolarity,                     ///< ����� - ����������
    &cInput,                        ///< ����� - ����
    PageTrig::PageHoldOff::pointer, ///< ����� - ���������
    PageTrig::PageFind::pointer,    ///< ����� - �����
    Page::Name::Trig, nullptr, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
