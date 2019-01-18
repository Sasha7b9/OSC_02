#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "device.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Display/Display_Primitives.h"
#include "Hardware/Timer.h"
#include "Hardware/Sound.h"
#include "Recorder/Recorder_Display.h"
#include <cstring>

#include "Osci/Osci_Display.h"
#endif


using namespace Display::Primitives;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NEED_SET_ORIENTATION    (bf.needSetOrientation)

#define NU {0, 0}

static struct StructWarning
{
    Warning             warning;
    bool                good;
    uint8               notUsed[2];
    const char * const  message[2][3];
}
warns[Warning::Number] =
{
    {Warning::LimitChan1_Volts, false, NU,           {{"������ ����� 1 - �����/���"},                                            {"LIMIT CHANNEL 1 - VOLTS/DIV"}}},
    {Warning::LimitChan2_Volts, false, NU,           {{"������ ����� 2 - �����/���"},                                            {"LIMIT CHANNEL 2 - VOLTS/DIV"}}},
    {Warning::LimitSweep_Time, false, NU,            {{"������ ��������� - �����/���"},                                               {"LIMIT SWEEP - TIME/DIV"}}},
    {Warning::EnabledPeakDet, false, NU,             {{"������� ������� ��������"},                                                       {"ENABLED PEAK. DET."}}},
    {Warning::LimitChan1_RShift, false, NU,          {{"������ ����� 1 - \x0d"},                                                      {"LIMIT CHANNEL 1 - \x0d"}}},
    {Warning::LimitChan2_RShift, false, NU,          {{"������ ����� 2 - \x0d"},                                                      {"LIMIT CHANNEL 2 - \x0d"}}},
    {Warning::LimitSweep_Level, false, NU,           {{"������ ��������� - �������"},                                                    {"LIMIT SWEEP - LEVEL"}}},
    {Warning::LimitSweep_TShift, false, NU,          {{"������ ��������� - \x97"},                                                        {"LIMIT SWEEP - \x97"}}},
    {Warning::TooSmallSweepForPeakDet, false, NU,    {{"���. ���. �� �������� �� ���������� ����� 0.5���/���"},
                                                                                              {"PEAK. DET. NOT WORK ON SWEETS LESS THAN 0.5us/div"}}},
    {Warning::TooFastScanForRecorder, false, NU,     {{"��������� �� ����� �������� �� ������� ���¨�����"},          {"SELF-RECORDER DOES NOT WORK FAST SCANS"}}},
    {Warning::FileIsSaved, true, NU,                 {{"���� ��������"},                                                                       {"FILE IS SAVED"}}},
    {Warning::SignalIsSaved, true, NU,               {{"������ ��������"},                                                                   {"SIGNAL IS SAVED"}}},
    {Warning::SignalIsDeleted, true, NU,             {{"������ ������"},                                                                   {"SIGNAL IS DELETED"}}},
    {Warning::MenuDebugEnabled, true, NU,            {{"���� ������� ��������"},                                                       {"MENU DEBUG IS ENABLED"}}},
    {Warning::TimeNotSet, true, NU,                  {{"����� �� �����������. ������ ���������� ��� ������"},        {"TIME IS NOT SET. YOU CAN INSTALL IT NOW"}}},
    {Warning::SignalNotFound, true, NU,              {{"������ �� ������"},                                                              {"SIGNAL IS NOT FOUND"}}},
    {Warning::SetTPosToLeft, true, NU,               {{"�� ���������� ��������� 10��/��� ���������� �������-",
                                          "������ \"��������� - \x7b\" � ��������� \"����\" ��� ������-", "��� ������ �������"},
                             {"AT SCANNER SLOWLY 10ms/div DESIRABLY SET \"SCAN - \x7b\" IN", "SWITCH \"Left\" FOR TO ACCELERATE THE OUTPUT SIGNAL"}}},
    {Warning::NeedRebootDevice, true, NU,            {{"��� ���������� ��������� � ����", "��������� ������"},
                                                                              {"FOR THE INTRODUCTION OF CHANGES", "IN FORCE SWITCH OFF THE DEVICE"}}},
    {Warning::ImpossibleEnableMathFunction, true, NU, {{"��������� ���������� ���"},                                           {"DISCONNECT CALCULATION OF FFT"}}},
    {Warning::ImpossibleEnableFFT, true, NU,         {{"��������� �������������� �������"},                                 {"DISCONNECT MATHEMATICAL FUNCTION"}}},
    {Warning::WrongFileSystem, false, NU,            {{"�� ���� ��������� ������. ���������, ��� �� ��� FAT32"},
                                                                                      {"I can not mount a usb flash FDrive:: Make sure to her FAT32"}}},
    {Warning::WrongModePeackDet, true, NU,           {{"����� �������� ����� ������, ��������� ������� ��������"},
                                                                                 {"To change the length of the memory, turn off the peak detector"}}},
    {Warning::DisableChannelB, true, NU,             {{"������� ��������� ����� 2"},                                           {"First, turn off the channel 2"}}},
    {Warning::TooLongMemory, true, NU,               {{"������ ����� ��������� ��� ����� ������ 16� � �����"},
                                                                                     {"The second channel runs at a length of memory 16k and less"}}},
    {Warning::NoPeakDet32k, true, NU,                {{"������� �������� �� �������� ��� ����� ������ 32�"},
                                                                                  {"The peak detector does not work when the memory length of 32k"}}},
    {Warning::NoPeakDet16k, true, NU,                {{"��� ������ �������� ��������� ��� ����� ������ 16�", "��������� ����� 2"},
                                                                         {"For the peak detector with a length of 16k memory", "disable channel 2"}}},
    {Warning::Warn50Ohms, false, NU,                 {{"��� �������� �������� ������������� 50 ��", "����������� �������� �� ���� ����� 5 �"},
                                                              {"When the input impedance value of 50 ohms", "is prohibited to input more than 5 V"}}},
    {Warning::WarnNeedForFlashDrive, true, NU,       {{"������� ���������� ����-����"},                                        {"First connect the flash drive"}}},
    {Warning::FirmwareSaved, true, NU,               {{"�������� ���������"},                                                                 {"Firmware saved"}}},
    {Warning::FullyCompletedOTP, false, NU,          {{"������ OTP ��������� ���������"},                                         {"OTP memory fully completed"}}}
};

