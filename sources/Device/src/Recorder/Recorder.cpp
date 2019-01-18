#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display/Display.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/FSMC.h"
#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Settings.h"
#include "Recorder/Recorder_Storage.h"
#endif


using namespace FPGA::ADDR;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��������� ������������ ����� ��������� � ����� ������������
struct StateOsci
{
    /// �������� �� ��������� ��� ������� � ������� �������
    uint16 storedRShift[2];
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���� true - ��� ������ �����
static bool running = false;
/// true, ���� ����������� ��� ���������������
static bool initialized = false;
/// ���� �������� ��������� ������������ � ������ �������� � ����� ������������
static StateOsci osci;

/// ������ ����� �� ���������� �� FPGA
static void ReadPoint();
/// ��������� ������������� ��������� ������������
static void StoreOsciSettings();
/// ������������ ����� ���������� ��������� �����������
static void RestoreOsciSettings();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Init()
{
    StoreOsciSettings();

    FPGA::HAL::LoadRegUPR();
    Osci::Settings::Range::LoadBoth();
    Osci::Settings::Trig::Input::Load();
    Settings::ScaleX::Load();
    Osci::Settings::TShift::Load();
    Osci::Settings::LoadHoldfOff();

    FPGA::HAL::Interrupt::P2P::Init(ReadPoint);

    running = false;

    Stop();

    Storage::Init();

    initialized = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::DeInit()
{
    if (!initialized)
    {
        return;
    }

    Stop();

    RestoreOsciSettings();

    initialized = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Update()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadPoint()
{
    if (Display::InProcess())
    {
        Display::SetFuncAfterUpadteOnce(ReadPoint);
    }
    else
    {
        BitSet16 dataA(FSMC::ReadFromFPGA(RD::DATA_A), FSMC::ReadFromFPGA(RD::DATA_A + 1));
        BitSet16 dataB(FSMC::ReadFromFPGA(RD::DATA_B), FSMC::ReadFromFPGA(RD::DATA_B + 1));

        Recorder::Storage::CurrentFrame().AddPoint(dataA, dataB);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Start()
{
    Osci::Settings::RShift::Set(Chan::A, Osci::Settings::RShift::ZERO);
    Osci::Settings::RShift::Set(Chan::B, Osci::Settings::RShift::ZERO);

    Storage::CreateNewFrame();

    FSMC::WriteToFPGA16(WR::PRED_LO, 0); //-V525
    FSMC::WriteToFPGA16(WR::POST_LO, 0);
    FSMC::WriteToFPGA8(WR::START, 0xff);

    FPGA::HAL::Interrupt::P2P::Enable();

    running = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Stop()
{
    FPGA::HAL::Interrupt::P2P::Disable();

    running = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void StoreOsciSettings()
{
    osci.storedRShift[Chan::A] = SET_RSHIFT(Chan::A);
    osci.storedRShift[Chan::B] = SET_RSHIFT(Chan::B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RestoreOsciSettings()
{
    Osci::Settings::RShift::Set(Chan::A, osci.storedRShift[Chan::A]);
    Osci::Settings::RShift::Set(Chan::B, osci.storedRShift[Chan::B]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::OnPressStart()
{
    return running ? Stop() : Start();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Recorder::IsRunning()
{
    return running;
}
