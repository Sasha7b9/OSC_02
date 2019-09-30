#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"



DEF_CHOICE_2( cView,                                                                                                                                 //--- ������� - ������ - ����������� ---
    "�����������",
    "�������� � ��������� ����������� �������",
    DISABLE_RU,
    ENABLE_RU,
    set.fft.enabled, &PageFFT::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Scale()
{
    return set.fft.enabled;
}

DEF_CHOICE_2( cScale,                                                                                                                                      //--- ������� - ������ - ����� ---
    "�����",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "��������",
    "��������",
    set.fft.scale, &PageFFT::self, IsActive_Scale, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Source()
{
    return set.fft.enabled;
}

DEF_CHOICE_3( cSource,                                                                                                                                  //--- ������� - ������ - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    "����� 1",
    "����� 2",
    "����� 1 + 2",
    set.fft.source, &PageFFT::self, IsActive_Source, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Window()
{
    return set.fft.enabled;
}

DEF_CHOICE_4( cWindow,                                                                                                                                      //--- ������� - ������ - ���� ---
    "����",
    "����� ���� ��� ������� �������",
    "�����������",
    "��������",
    "��������",
    "�����",
    set.fft.window, &PageFFT::self, IsActive_Window, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Range()
{
    return set.fft.enabled && (set.fft.scale == ScaleFFT::Log);
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
    set.fft.cursor = (uint8)((set.fft.cursor + 1) % 2);
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


static bool IsActive_Cursors()
{
    return set.fft.enabled;
}

static bool HandlerKey_FFT_Cursors(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        if (event.type == TypePress::Press || event.type == TypePress::Repeat)
        {
            Key::E key = event.key;

            int delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

            set.fft.posCur[set.fft.cursor] += (uint8)delta;
            Beeper::RegulatorShiftRotate();

            return true;
        }
    }

    return false;
}

DEF_PAGE_1( pCursors, // -V641 // -V1027                                                                                                                 //--- ������� - ������ - ������� ---
    "�������",
    "�������� ������� ��� ��������� ���������� �������",
    &bCursors_Source,
    PageName::Function_FFT_Cursors, &PageFFT::self, IsActive_Cursors, Choice::Changed, Page::BeforeDraw, HandlerKey_FFT_Cursors
)

const Page * const PageFFT::PageCursors::self = (const Page *)&pCursors;

static bool IsActive_FFT()
{
    return (set.math.modeDraw != FuncModeDraw::Disable);
}

static void OnOpenClose_FFT(bool)
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning(Warning::ImpossibleEnableFFT);
    }
}

DEF_PAGE_6( pFFT, // -V641 // -V1027                                                                                                                                //--- ������ - ������ ---
    "������",
    "����������� ������� �������� �������",
    &cView,
    &cScale,
    &cSource,
    &cWindow,
    &pCursors,
    &cRange,
    PageName::Function_FFT, &PageFunction::self, IsActive_FFT, OnOpenClose_FFT, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFFT::self = (const Page *)&pFFT;
