#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"



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
    set.fft.scale, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_3( cSource,                                                                                                                                  //--- ������� - ������ - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    "����� 1",
    "����� 2",
    "����� 1 + 2",
    set.fft.source, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
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
    return (set.fft.enabled && (set.fft.scale == ScaleFFT::Log));
}

DEF_CHOICE_3( cRange,                                                                                                                                   //--- ������� - ������ - �������� ---
    "��������",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "-40��",
    "-60��",
    "-80��",
    set.fft.maxDB, &PageFFT::self, IsActive_Range, Choice::Changed, Choice::AfterDraw
)


static bool HandlerKey_FFT_Cursors(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        if (event.IsPress() || event.IsRepeat())
        {
            int delta = (event.IsArrowUp() || event.IsArrowRight()) ? 1 : -1;

            set.fft.posCur[set.fft.cursor] += static_cast<uint8>(delta);
            Beeper::RegulatorShiftRotate();

            return true;
        }
    }
    else if(event.key == Key::F1 && event.IsRelease())
    {
        set.fft.cursor = static_cast<uint8>((set.fft.cursor + 1) % 2);
        return true;
    }

    return false;
}

static bool ShowTitle()
{
    return false;
}


static void OnCursors_BeforeDraw()
{
    Region(60, 20).DrawBounded(0, 219, Color::BACK, Color::FILL);

    Text((set.fft.cursor == 0) ? "������ 1" : "������ 2").Draw(12, 224, Color::FILL);
}


DEF_PAGE_0( pCursors,
    "�������",
    PageName::FFT_Cursors, &PageFFT::self, ShowTitle, Page::OpenClose, OnCursors_BeforeDraw, HandlerKey_FFT_Cursors
)


const Page * const PageFFT::Cursors::self = static_cast<const Page *>(&pCursors);

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
