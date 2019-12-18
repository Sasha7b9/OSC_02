#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"


void PageTrig::OnChanged_Mode(bool)
{


    Osci::Stop(false);
    if(!TrigStartMode::IsSingle())
    {
        Osci::OnPressStart();
    }
    
    // ���� ��������� � ������ �������������
    if(Osci::InModeRandomizer())
    /// \todo ��� ������ ������� sTime_RandomizeModeEnabled() ���� ������� ������, ��� � �������� ����� ������������ �����������
    //if (SET_TBASE < TBase_50ns)
    {
        // � ������������� �� ��������� ����� �������, �� ���� ��������� ��������� ��� �������, ����� ������������ ��� ����������� � ����� 
        // ������������� �������������� ��� ������
        if (TrigStartMode::IsSingle())
        {
            set.time.sampleTypeOld = SampleType();
            SampleType().Set(SampleType::Real);
        }
        else if(TrigStartMode::IsAuto())    // ����� ����������� ����� ����������
        {
            SampleType().Set(set.time.sampleTypeOld);
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
    TrigStartMode::Ref(), &PageTrig::self, Item::Active, PageTrig::OnChanged_Mode, Choice::AfterDraw
)


static void OnChanged_Source(bool)
{
    TrigSource::Set();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- ����� - �������� ---
    "��������",
    "����� ��������� ������� �������������."
    ,
    "����� 1",
    "����� 2",
    TrigSource::Ref(), &PageTrig::self, Item::Active, OnChanged_Source, Choice::AfterDraw
)


static void OnChanged_Polarity(bool)
{
    TrigPolarity().Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- ����� - ���������� ---
    "����������"
    ,
    "1. \"�����\" - ������ ���������� �� ������ ��������������.\n"
    "2. \"����\" - ������ ���������� �� ����� ��������������."
    ,
    "�����",
    "����",
    TrigPolarity::Ref(), &PageTrig::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)


static void OnChanged_Filtr(bool)
{
    TrigInput().Load();
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
    TrigInput::Ref(), &PageTrig::self, Item::Active, OnChanged_Filtr, Choice::AfterDraw
)

DEF_PAGE_6( pTrig,                                                                                                                                                            //--- ����� ---
    "�����",
    "�������� ��������� �������������.",
    &cMode,
    &cSource,
    &cPolarity,
    &cFiltr,
    PageTrig::PageHoldOff::self,
    PageTrig::PageFind::self,
    PageName::Trig, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTrig::self = static_cast<const Page *>(&pTrig);
