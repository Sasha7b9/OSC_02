#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageTime.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"


int TPos::PosX()
{
    int x[] = { Grid::Left(), (Grid::Right() - Grid::Left()) / 2 + Grid::Left(), Grid::Right() };

    return x[TPos()];
}


void TPos::Draw()
{
    int x0 = PosX() - 3;

    int y = Grid::Top() - 1;
    
    Region(6, 6).Fill(x0, y, Color::BACK);

    Char(Symbol8::TPOS_1).Draw(x0, y, Color::FILL);
}


PeakDetMode::E &PeakDetMode::Ref()
{
    return set.time.peakDet;
}


SampleType::E &SampleType::Ref()
{
    return set.time.sampleType;
}


LinkingTShift::E &LinkingTShift::Ref()
{
    return set.time.linkingTShift;
}


TPos::E &TPos::Ref()
{
    return set.time.tPos;
}


int TPos::InBytes()
{
    static const int m[][2][3] =
    {
        {{0,   256,  510},  {0,   512,  1022}},
        {{0,   512,  1022}, {0,  1024,  2046}},
        {{0,  1024,  2046}, {0,  2048,  4094}},
        {{0,  2048,  4094}, {0,  4096,  8190}},
        {{0,  4096,  8190}, {0,  8192, 16382}},
        {{0,  8192, 16382}, {0,  8192, 16382}},
        {{0, 16384, 32766}, {0, 16384, 32766}}
    };
    return m[ENumPointsFPGA()][PeakDetMode()][Ref()];
}


bool PeakDetMode::IsEnabled()
{
    return (Ref() == PeakDetMode::Enabled) && (TBase() >= TBase::MIN_PEAK_DET);
}


static bool IsActive_Sample()
{
    return OSCI_IN_MODE_RANDOMIZER && !TrigStartMode::IsSingle();
}

DEF_CHOICE_2( cSample,                                                                                                                                          //--- ��������� - ������� ---
    "�������"
    ,
    "\"��������\" - \n"
    "\"�������������\" -"
    ,
    "�������� �����",
    "�������������",
    SampleType::Ref(), &PageTime::self, IsActive_Sample, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_PeakDet()
{
    return (TBase() >= TBase::MIN_PEAK_DET);
}

void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        Osci::OnChangedPoints();
    }
    else
    {
        Display::ShowWarning("���. ���. �� �������� �� ��������� ����� 0.5���/���");
    }
}

DEF_CHOICE_2( cPeakDet,                                                                                                                                         //--- ��������� - ��� ��� ---
    "��� ���",
    "��������/��������� ������� ��������."
    ,
    DISABLE_RU,
    ENABLE_RU,
    PeakDetMode::Ref(), &PageTime::self, IsActive_PeakDet, PageTime::OnChanged_PeakDet, Choice::AfterDraw
)


void PageTime::OnChanged_TPos(bool active)
{
    PageMemory::OnChanged_Points(active);
    TShift::Set(TShift());
}

DEF_CHOICE_3( cTPos,                                                                                                                                                 //--- ��������� - �� ---
    "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����."
    ,
    "����",
    "�����",
    "�����",
    TPos::Ref(), &PageTime::self, Item::Active, PageTime::OnChanged_TPos, Choice::AfterDraw
)


DEF_CHOICE_2( cShiftXtype,                                                                                                                                     //--- ��������� - �������� ---
    "��������",
    "����� ����� ��������� �������� �� �����������\n1. \"�����\" - ����������� ���������� �������� � ��������.\n2. \"�������\" - ����������� ��������� ������� �� ������."
    ,
    "�����",
    "�������",
    LinkingTShift::Ref(), &PageTime::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

DEF_PAGE_4( pTime,                                                                                                                                                        //--- ��������� ---
    "����",
    "�������� ��������� ��������",
    &cSample,
    &cPeakDet,
    &cTPos,
    //&cDivRole,
    &cShiftXtype,
    PageName::Time, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTime::self = static_cast<const Page *>(&pTime);
