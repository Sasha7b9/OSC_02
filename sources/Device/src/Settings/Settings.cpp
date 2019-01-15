#include "defines.h"
#include "Settings.h"
#include "Hardware/EEPROM.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"


using namespace FPGA::SET;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Settings set = Settings::defaultSettings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Load(bool _default)
{
    if(_default || !EEPROM::LoadSettings())
    {
        Reset();
        FPGA::SET::Load();
        uint8 data[1024];
        TransformFromLoad(data);
        Menu::Init();
        FPGA::OnPressStart();
    }

    /*
    uint size = 0;
    AT25160N::ReadData(0, (uint8 *)&size, 4);

    if(size > 0 && size < 1024)
    {
        uint8 *data = (uint8 *)malloc(size);

        AT25160N::ReadData(0, data, size);

        uint crc32 = Hardware::CalculateCRC32((uint)data + 8, size - 8);

        if(crc32 == *((uint *)(data + 4)))
        {
            memcpy(&set, data, size);
        }
        else
        {
            Reset();
        }

        free(data);
    }
    else
    {
        Reset();
    }
    */
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    uint8 data[1024];
    set.TransformForSave(data);
    EEPROM::SaveSettings();

    /*
    SET_SIZE = sizeof(Settings);
    SET_CRC32 = Hardware::CalculateCRC32((uint)((uint8 *)&set + 8), SET_SIZE - 8);

    AT25160N::WriteData(0, (uint8 *)&set, SET_SIZE);
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Reset()
{
    set = defaultSettings;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Range::Name() const
{
    static const struct StructRange
    {
        const char * names[Language::Number];
        StructRange(pString nRU, pString nEN)
        {
            names[Language::RU] = nRU;
            names[Language::EN] = nEN;
        }
    }
    names[Range::Number] =
    {
        StructRange("2��",  "2mV"),
        StructRange("5��",  "5mV"),
        StructRange("10��", "10mV"),
        StructRange("20��", "20mV"),
        StructRange("50��", "50mV"),
        StructRange("0.1�", "0.1V"),
        StructRange("0.2�", "0.2V"),
        StructRange("0.5�", "0.5V"),
        StructRange("1�",   "1V"),
        StructRange("2�",   "2V"),
        StructRange("5�",   "5V"),
        StructRange("10�",  "10V"),
        StructRange("20�",  "20V")
    };

    return names[value].names[LANG];
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString TBase::Name() const
{
    static pString names[TBase::Number][Language::Number] =
    {
        {"2��",     "2ns"},
        {"5��",     "5ns"},
        {"10��",    "10ns"},
        {"20��",    "20ns"},
        {"50��",    "50ns"},
        {"0.1���",  "0.1us"},
        {"0.2���",  "0.2us"},
        {"0.5���",  "0.5us"},
        {"1���",    "1us"},
        {"2���",    "2us"},
        {"5���",    "5us"},
        {"10���",   "10us"},
        {"20���",   "20us"},
        {"50���",   "50us"},
        {"0.1��",   "0.1ms"},
        {"0.2��",   "0.2ms"},
        {"0.5��",   "0.5ms"},
        {"1��",     "1ms"},
        {"2��",     "2ms"},
        {"5��",     "5ms"},
        {"10��",    "10ms"},
        {"20��",    "20ms"},
        {"50��",    "50ms"},
        {"0.1�",    "0.1s"},
        {"0.2�",    "0.2s"},
        {"0.5�",    "0.5s"},
        {"1�",      "1s"},
        {"2�",      "2s"},
        {"5�",      "5s"},
        {"10�",     "10s"}
    };

    return names[value][LANG];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float MaxDBforFFT(FFTmaxDB::E maxDB)
{
    static const float arrayMAX_DB_FOR_FFT[] = {-40.0F, -60.0F, -80.0F};

    return arrayMAX_DB_FOR_FFT[maxDB];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Settings &Settings::GetDefault()
{
    return (Settings &)defaultSettings;
}
