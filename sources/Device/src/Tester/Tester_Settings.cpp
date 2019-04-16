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

    static const pString names[] =
    {
        "2���",
        "5���",
        "10���",
        "20���",
        "50���",
        "100���",
        "200���",
        "500���",
        "1��",
        "2��",
        "5��",
        "10��",
        "20��"
    };

    pString name = names[value];

    if (name)
    {
        return names[value];
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
