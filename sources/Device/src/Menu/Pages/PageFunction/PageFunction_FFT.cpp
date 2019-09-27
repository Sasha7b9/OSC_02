#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cView,                                                                                                                                 //--- ������� - ������ - ����������� ---
    "�����������",
    "�������� � ��������� ����������� �������",
    DISABLE_RU,
    ENABLE_RU,
    FFT_ENABLED, &PageFFT::self, Item::EmptyActive, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Scale()
{
    return FFT_ENABLED;
}

DEF_CHOICE_2( cScale,                                                                                                                                      //--- ������� - ������ - ����� ---
    "�����",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "��������",
    "��������",
    SCALE_FFT, &PageFFT::self, IsActive_Scale, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Source()
{
    return FFT_ENABLED;
}

DEF_CHOICE_3( cSource,                                                                                                                                  //--- ������� - ������ - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    "����� 1",
    "����� 2",
    "����� 1 + 2",
    SOURCE_FFT, &PageFFT::self, IsActive_Source, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Window()
{
    return FFT_ENABLED;
}

DEF_CHOICE_4( cWindow,                                                                                                                                      //--- ������� - ������ - ���� ---
    "����",
    "����� ���� ��� ������� �������",
    "�����������",
    "��������",
    "��������",
    "�����",
    WINDOW_FFT, &PageFFT::self, IsActive_Window, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Range()
{
    return FFT_ENABLED && SCALE_FFT_IS_LOG;
}

DEF_CHOICE_3( cRange,                                                                                                                                   //--- ������� - ������ - �������� ---
    "��������",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "-40��",
    "-60��",
    "-80��",
    MAX_DB_FFT, &PageFFT::self, IsActive_Range, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Cursors_Source()
{
    MATH_CURRENT_CUR = (uint8)((MATH_CURRENT_CUR + 1) % 2);
}

static void Draw_Cursors_Source(int x, int y)
{
    String(MATH_CURRENT_CUR_IS_0 ? "1" : "2").Draw(x + 7, y + 5);
}

DEF_GRAPH_BUTTON( bCursors_Source,                                                                                                            //--- ������� - ������ - ������� - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    &PageFFT::PageCursors::self, Item::EmptyActive, OnPress_Cursors_Source, Draw_Cursors_Source
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Cursors()
{
    return FFT_ENABLED;
}

static bool OnArrows_FFT_Cursors(const KeyEvent &event)
{
    if (event.type != TypePress::Press && event.type != TypePress::Repeat)
    {
        return true;
    }

    Key::E key = event.key;

    int delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    FFT_POS_CURSOR(MATH_CURRENT_CUR) += (uint8)delta;
    Beeper::RegulatorShiftRotate();

    return true;
}

DEF_PAGE_1( pCursors, // -V641 // -V1027                                                                                                                 //--- ������� - ������ - ������� ---
    "�������",
    "�������� ������� ��� ��������� ���������� �������",
    &bCursors_Source,                       ///< ������ - ������ - ������� - ��������
    PageName::Function_FFT_Cursors, &PageFFT::self, IsActive_Cursors, E_VB, Page::EmptyBeforeDraw, OnArrows_FFT_Cursors
)

const Page * const PageFFT::PageCursors::self = (const Page *)&pCursors;

static bool IsActive_FFT()
{
    return !FUNC_MODE_DRAW_IS_ENABLED;
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
    &cView,         ///< ������ - ������ - �����������
    &cScale,        ///< ������ - ������ - �����
    &cSource,       ///< ������ - ������ - ��������
    &cWindow,       ///< ������ - ������ - ���� 
    &pCursors,      ///< ������ - ������ - �������
    &cRange,        ///< ������ - ������ - ��������
    PageName::Function_FFT, &PageFunction::self, IsActive_FFT, OnOpenClose_FFT, Page::EmptyBeforeDraw, E_BfKE
)

const Page * const PageFFT::self = (const Page *)&pFFT;
