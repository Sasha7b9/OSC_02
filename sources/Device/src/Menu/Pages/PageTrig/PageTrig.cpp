#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"



void PageTrig::OnChanged_Mode(bool)
{
    Osci::Stop(false);
    if(set.trig.startMode != TrigStartMode::Single)
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
        if (set.trig.startMode == TrigStartMode::Single)
        {
            set.time.sampleTypeOld = set.time.sampleType;
            set.time.sampleType = SampleType::Real;
        }
        else if(set.trig.startMode == TrigStartMode::Auto)    // ����� ����������� ����� ����������
        {
            set.time.sampleType = set.time.sampleTypeOld;
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
    set.trig.startMode, &PageTrig::self, Item::Active, PageTrig::OnChanged_Mode, Choice::AfterDraw
)


static void OnChanged_Source(bool)
{
    TrigSource::Load();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- ����� - �������� ---
    "��������",
    "����� ��������� ������� �������������."
    ,
    "����� 1",
    "����� 2",
    set.trig.source, &PageTrig::self, Item::Active, OnChanged_Source, Choice::AfterDraw
)


static void OnChanged_Polarity(bool)
{
    TrigPolarity::Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- ����� - ���������� ---
    "����������"
    ,
    "1. \"�����\" - ������ ���������� �� ������ ��������������.\n"
    "2. \"����\" - ������ ���������� �� ����� ��������������."
    ,
    "�����",
    "����",
    set.trig.polarity, &PageTrig::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)


static void OnChanged_Filtr(bool)
{
    TrigInput::Load();
}

DEF_CHOICE_3( cFiltr, // -V206                                                                                                                                         //--- ����� - ���� ---
    "������"
    ,
    "����� ������� �� ��������� �������������:\n"
    "1. \"��\" - ������ ������.\n"
    "2. \"���\" - ������ ������ ������.\n"
    "3. \"���\" - ������ ������� ������."
    ,
    "��",
    "��",
    "��",
    set.trig.input, &PageTrig::self, Item::Active, OnChanged_Filtr, Choice::AfterDraw
)

DEF_PAGE_6( pTrig, // -V641 // -V1027                                                                                                                                         //--- ����� ---
    "�����",
    "�������� ��������� �������������.",
    &cMode,                         ///< ����� - �����
    &cSource,                       ///< ����� - ��������
    &cPolarity,                     ///< ����� - ����������
    &cFiltr,                        ///< ����� - ����
    PageTrig::PageHoldOff::self, ///< ����� - ���������
    PageTrig::PageFind::self,    ///< ����� - �����
    PageName::Trig, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageTrig::self = (const Page *)&pTrig;
