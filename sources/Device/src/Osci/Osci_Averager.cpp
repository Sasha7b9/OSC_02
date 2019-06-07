#include "defines.h"
#include "Osci_Averager.h"
#include "Display/Display_Types.h"
#include "Settings/Settings.h"
#include "Osci.h"
#include "Data/Reader.h"
#include "FPGA/FPGA.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���� 1, ��� ��������������� ������ ��� ���������� - ��� ������ ����� ��������� ������
/// ��� 0 - ����� 1, ��� 1 - ����� 2
static uint8 isEmpty = 0xff;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Averager::Process(Chan::E ch, const uint8 *dataNew, int size)
{
    /*
        � ������ ������������� � ���������� ���� ������������ ������ �� ������, ������ �������. ������ ����� ������ ��� ���������� �� ����������� ����������� �������.
        ��� ����� ����� ������� ������� ������, � ������� �������� �� �����, ������� ������� � ������ ��������.
    */

    if ((ch == Chan::A) && (isEmpty & 0x01))
    {
        isEmpty &= ~0x01;            // �������� ��� 0 - ����� 1

        std::memcpy(AVE_DATA(ch), dataNew, (uint)size);

        LOG_WRITE("������� 1");

        return;
    }

    if ((ch == Chan::A) && (isEmpty & 0x02))
    {
        isEmpty &= ~0x02;           // �������� ��� 1 - ����� 2

        std::memcpy(AVE_DATA(ch), dataNew, (uint)size);

        LOG_WRITE("������� 2");

        return;
    }


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

    for (int i = index; i < size; i += step)
    {
        av[i] = (uint16)(av[i] - (av[i] >> numAve));

        av[i] += *_new;

        *_new = (uint8)(av[i] >> numAve);

        _new += step;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Averager::Prepare()
{
    isEmpty = 0xff;
}
