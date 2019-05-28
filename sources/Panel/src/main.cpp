#include "defines.h"
#include "Transceiver.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Utils/DecoderPanel.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include <stdlib.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    CPU::Init();
    Display::Init();
    Keyboard::Init();
    Transceiver::Init(EmptyFuncVV);

    while(1)
    {
        Transceiver::Update();
    }
}
