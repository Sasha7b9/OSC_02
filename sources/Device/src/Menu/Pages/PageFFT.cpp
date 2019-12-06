#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"


ScaleFFT::E &ScaleFFT::Ref()
{
    return set.fft.scale;
}


SourceFFT::E &SourceFFT::Ref()
{
    return set.fft.source;
}


MaxDBFFT::operator MaxDBFFT::E()
{
    return set.fft.maxDB;
}


bool WindowFFT::IsHamming()
{
    return (set.fft.window == Hamming);
}


bool WindowFFT::IsBlackman()
{
    return (set.fft.window == Blackman);
}


bool WindowFFT::IsHann()
{
    return (set.fft.window == Hann);
}


static bool IsActive_Parameter()
{
    return set.fft.enabled;
}


DEF_CHOICE_2( cView,                                                                                                                                 //--- ������� - ������ - ����������� ---
    "�����������",
    "�������� � ��������� ����������� �������",
    DISABLE_RU,
    ENABLE_RU,
    set.fft.enabled, &PageFFT::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cScale,                                                                                                                                      //--- ������� - ������ - ����� ---
    "�����",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "��������",
    "��������",
    ScaleFFT::Ref(), &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_3( cSource,                                                                                                                                  //--- ������� - ������ - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    "����� 1",
    "����� 2",
    "����� 1 + 2",
    SourceFFT::Ref(), &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_4( cWindow,                                                                                                                                      //--- ������� - ������ - ���� ---
    "����",
    "����� ���� ��� ������� �������",
    "�����������",
    "��������",
    "��������",
    "�����",
    set.fft.window, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Range()
{
    return set.fft.enabled && ScaleFFT::IsLog();
}

DEF_CHOICE_3( cRange,                                                                                                                                   //--- ������� - ������ - �������� ---
    "��������",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "-40��",
    "-60��",
    "-80��",
    set.fft.maxDB, &PageFFT::self, IsActive_Range, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Cursors_Source()
{
    set.fft.cursor = static_cast<uint8>((set.fft.cursor + 1) % 2);
}

static void Draw_Cursors_Source(int x, int y)
{
    String((set.fft.cursor == 0) ? "1" : "2").Draw(x + 7, y + 5);
}

DEF_GRAPH_BUTTON( bCursors_Source,                                                                                                            //--- ������� - ������ - ������� - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    &PageFFT::PageCursors::self, Item::Active, OnPress_Cursors_Source, Draw_Cursors_Source
)


static bool HandlerKey_FFT_Cursors(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        if (event.IsPress() || event.IsRepeat())
        {
            int delta = (event.IsUp() || event.IsRight()) ? 1 : -1;

            set.fft.posCur[set.fft.cursor] += static_cast<uint8>(delta);
            Beeper::RegulatorShiftRotate();

            return true;
        }
    }

    return false;
}

DEF_PAGE_1( pCursors,                                                                                                                                    //--- ������� - ������ - ������� ---
    "�������", 
    "�������� ������� ��� ��������� ���������� �������",
    &bCursors_Source,
    PageName::FFT_Cursors, &PageFFT::self, IsActive_Parameter, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, HandlerKey_FFT_Cursors
)

const Page * const PageFFT::PageCursors::self = static_cast<const Page *>(&pCursors);

static bool IsActive_FFT()
{
    return FuncModeDraw::IsDisabled();
}

static void OnOpenClose_FFT(bool)
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning("��������� �������������� �������");
    }
}

DEF_PAGE_6( pFFT,                                                                                                                                                   //--- ������ - ������ ---
    "������",
    "����������� ������� �������� �������",
    &cView,
    &cScale,
    &cSource,
    &cWindow,
    &pCursors,
    &cRange,
    PageName::FFT, &PageFunction::self, IsActive_FFT, Page::NormalTitle, OnOpenClose_FFT, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFFT::self = static_cast<const Page *>(&pFFT);
