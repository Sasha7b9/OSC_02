#include "defines.h"
#include "Osci_Averager.h"
#include "Display/Display_Types.h"
#include "Settings/Settings.h"
#include "Osci.h"
#include "Data/Reader.h"
#include "FPGA/FPGA.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ����� �������� ���������� �������� ��� ������� �� �������, ��� ���������������� � ����������
static uint16 numSignals[2] = { 0, 0 };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Averager::Process(Chan::E ch, const uint8 *dataNew, int size)
{
    /*
        � ������ ������������� � ���������� ���� ������������ ������ �� ������, ������ �������. ������ ����� ������ ��� ���������� �� ����������� ����������� �������.
        ��� ����� ����� ������� ������� ������, � ������� �������� �� �����, ������� ������� � ������ ��������.
    */

    uint16 numAve = (uint16)ENUM_AVE;

    int index = 0;
    int step = 1;

    if (Osci::InModeRandomizer())
    {
        Osci::StructReadRand str = Osci::GetInfoForReadRand();

        index = str.posFirst;
        step = str.step;
    }

    uint8 *_new = (uint8 *)dataNew + index;
    uint16 *av = AVE_DATA(ch);

    if (numSignals[ch] <= NUM_AVE)
    {
        if (numSignals[ch] == 0)
        {
            std::memcpy(AVE_DATA(ch), _new, (uint)size);
        }

        for (int i = index; i < size; i += step)
        {
            av[i] = (uint16)(av[i] - (av[i] >> numAve));

            av[i] += *_new;

            _new += step;
        }
    }
    else
    {
        for (int i = index; i < size; i += step)
        {
            av[i] = (uint16)(av[i] - (av[i] >> numAve));

            av[i] += *_new;

            *_new = (uint8)(av[i] >> numAve);

            _new += step;
        }
    }

    if (numSignals[ch] < NUM_AVE_MAX + 10)
    {
        numSignals[ch]++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Averager::Prepare()
{
    numSignals[0] = numSignals[1] = 0;
}
