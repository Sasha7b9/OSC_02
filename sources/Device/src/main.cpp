#include "defines.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


int main(void)
{
    Device::Init();
    
    while (1)
    {
        FDrive::Update();

        static bool alreadySave = false;

        if (FDrive::IsConnected() && !alreadySave)
        {
            ROM::Settings::Erase();
            alreadySave = true;
        }
    }
}
