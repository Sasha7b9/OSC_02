#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/ContextRecorder.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Recorder/Recorder.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


/// ��������� ������������ ����� ��������� � ����� ������������
struct StateOsci
{
    /// �������� �� ��������� ��� ������� � ������� �������
    int16 storedRShift[2];
};



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



void Recorder::Init()
{
    StoreOsciSettings();

    ContextRecorder::LoadRegUPR();
    Range::LoadBoth();
    TrigInput().Load();
    Recorder::ScaleX::Load();
    TShift().Load();
    Osci::LoadHoldfOff();
    StorageRecorder::Init();

    running = false;

    initialized = true;

    FPGA::GiveStart(0, 0);
}


void Recorder::DeInit()
{
    if (initialized)
    {
        Stop();

        RestoreOsciSettings();

        initialized = false;
    }
}


void Recorder::ReadPoint()
{
    if (HAL_PIO::Read(PIN_P2P))
    {
        if (StorageRecorder::LastRecord()->FreeMemory() > 4)
        {
            HAL_BUS::FPGA::SetAddrData(RD::DATA_A, RD::DATA_A + 1);
            BitSet16 dataA(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());

            HAL_BUS::FPGA::SetAddrData(RD::DATA_B, RD::DATA_B + 1);
            BitSet16 dataB(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());

            StorageRecorder::LastRecord()->AddPoints(dataA, dataB);
        }
        else
        {
            Stop();
        }
    }
}


void Recorder::Start()
{
    StorageRecorder::CreateNewRecord();

    running = true;
}


void Recorder::Stop()
{
    running = false;
}


static void StoreOsciSettings()
{
    osci.storedRShift[Chan::A] = RShift(Chan::A);
    osci.storedRShift[Chan::B] = RShift(Chan::B);
}


static void RestoreOsciSettings()
{
    RShift(Chan::A).Set(osci.storedRShift[Chan::A]);
    RShift(Chan::B).Set(osci.storedRShift[Chan::B]);
}


void Recorder::OnPressStart()
{
    if (Menu::OpenedItem() == const_cast<Page *>(PageRecorder::self))
    {
        if (running)
        {
            Stop();
            Keyboard::Unlock();
        }
        else
        {
            static const Key::E keys[] = { Key::F4, Key::Start, Key::None };
            Start();
            Keyboard::Lock(keys);
        }
    }
    else
    {
        Display::ShowWarning("��������� �� �������� �����������");
    }
}


bool Recorder::IsRunning()
{
    return running;
}


void Recorder::ScaleX::Change(int delta)
{
    if (!Recorder::IsRunning())
    {
        if (delta > 0)
        {
            ::Math::LimitationIncrease<uint8>(reinterpret_cast<uint8 *>(&Recorder::ScaleX::Ref()), static_cast<uint8>(Recorder::ScaleX::Count - 1));
        }
        else
        {
            ::Math::LimitationDecrease<uint8>(reinterpret_cast<uint8 *>(&Recorder::ScaleX::Ref()), 0);
        }

        Load();
    }
}


pString Recorder::ScaleX::ToString()
{
    static const struct StructScaleX
    {
        const char *name;
        StructScaleX(pString nRU)
        {
            name = nRU;
        };
    }
    scales[Count] =
    {
        StructScaleX("0.1\x10�"),
        StructScaleX("0.2\x10�"),
        StructScaleX("0.5\x10�"),
        StructScaleX("1\x10�"),
        StructScaleX("2\x10�"),
        StructScaleX("5\x10�"),
        StructScaleX("10\x10�")
    };

    return scales[Ref()].name;
}


uint Recorder::ScaleX::BytesToSec() const
{
    static const struct StructBytes
    {
        uint value;
        StructBytes(uint v) : value(v) {};
    }
    bytes[Count] =
    {
        800,
        400,
        160,
        80,
        40,
        16,
        8
    };

    return bytes[Ref()].value;
}


uint Recorder::ScaleX::TimeForPointMS()
{
    static const struct StructTime
    {
        uint value;
        StructTime(uint v) : value(v) {};
    }
    bytes[Count] =
    {
        5,
        10,
        25,
        50,
        100,
        250,
        500
    };

    return bytes[Ref()].value;
}

#ifdef WIN32
#pragma warning(disable:4310)
#endif


void Recorder::ScaleX::Load()
{
    static const uint8 values[Count] =
    {
        BIN_U8(01010110),  // -V2501  // 100ms  
        BIN_U8(01010111),  // -V2501  // 200ms  
        BIN_U8(01011001),  // -V2501  // 500ms  
        BIN_U8(01011010),  // -V2501  // 1s     
        BIN_U8(01011011),  // -V2501  // 2s     
        BIN_U8(01011101),  // -V2501  // 5s     
        BIN_U8(01011110)   // -V2501  // 10s
    };

    HAL_BUS::FPGA::Write8(WR::TBASE, values[Ref()]);

    if (Recorder::IsRunning())
    {
        Recorder::Stop();
        Recorder::Start();
    }
}


bool Recorder::IsEnabledA()
{
    return set.rec.enA;
}


bool Recorder::IsEnabledB()
{
    return set.rec.enB;
}


bool Recorder::IsEnabledSensor()
{
    return set.rec.enSensor;
}


Recorder::TypeMemory::E &Recorder::TypeMemory::Ref()
{
    return set.rec.typeMemory;
}


Recorder::ScaleX::E &Recorder::ScaleX::Ref()
{
    return set.rec.scaleX;
}
