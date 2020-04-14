#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Settings/Settings.h"
#include <cstring>


/// ����� �������� ���������� �������� ��� ������� �� �������, ��� ���������������� � ����������
static uint16 numSignals[2] = { 0, 0 };


void AveragerOsci::Process(Chan::E ch, const uint8 *dataNew, uint size)
{
    uint8 *_new = const_cast<uint8 *>(dataNew);
    uint16 *av = AVE_DATA(ch);

    if (numSignals[ch] < ENumAverage().Number())
    {
        if (numSignals[ch] == 0)
        {
            for (uint i = 0; i < size; i++)
            {
                av[i] = dataNew[i];
            }
        }
        else
        {
            for(uint i = 0; i < size; i++)
            {
                if(dataNew[i] != VALUE::NONE)
                {
                    av[i] += dataNew[i];
                    *_new++ = static_cast<uint8>(av[i] / (numSignals[ch] + 1));
                }
            }
        }
    }
    else
    {
        uint16 shift = static_cast<uint16>(set.disp.enumAverage);

        for(uint i = 0; i < size; i++)
        {
            av[i] = static_cast<uint16>(av[i] - (av[i] >> shift) + *_new);
            *_new++ = static_cast<uint8>(av[i] >> shift);
        }
    }

    if(numSignals[ch] < NUM_AVE_MAX + 10)
    {
        numSignals[ch]++;
    }
}


void AveragerOsci::SettingChanged()
{
    numSignals[0] = 0;
    numSignals[1] = 0;
}
