#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA.h"
#include "FPGA_HAL.h"
#include "FPGA_Settings.h"
#include "AD9286.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Values.h"
#include <stdlib.h>

#include "Hardware/HAL/HAL.h"
#include "Osci/Osci_Storage.h"


using namespace FPGA::HAL::GPIO;

using Hardware::AD9286;


extern bool givingStart;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class ADC3_
    {
    public:
        static void Init()
        {
            ::HAL::ADC3_::Init();
        }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class DataAccessor
    {
    public:
        static uint8 *DataA(Osci::Data *data)
        {
            return data->dataA;
        }
        static uint8 *DataB(Osci::Data *data)
        {
            return data->dataB;
        }
    };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{
    givingStart = false;

    HAL::GPIO::Init();

    SetPin(Pin::SPI3_CS1);
    SetPin(Pin::SPI3_CS2);

    ResetPin(Pin::SPI3_SCK);
    ResetPin(Pin::SPI3_DAT);

    AD9286::Init();

    ADC3_::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void AverageData(const uint8 *dataNew, const uint8 *dataOld, int size, float k)
{
    // ������� ���������� ������:
    // new = new * k + old * (1 - k)

    uint8 *_new = (uint8 *)dataNew;
    const uint8 *_old = dataOld;

    for (int i = 0; i < size; i++)
    {
        *_new = (uint8)((*_new * k) + (*_old * (1.0F - k)));

        _new++;
        _old++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadData()
{
    Osci::Data *data = Osci::Storage::PrepareForNewData();

    ReadDataChanenl(Chan::A, DataAccessor::DataA(data));
    ReadDataChanenl(Chan::B, DataAccessor::DataB(data));

    if (ENUM_AVE != Display::ENumAverage::_1)               // ���� �������� ����������
    {
        Osci::Data *last = Osci::Storage::GetData(0);
        Osci::Data *prev = Osci::Storage::GetData(1);

        if (prev && last)
        {
            const DataSettings *setLast = last->Settings();
            const DataSettings *setPrev = prev->Settings();

            float k = 1.0F / NUM_AVE;

            if (setLast->Equals(*setPrev))
            {
                if (ENABLED_A(setLast))
                {
                    AverageData(last->DataA(), prev->DataA(), setLast->SizeChannel(), k);
                }
                if (ENABLED_B(setPrev))
                {
                    AverageData(last->DataB(), prev->DataB(), setLast->SizeChannel(), k);
                }
            }
        }
    }
}
