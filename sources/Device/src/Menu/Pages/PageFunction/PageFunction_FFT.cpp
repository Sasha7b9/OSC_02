#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase ppFFT;
extern const PageBase pppFFT_Cursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cFFT_View,                                                                                                                             //--- ������� - ������ - ����������� ---
    "�����������",
    "�������� � ��������� ����������� �������",
    DISABLE_RU,
    ENABLE_RU,
    FFT_ENABLED, ppFFT, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cFFT_Scale,                                                                                                                                  //--- ������� - ������ - ����� ---
    "�����",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "��������",
    "��������",
    SCALE_FFT, ppFFT, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3( cFFT_Source,                                                                                                                              //--- ������� - ������ - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    "����� 1",
    "����� 2",
    "����� 1 + 2",
    SOURCE_FFT, ppFFT, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4( cFFT_Window,                                                                                                                                  //--- ������� - ������ - ���� ---
    "����",
    "����� ���� ��� ������� �������",
    "�����������",
    "��������",
    "��������",
    "�����",
    WINDOW_FFT, ppFFT, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_FFT_Range()
{
    return SCALE_FFT_IS_LOG;
}

DEF_CHOICE_3( cFFT_Range,                                                                                                                               //--- ������� - ������ - �������� ---
    "��������",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "-40��",
    "-60��",
    "-80��",
    MAX_DB_FFT, ppFFT, IsActive_FFT_Range, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_FFT_Cursors_Source()
{
    MATH_CURRENT_CUR = (uint8)((MATH_CURRENT_CUR + 1) % 2);
}

static void Draw_FFT_Cursors_Source(int x, int y)
{
    String(MATH_CURRENT_CUR_IS_0 ? "1" : "2").Draw(x + 7, y + 5);
}

DEF_SMALL_BUTTON( bFFT_Cursors_Source,                                                                                                        //--- ������� - ������ - ������� - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    pppFFT_Cursors, 0, OnPress_FFT_Cursors_Source, Draw_FFT_Cursors_Source
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_FFT_Cursors()
{
    return FFT_ENABLED;
}

static bool HandlerKey_FFT_Cursors(KeyEvent event)
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

DEF_PAGE_1( pppFFT_Cursors, // -V641 // -V1027                                                                                                           //--- ������� - ������ - ������� ---
    "�������",
    "�������� ������� ��� ��������� ���������� �������",
    &bFFT_Cursors_Source,                       ///< ������ - ������ - ������� - ��������
    Page::Name::SB_Service_FFT_Cursors, 
    &ppFFT,                                     ///< ������ - ������
    IsActive_FFT_Cursors, Page::FuncPress, 0, HandlerKey_FFT_Cursors
)



static bool IsActive_FFT()
{
    return !FUNC_MODE_DRAW_IS_ENABLED;
}

static void OnPress_FFT(bool)
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning(Warning::ImpossibleEnableFFT);
    }
}

const PageBase *PageFunction::PageFFT::pointer = &ppFFT;


DEF_PAGE_6( ppFFT, // -V641 // -V1027                                                                                                                               //--- ������ - ������ ---
    "������",
    "����������� ������� �������� �������",
    &cFFT_View,         ///< ������ - ������ - �����������
    &cFFT_Scale,        ///< ������ - ������ - �����
    &cFFT_Source,       ///< ������ - ������ - ��������
    &cFFT_Window,       ///< ������ - ������ - ���� 
    &pppFFT_Cursors,    ///< ������ - ������ - �������
    &cFFT_Range,        ///< ������ - ������ - ��������
    Page::Name::Function_FFT, PageFunction::pointer, IsActive_FFT, OnPress_FFT, 0, FuncRegSetPage
)
