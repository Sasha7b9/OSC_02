#include "defines.h"
#include "Osci.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include <cstring>


/// ����� �������� ���������� �������� ��� ������� �� �������, ��� ���������������� � ����������
static uint16 numSignals[2] = { 0, 0 };


/// ������ ������� ������ � RAM. ���������� false, ���� ����� �� ������ - ������ ��������� ��� ������������ ������ � ������
static bool ExchangeDatas(uint numInRAM0, uint numInRAM1);
/// ����������� ������ d0 � d1, � d1 � d0
static void ExchangeData(uint8 *d0, uint8 *d1, uint numPoints);


void AveragerOsci::Process(Chan::E ch, const uint8 *dataNew, uint size)
{
    /*
        � ������ ������������� � ���������� ���� ������������ ������ �� ������, ������ �������. ������ ����� ������ ��� ���������� �� ����������� ����������� �������.
        ��� ����� ����� ������� ������� ������, � ������� �������� �� �����, ������� ������� � ������ ��������.
    */

    if(!ExchangeDatas(0, 1))                                            // ������ ����� �������� ��������� ��������� ������ �������������� � RAM, � ���������� - ����������
    {
        return;
    }

    uint16 numAve = static_cast<uint16>(set.disp.ENumAverage);

    uint index = 0;
    uint step = 1;

    if (Osci::InModeRandomizer())
    {
        Osci::StructReadRand str = Osci::GetInfoForReadRand();

        index = str.posFirst;
        step = str.step;
    }

    uint8 *data = const_cast<uint8 *>(dataNew) + index;
    uint16 *av = AVE_DATA(ch);

    /// \todo ����� ����������� - ����� � AVE_DATA ������� ���� ��� 8� * 16����

    if (numSignals[ch] < set.disp.ENumAverage)
    {
        if (numSignals[ch] == 0)
        {
            std::memset(AVE_DATA(ch), 0, static_cast<uint>(size) * 2);

            for (uint i = 0; i < size; i++)
            {
                av[i] = dataNew[i];
            }
        }
        else
        {
            for (uint i = index; i < size; i += step)
            {
                av[i] += *data;

                data += step;
            }
        }
    }
    else
    {
        for (uint i = index; i < size; i += step)
        {
            av[i] = static_cast<uint16>(av[i] - (av[i] >> numAve) + *data);

            *data = static_cast<uint8>(av[i] >> numAve);

            data += step;
        }
    }

    if (numSignals[ch] < NUM_AVE_MAX + 10)
    {
        numSignals[ch]++;
    }
}


void AveragerOsci::SettingChanged()
{
    numSignals[0] = numSignals[1] = 0;
}


void AveragerOsci::Draw()
{
    if (set.disp.ENumAverage > 1 && numSignals[0] < set.disp.ENumAverage)
    {
        int height = 10;

        Rectangle(Grid::Width(), height).Draw(Grid::Left(), Grid::Top(), Color::GRID);

        Region(static_cast<int>(Grid::Width() * static_cast<float>(numSignals[0]) / set.disp.ENumAverage), height).Fill(Grid::Left(), Grid::Top());
    }
}


static bool ExchangeDatas(uint numInRAM0, uint numInRAM1)
{
    if(RAM::NumberDatas() < 2)
    {
        return false;
    }

    DataSettings *ds0 = RAM::Read(numInRAM0);
    DataSettings *ds1 = RAM::Read(numInRAM1);

    if(!ds0->Equals(*ds1))
    {
        return false;
    }

    if(ENABLED_A(ds0) && ENABLED_A(ds1))
    {
        ExchangeData(ds0->dataA, ds1->dataA, ds0->SizeChannel());
    }

    if(ENABLED_B(ds0) && ENABLED_B(ds1))
    {
        ExchangeData(ds0->dataB, ds1->dataB, ds0->SizeChannel());
    }

    return true;
}


static void ExchangeData(uint8 *d0, uint8 *d1, uint numPoints)
{
    static const uint SIZE_BLOCK = 32;

    uint8 buffer[SIZE_BLOCK];

    while(numPoints)
    {
        std::memcpy(buffer, d0, SIZE_BLOCK);
        std::memcpy(d0, d1, SIZE_BLOCK);
        std::memcpy(d1, buffer, SIZE_BLOCK);

        numPoints -= SIZE_BLOCK;
    }
}
