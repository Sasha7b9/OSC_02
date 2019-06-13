#include "defines.h"
#include "Command.h"
#include "Averager.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int enumAve = 0;

struct StructX
{
    /// ���������� ������ ������ ����
    uint data[TESTER_NUM_POINTS];
    /// ���������� ���������, ����������� � data
    int cout;
};

struct StructY
{
    /// ���������� ������ ������ ����
    uint16 data[TESTER_NUM_POINTS];
    /// ���������� ���������, ����������� � data
    int cout;
};

static StructX dataX[5] __attribute__((section("CCM_DATA")));
static StructY dataY[5] __attribute__((section("CCM_DATA")));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Averager::Tester::SetCount(int count)
{
    if (enumAve != count)
    {
        for (int step = 0; step < 5; step++)
        {
            for (int i = 0; i < TESTER_NUM_POINTS; i++)
            {
                dataX[step].data[i] = 0;
                dataY[step].data[i] = 0;
            }
        }
    }

    enumAve = count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Averager::Tester::SetDataX(uint16 *data, int step)
{
    if (enumAve == 0)
    {
        return;
    }
    
    uint *ave = &dataX[step].data[0];

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave[i] = (uint)(ave[i] - (ave[i] >> enumAve) + *data);
        *data = (uint16)(ave[i] >> enumAve);
        data++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Averager::Tester::SetDataY(uint8 *y, int step)
{
    if(step != 0)
    {
        step = 0;
    }
    
    if (enumAve == 0)
    {
        return;
    }
    
    uint16 *ave = &dataY[step].data[0];
    
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave[i] = (uint16)(ave[i] - (ave[i] >> enumAve) + y[i]);
        y[i] = (uint8)(ave[i] >> enumAve);
    }
}
