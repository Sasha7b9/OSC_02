#include "defines.h"
#include "Hardware/Timer.h"
#include "Osci/Osci_win.h"
#include "Settings/ParametersOsci.h"


bool OsciHAL::ReadyPoint()
{
    static uint timeLastRead = 0;   // ����� ���������� ������ �����

    if(TIME_MS - timeLastRead >= TBase::TimePoint(TBase()))
    {
        timeLastRead = TIME_MS;
        return true;
    }

    return false;
}
