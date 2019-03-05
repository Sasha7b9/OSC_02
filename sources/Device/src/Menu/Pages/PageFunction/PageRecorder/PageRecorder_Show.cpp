#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder_Storage.h"
#include "Recorder/Recorder_Display.h"


extern const PageBase pageShow;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3(cSource,                                                                                                                  //--- ������� - ����������� - �������� - �������� ---
    "��������", "Source",
    "����� ������������ ������",
    "Location of data",
    "���", "RAM",
    "USB-Flash", "USB-Flash",
    "USB-PC", "USB-PC",
    RECORDER_STORAGE_PLAY, pageShow, Choice::EmptyActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cCursor,
    "������", "Cursor",
    "",
    "",
    "1", "1",
    "2", "2",
    Recorder::Settings::currentCursor, pageShow, Choice::EmptyActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{
    Recorder::Display::MoveLeft();
}

DEF_BUTTON( bNext,                                                                                                                     //--- ������� - ����������� - �������� - ��������� ---
    "�����", "Left",
    "",
    "",
    pageShow, FuncActive, OnPress_Next, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Prev()
{
    Recorder::Display::MoveRight();
}

DEF_BUTTON( bPrev,                                                                                                                    //--- ������� - ����������� - �������� - ���������� ---
    "������", "Right",
    "",
    "",
    pageShow, FuncActive, OnPress_Prev, Button::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_PageShow()
{
    return (Recorder::Storage::CurrentFrame()->NumPoints() != 0) && (!Recorder::IsRunning());
}

static bool FuncOnKey_PageShow(KeyEvent event)
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

DEF_PAGE_3( pageShow, // -V641 // -V1027                                                                                                           //--- ������� - ����������� - �������� ---
    "��������", "VIEW",
    "�������� ���������� ������",
    "View recorded data",
    //&cSource,                                                       ///< ������� - ����������� - �������� - ��������
    &bNext,                                                         ///< ������� - ����������� - �������� - ���������
    &bPrev,                                                         ///< ������� - ����������� - �������� - ����������
    &cCursor,
    //PageFunction::PageRecorder::PageShow::PageOperations::pointer,  ///< ������� - ����������� - �������� - ��������
    //PageFunction::PageRecorder::PageShow::PageView::pointer,        ///< ������� - ����������� - �������� - ��������
    Page::Name::Function_Recorder_Show, PageFunction::PageRecorder::pointer, IsActive_PageShow, FuncPressPage, FuncDrawPage, FuncOnKey_PageShow
)

const PageBase *PageFunction::PageRecorder::PageShow::pointer = &pageShow;
