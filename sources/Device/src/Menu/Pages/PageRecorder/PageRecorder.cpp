#include "defines.h"
#include "Device.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"


static bool IsActive_Destination()
{
    return !Recorder::IsRunning();
}

DEF_CHOICE_3( cDestination,                                                                                                                     //--- ������� - ����������� - ��������� � ---
    "��������� �",
    "���� ��������� ������",
    "���",
    "����� ��",
    "����� ��",
    set.rec.storageRecord, &PageRecorder::self, IsActive_Destination, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Start()
{
    return set.rec.enA || set.rec.enB || set.rec.enSensor;
}

static void Draw_Start(int x, int y)
{
    String("����").Draw(x, y + 5);
}

static void Draw_Stop(int x, int y)
{
    String("����").Draw(x, y + 5);
}

static void Draw_StartStop(int x, int y)
{
    Recorder::IsRunning() ? Draw_Stop(x, y) : Draw_Start(x, y);
}

static void OnPress_Start()
{
    if (IsActive_Start())
    {
        Recorder::OnPressStart();
    }
    else
    {
        Display::ShowWarning("����� ������� ���� �� ���� �������� ������");
    }
}

DEF_GRAPH_BUTTON_HINTS_2( bStart,                                                                                                                 //--- ������� - ����������� - ����/���� ---
    "����",
    "������/������� �������� �����������",
    &PageRecorder::self, IsActive_Start, OnPress_Start, Draw_StartStop,
    Draw_Start, "������",
    Draw_Stop, "�������"
)



static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}

DEF_PAGE_4( pRecorder, // -V641 // -V1027                                                                                                                     //--- ������� - ����������� ---
    "�����������",
    "������ � ��������������� �������� ������ � ��������",
    PageRecorder::PageSource::self,
    &cDestination,
    PageRecorder::PageShow::self,
    &bStart,
    PageName::Recorder, &PageFunction::self, Item::Active, OnOpenClose_Recorder, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageRecorder::self = (const Page *)&pRecorder;
