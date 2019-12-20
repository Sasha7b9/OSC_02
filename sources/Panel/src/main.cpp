#include "defines.h"
#include "common/Decoder_d.h"
#include "common/Transceiver.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include <stdlib.h>



int main()
{
    CPU::Init();
    Display::Init();
    Keyboard::Init();
    Transceiver::Init();

    while(1)
    {
        static uint prevTime = 0;
        
        Transceiver::Receive();

        if(TIME_MS - prevTime > 2)
        {
            Keyboard::Update();
            prevTime = TIME_MS;
        }
    }
}
