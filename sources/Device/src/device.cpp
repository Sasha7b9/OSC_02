#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Device.h"
#include "Display/Display.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Hardware/Timer.h"
#include "Keyboard/DecoderDevice.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Hardware/Sound.h"
#include "Hardware/VCP.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stdlib.h>
#include <cmath>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Device
{
    /// ��������� ���������� � ������������ � ������������� �������
    static void SetCurrentMode();
    /// ���������� ����� ������ mode, ���� ������� �������� page ��� � ����� ������� �������� �������� ���� page
    static bool SetCurrentMode(const PageBase *page, Mode::E mode);

    static Mode::E currentMode = Mode::Osci;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Device::Init()
{
    SetCurrentMode();

    Hardware::Init();

    VCP::Init();

    Tester::Init();

    Display::Init();

    PAUSE_ON_MS(500);

    FPGA::Init();

    Settings::Load();

    FrequencyCounter::Init();

    FPGA::SET::Load();

    Menu::Init();

    FPGA::OnPressStart();

    Multimeter::Init();

    FDrive::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Device::SetCurrentMode()
{
    if (!SetCurrentMode(PageFunction::PageMultimeter::pointer, Device::Mode::Multimeter))
    {
        if (!SetCurrentMode(PageFunction::PageTester::pointer, Device::Mode::Tester))
        {
            if (!SetCurrentMode(PageFunction::PageRecorder::pointer, Device::Mode::Recorder))
            {

            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool Device::SetCurrentMode(const PageBase *page, Device::Mode::E mode)
{
    Control *opened = Menu::OpenedItem();

    if (opened == (Page *)page || opened->ExistKeeper(page))
    {
        SetMode(mode);
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::Update()
{
    START_MULTI_MEASUREMENT();

    Display::Update();

    FPGA::Update();

    Menu::SaveSettings();

    Tester::Update();

    Multimeter::Update();

    Menu::Update();

    FDrive::Update();

    Decoder::Update();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Device::Mode::E Device::CurrentMode()
{
    return currentMode;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetMode(Mode::E mode)
{
    currentMode = mode;

    if (currentMode == Mode::Osci)
    {
        Tester::Disable();
    }
    else if (currentMode == Mode::Tester)
    {
        Tester::Enable();
    }
    else if (currentMode == Mode::Multimeter)
    {
        Tester::Disable();
    }
    else if (currentMode == Mode::Recorder)
    {
        Tester::Disable();
    }
    else
    {
        // ����� ������ ���� - ��� �������� ���������
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::InModeTester()
{
    return (currentMode == Device::Mode::Tester);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::InModeMultimeter()
{
    return (currentMode == Device::Mode::Multimeter);
}
