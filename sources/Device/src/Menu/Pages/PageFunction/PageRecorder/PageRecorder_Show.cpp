#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder_Storage.h"
#include "Recorder/Recorder_Display.h"


extern const Page pShow;

const Page * const PageRecorder::PageShow::self = (const Page *)&pShow;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEF_CHOICE_3( cSource,                                                                                                                  //--- ������� - ����������� - �������� - �������� ---
//    "��������",
//    "����� ������������ ������",
//    "���",
//    "USB-Flash",
//    "USB-PC",
//    RECORDER_STORAGE_PLAY, &PageRecorder::PageShow::self, 0, 0, 0
//)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cCursor,
    "������",
    "",
    "1",
    "2",
    Recorder::Settings::currentCursor, &PageRecorder::PageShow::self, E_BtV, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{
    Recorder::Display::MoveLeft();
}

DEF_BUTTON( bNext,                                                                                                                     //--- ������� - ����������� - �������� - ��������� ---
    "�����",
    "",
    &PageRecorder::PageShow::self, E_BtV, OnPress_Next
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Prev()
{
    Recorder::Display::MoveRight();
}

DEF_BUTTON( bPrev,                                                                                                                    //--- ������� - ����������� - �������� - ���������� ---
    "������",
    "",
    &PageRecorder::PageShow::self, E_BtV, OnPress_Prev
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_PageShow()
{
    return (Recorder::Storage::CurrentFrame()->NumPoints() != 0) && (!Recorder::IsRunning());
}

static bool OnArrows_PageShow(KeyEvent event)
{
    if (event.type == TypePress::Press || event.type == TypePress::Repeat)
    {
        if (event.key == Key::Left)
        {
            Recorder::Display::MoveCursorLeft();
            return true;
        }
        else if (event.key == Key::Right)
        {
            Recorder::Display::MoveCursorRight();
            return true;
        }
        else
        {
            // ����� ������ �� ����
        }
    }

    return false;
}

DEF_PAGE_3( pShow, // -V641 // -V1027                                                                                                              //--- ������� - ����������� - �������� ---
    "��������",
    "�������� ���������� ������",
    //&cSource,                                                       ///< ������� - ����������� - �������� - ��������
    &bNext,                                                         ///< ������� - ����������� - �������� - ���������
    &bPrev,                                                         ///< ������� - ����������� - �������� - ����������
    &cCursor,
    //PageRecorder::PageShow::PageOperations::self,  ///< ������� - ����������� - �������� - ��������
    //PageRecorder::PageShow::PageView::self,        ///< ������� - ����������� - �������� - ��������
    PageName::Function_Recorder_Show, &PageRecorder::self, IsActive_PageShow, E_VB, E_VV, OnArrows_PageShow
)
