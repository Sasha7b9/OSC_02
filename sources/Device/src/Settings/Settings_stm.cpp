#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory/ROM.h"
#include "FPGA/ContextOsci.h"
#include "Osci/Osci.h"



void Settings::Load(bool _default)
{
    if(_default || !ROM::Settings::Load())
    {
        Reset();
        Osci::Init();
        Menu::Init();
        ContextOsci::OnPressStart();
    }
}


void Settings::Save()
{
    ROM::Settings::Save();
}
