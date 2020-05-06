#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"


static void OnChanged_Mode(bool)
{
    Osci::ChangedTrigStartMode();
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
    set.trig.startMode, &PageTrig::self, Item::Active, OnChanged_Mode, Choice::AfterDraw
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
    S_TRIG_SOURCE, &PageTrig::self, Item::Active, OnChanged_Source, Choice::AfterDraw
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
    set.trig.polarity, &PageTrig::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
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
    S_TRIG_INPUT, &PageTrig::self, Item::Active, OnChanged_Filtr, Choice::AfterDraw
)

DEF_PAGE_6( pTrig,                                                                                                                                                            //--- ����� ---
    "�����",
    "�������� ��������� �������������.",
    &cMode,
    &cSource,
    &cPolarity,
    &cFiltr,
    PageTrig::HoldOff::self,
    PageTrig::Find::self,
    PageName::Trig, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTrig::self = static_cast<const Page *>(&pTrig);
