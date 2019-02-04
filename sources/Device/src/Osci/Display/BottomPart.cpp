#include "defines.h"
#include "BottomPart.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Grid.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "Settings/Settings.h"
#include "Utils/Dictionary.h"
#include "Utils/Values.h"
#include <cmath>

#include "FlashDrive/FlashDrive.h"
#include "Hardware/VCP.h"
#include "Hardware/Battery.h"


using namespace Display::Primitives;
using namespace Osci::Measurements;
using namespace Osci::Measurements::Cursors;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::BottomPart::Draw()
{
    int x0 = 273;
    int y0 = Grid::Bottom() + 1;

    Font::SetCurrent(Font::Type::_UGO2);

    // ������
    if (FDrive::IsConnected())
    {
    }

    if (CONNECTED_TO_USB || CABLE_USB_IS_CONNECTED)
    {
        Char(SYMBOL_USB).Draw4SymbolsInRect(x0 + 72, y0 + 2, CONNECTED_TO_USB ? Color::WHITE : Color::FLASH_01);
    }

    Color::SetCurrent(Color::FILL);
    // ������� ��������
    if (!SET_PEAKDET_DIS)
    {
        Char('\x12').Draw(x0 + 38, y0 + 11);
        Char('\x13').Draw(x0 + 46, y0 + 11);
    }

    Battery::Draw(x0, y0);
}
