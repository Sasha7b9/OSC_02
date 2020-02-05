#include "defines.h"
#include "device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/HAL/HAL.h"
#include "Test/Test.h"


int main(void)
{
    Device::Init();

    AT25160N::Init();
    
    Test::Run();
  
    while (1)
    {
        Device::Update();

        LOG_WRITE("%d �� �� %d ��", 512, (uint)(HAL_FSMC::TestTimeRAM(512) * 1000));
    }
}
