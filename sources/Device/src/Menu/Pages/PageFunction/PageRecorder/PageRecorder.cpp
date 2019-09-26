#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


extern const Page pageRecorder;

const Page * const PageRecorder::self = (const Page *)&pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Destination()
{
    return !Recorder::IsRunning();
}

DEF_CHOICE_3( cDestination,                                                                                                                     //--- ������� - ����������� - ��������� � ---
    "��������� �",
    "���� ��������� ������",
    "���",
    "���",
    "����� ��",
    REC_STORAGE_RECORD, &PageRecorder::self, IsActive_Destination, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Start()
{
    return REC_SRC_A_IS_ENABLED || REC_SRC_B_IS_ENABLED || REC_SRC_SENSOR_IS_ENABLED;
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
    Recorder::OnPressStart();
}

DEF_GRAPH_BUTTON_HINTS_2( bStart,                                                                                                                 //--- ������� - ����������� - ����/���� ---
    "����",
    "������/������� �������� �����������",
    &PageRecorder::self, IsActive_Start, OnPress_Start, Draw_StartStop,
    Draw_Start, "������",
    Draw_Stop, "�������"
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}

DEF_PAGE_4( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ������� - ����������� ---
    "�����������",
    "������ � ��������������� �������� ������ � ��������",
    PageRecorder::PageSource::self,     ///< ������� - ����������� - ��������
    &cDestination,                      ///< ������� - ����������� - ��������� �
    PageRecorder::PageShow::self,       ///< ������� - ����������� - ��������
    &bStart,                            ///< ������� - ����������� - ����/����
    PageName::Function_Recorder, &PageFunction::self, E_BtV, OnOpenClose_Recorder, E_VV, E_BfKE
)
