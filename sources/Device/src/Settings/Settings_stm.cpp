#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Osci/Osci.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Load(bool _default)
{
    if(_default || !Memory::LoadSettings())
    {
        Reset();
        Osci::Init();
        Menu::Init();
        FPGA::OnPressStart();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    volatile int tShift = SET_TSHIFT;

    Memory::SaveSettings();

    tShift = SET_TSHIFT;
}
