#include "defines.h"
#include "Hardware/Memory/MemoryRAM.h"
#include "Utils/Math.h"
#include "Osci/StorageOsci.h"


int16 MemoryRAM::currentSignal = 0;



void MemoryRAM::SelectPrevSignal()
{
    Math::CircleDecrease<int16>(&currentSignal, 0, static_cast<int16>(StorageOsci::NumElementsInStorage()) - 1);
}
