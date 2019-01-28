#include "defines.h"
#include "FPGA/FPGA_Math.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Tester::Settings::Scale::ToString() const // -V2506
{
    if (Chan(ch).IsA())
    {
        return Osci::Settings::Range((Osci::Settings::Range::E)value).ToString(Divider::_1);
    }

    static const pString names[][Language::Size] =
    {
        "2���",     "2uA",
        "5���",     "5u�",
        "10���",    "10u�",
        "20���",    "20u�",
        "50���",    "50u�",
        "100���",   "100u�",
        "200���",   "200u�",
        "500���",   "500u�",
        "1��",      "1m�",
        "2��",      "2m�",
        "5��",      "5m�",
        "10��",     "10m�",
        "20��",     "20m�"
    };

    pString name = names[value][LANG];

    if (name)
    {
        return names[value][LANG];
    }
    else
    {
        LOG_ERROR("������ �� ��������");
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Tester::Settings::Shift::ToString(Scale::E scale) // -V2506
{
    if (ch == Chan::A)
    {
        return Osci::Settings::RShift::ToString(shift, (Osci::Settings::Range::E)scale, Divider::_1);
    }

    float shiftAbs = FPGA::Math::RShift2Abs(shift, (Osci::Settings::Range::E)scale) * 1e-3F;

    return Current(shiftAbs).ToString();
}
