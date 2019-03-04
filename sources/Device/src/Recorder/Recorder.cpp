#include "defines.h"
#include "Display/Display.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/HAL/HAL.h"
#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Settings.h"
#include "Recorder/Recorder_Storage.h"

#include "Hardware/HAL/HAL_PIO.h"
#include "Data/Heap.h"


using namespace HAL::ADDRESSES::FPGA;

using HAL::FSMC;


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
void Recorder::ReadPoint()
{
    if (!IsRunning())
    {
        return;
    }

    if(::HAL::PIO::Read(::HAL::PIO::Port::_G, ::HAL::PIO::Pin::_1))
    {
        BitSet16 dataA(FSMC::ReadFromFPGA(RD::DATA_A), FSMC::ReadFromFPGA(RD::DATA_A + 1));
        BitSet16 dataB(FSMC::ReadFromFPGA(RD::DATA_B), FSMC::ReadFromFPGA(RD::DATA_B + 1));

        if (Recorder::Storage::CurrentFrame()->Size() + sizeof(Storage::Point) < Heap::Size())
        {
            Recorder::Storage::CurrentFrame()->AddPoint(dataA, dataB);
        }
        else
        {
            Stop();
        }
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

    running = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Stop()
{
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