#define NUM_WARNINGS    10
static const char       *warnings[NUM_WARNINGS] = {0};      ///< ����� ��������������� ���������.
static uint             timeWarnings[NUM_WARNINGS] = {0};   ///< ����� �����, ����� ��������������� ��������� ��������� �� �����.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
volatile static bool drawRShiftMarkers = false;
static pFuncVV funcOnHand       = 0;
static uint timeStart = 0;
static const char *textWait = 0;
static bool clearBackground = false;
volatile static pFuncVV funcAdditionDraw = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    for(uint8 i = 0; i < Color::NUMBER.value; i++)
    {
        Color(i).SetValue(COLOR(i));
    }

    Painter::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    DEF_STRUCT(StructDraw, pFuncVV) funcs[Device::Mode::Size] =
    {
        Osci::Display::Update,
        Tester::Display::Update,
        Multimeter::Display::Update,
        Recorder::Display::Update
    };

    HANDLER_CHOICE_AND_SAFE_RUN(pFuncVV, Device::State::CurrentMode());

    Console::Draw();

    if (funcAdditionDraw)
    {
        funcAdditionDraw();
    }

    Painter::EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerShowWarning()
{
    uint time = TIME_MS;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (time - timeWarnings[i] > (uint)TIME_MESSAGES * 1000)
        {
            timeWarnings[i] = 0;
            warnings[i] = 0;
        }
    }

    int pointer = 0;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] != 0)
        {
            warnings[pointer] = warnings[i];
            timeWarnings[pointer] = timeWarnings[i];
            if (pointer != i)
            {
                timeWarnings[i] = 0;
                warnings[i] = 0;
            }
            pointer++;
        }
    }

    if (pointer == 0)
    {
        Timer::Disable(Timer::Type::ShowMessages);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ShowWarn(const char *message)
{
    if (warnings[0] == 0)
    {
        Timer::SetAndEnable(Timer::Type::ShowMessages, OnTimerShowWarning, 100);
    }
    bool alreadyStored = false;

    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] == 0 && !alreadyStored)
        {
            warnings[i] = message;
            timeWarnings[i] = TIME_MS;
            alreadyStored = true;
        }
        else if (warnings[i] == message)
        {
            timeWarnings[i] = TIME_MS;
            return;
        }
        else
        {
            // ������ ������ �� ����
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowWarning(Warning::E warning)
{
    Color::ResetFlash();
    for (int i = 2; i >= 0; i--)
    {
        const char *message = warns[warning].message[LANG][i];
        if (message)
        {
            ShowWarn(message);
        }
    }
    if (warns[warning].good)
    {
        Sound::WarnBeepGood();
    }
    else
    {
        Sound::WarnBeepBad();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnRShiftMarkersAutoHide()
{
    drawRShiftMarkers = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ChangedRShiftMarkers(bool /*active*/)
{
    drawRShiftMarkers = !ALT_MARKERS_HIDE;
    Timer::SetAndStartOnce(Timer::Type::RShiftMarkersAutoHide, OnRShiftMarkersAutoHide, 5000);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetDrawMode(DrawMode::E mode, pFuncVV func)
{
    funcOnHand = func;
    if (mode == DrawMode::Hand)
    {
        Timer::SetAndEnable(Timer::Type::Display, funcOnHand, 40);
    }
    else
    {
        Timer::Disable(Timer::Type::Display);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::FuncOnWaitStop()
{
    Display::SetDrawMode(DrawMode::Auto, 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncOnWait()
{
    if (clearBackground)
    {
        Painter::BeginScene(Color::BACK);
    }

    uint time = ((TIME_MS - timeStart) / 50) % 100;

    if (time > 50)
    {
        time = (100 - time);
    }

    int width = 200;
    int height = 80;
    int x = 160 - width / 2;
    int y = 120 - height / 2;

    Region(width, height).Draw(x, y, Color::BACK);
    Rectangle(width, height).Draw(x, y, Color::FILL);

    //Painter::DrawStringInCenterRect(x, y, width, height - 20, textWait);
    Text(textWait).DrawInCenterRect(x, y, width, height - 20);

    char buf[100];
    buf[0] = 0;

    for (uint i = 0; i < time; i++)
    {
        std::strcat(buf, "."); //-V2513
    }

    //Painter::DrawStringInCenterRect(x, y + 20, width, height - 20, buf);
    Text(buf).DrawInCenterRect(x, y + 20, width, height - 20);

    Painter::EndScene();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::FuncOnWaitStart(const char *text, bool eraseBackground)
{
    timeStart = TIME_MS;
    textWait = text;
    clearBackground = eraseBackground;
    Display::SetDrawMode(DrawMode::Hand, FuncOnWait);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RemoveAddDrawFunction()
{
    funcAdditionDraw = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetOrientation(Orientation orientation)
{
    DISPLAY_ORIENTATION = orientation;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetAddDrawFunction(pFuncVV func, uint time)
{
    funcAdditionDraw = func;
    Timer::SetAndStartOnce(Timer::Type::RemoveAddFunction, RemoveAddDrawFunction, time);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BitSet64 Display::PointsOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = (SET_PEAKDET_EN ? (SHIFT_IN_MEMORY / 2) : SHIFT_IN_MEMORY);
    retValue.sword1 = retValue.sword0 + 281;

    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BitSet64 Display::BytesOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = SHIFT_IN_MEMORY;
    retValue.sword1 = retValue.sword0 + (SET_PEAKDET_EN ? 281 * 2 : 281);

    return retValue;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::TimeMenuAutoHide()
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[MENU_AUTO_HIDE] * 1000;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::IsSeparate()
{
    return FUNC_MODE_DRAW_IS_SEPARATE || FFT_ENABLED;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::ENumSignalsInSec::ToFPS() const // -V2506
{
    static const int fps[] = {25, 10, 5, 2, 1};

    return fps[value];
}
