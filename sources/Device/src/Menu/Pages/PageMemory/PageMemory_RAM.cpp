#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Osci/Osci.h"
#include "Hardware/RAM.h"
#include "Osci/Osci_Storage.h"


using namespace Display::Primitives;

extern const Page pageRAM;

const Page * const PageRAM::self = (const Page *)&pageRAM;



static void OnPress_Next()
{
    Memory::RAM::SelectNextSignal();
}

static void Draw_Next(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_ARROW_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bNext,                                                                                                                             //--- ������ - ��������� - ��������� ---
    "���������",
    "������� � ���������� �������",
    &PageRAM::self, Item::Active, OnPress_Next, Draw_Next
)


static void OnPress_Prev()
{
    Memory::RAM::SelectPrevSignal();
}

static void Draw_Prev(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_ARROW_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bPrev,                                                                                                                            //--- ������ - ��������� - ���������� ---
    "����������",
    "������� � ����������� �������",
    &PageRAM::self, Item::Active, OnPress_Prev, Draw_Prev
)


static void OnOpenClose_RAM(bool enter)
{
    if (enter)
    {
        RUN_FPGA_BEFORE_SB = FPGA::IsRunning() ? 1U : 0U;
        Osci::Stop(false);
        set.mem.modeWork = ModeWork::RAM;
        Memory::RAM::ResetSignal();
    }
    else
    {
        set.mem.modeWork = ModeWork::Dir;
        if (RUN_FPGA_BEFORE_SB)
        {
            FPGA::OnPressStart();
        }
    }
}

static void AfterDraw_RAM()
{
    int width = 40;
    int height = 10;
    Region(width, height).Fill(Grid::Right() - width, Grid::Top(), Color::BACK);
    Rectangle(width, height).Draw(Grid::Right() - width, Grid::Top(), Color::FILL);
    Integer(Memory::RAM::CurrentSignal() + 1).ToString(false, 3).Draw(Grid::Right() - width + 2, Grid::Top() + 1);
    String("/").Draw(Grid::Right() - width + 17, Grid::Top() + 1);
    Integer(Osci::Storage::NumElementsInStorage()).ToString(false, 3).Draw(Grid::Right() - width + 23, Grid::Top() + 1);
}

static bool HandlerKey_RAM(const KeyEvent &)
{
    /*
    Key::E key = event.key;

    if (DataStorage::NumElementsInStorage() > 1)
    {
        Beeper::RegulatorSwitchRotate();
    }
    if (key == Key::Up || key == Key::Right)
    {
        OnPress_Last_Next();
    }
    else
    {
        OnPress_Last_Prev();
    }
    */

    return false;
}

DEF_PAGE_2( pageRAM, // -V641 // -V1027                                                                                                                          //--- ������ - ��������� ---
    "���������",
    "������� � ����� ������ � ���������� ����������� ���������",
    &bPrev,                        ///< ������ - ��������� - ����������
    &bNext,                        ///< ������ - ��������� - ���������
    PageName::Memory_Last,
    &PageMemory::self, Item::Active, OnOpenClose_RAM, AfterDraw_RAM, HandlerKey_RAM
)
