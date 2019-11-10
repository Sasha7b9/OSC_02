#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "FPGA/ContextOsci.h"
#include "Hardware/Memory/MemoryRAM.h"
#include "Menu/MenuItems.h"
#include "Osci/Osci.h"
#include "Osci/StorageOsci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


static void OnPress_Next()
{
    Math::CircleIncrease<int16>(&MemoryRAM::currentSignal, 0, static_cast<int16>(StorageOsci::NumElementsInStorage()) - 1);
}

static void Draw_Next(int x, int y)
{
    Char(SymbolUGO2::ARROW_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bNext,                                                                                                                             //--- ������ - ��������� - ��������� ---
    "���������",
    "������� � ���������� �������",
    &PageRAM::self, Item::Active, OnPress_Next, Draw_Next
)


static void OnPress_Prev()
{
    MemoryRAM::SelectPrevSignal();
}

static void Draw_Prev(int x, int y)
{
    Char(SymbolUGO2::ARROW_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
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
        RUN_FPGA_BEFORE_SB = Osci::IsRunning() ? 1U : 0U;
        Osci::Stop(false);
        set.mem.modeWork = ModeWork::RAM;
        MemoryRAM::currentSignal = 0;
    }
    else
    {
        set.mem.modeWork = ModeWork::Dir;
        if (RUN_FPGA_BEFORE_SB)
        {
            ContextOsci::OnPressStart();
        }
    }
}

static void AfterDraw_RAM()
{
    int width = 40;
    int height = 10;
    Region(width, height).Fill(Grid::Right() - width, Grid::Top(), Color::BACK);
    Rectangle(width, height).Draw(Grid::Right() - width, Grid::Top(), Color::FILL);
    Integer(MemoryRAM::currentSignal + 1).ToString(false, 3).Draw(Grid::Right() - width + 2, Grid::Top() + 1);
    String("/").Draw(Grid::Right() - width + 17, Grid::Top() + 1);
    Integer(StorageOsci::NumElementsInStorage()).ToString(false, 3).Draw(Grid::Right() - width + 23, Grid::Top() + 1);
}

static bool HandlerKey_RAM(const KeyEvent &)
{
    return false;
}

DEF_PAGE_2( pRAM,                                                                                                                                                //--- ������ - ��������� ---
    "���������",
    "������� � ����� ������ � ���������� ����������� ���������",
    &bPrev,
    &bNext,
    PageName::Memory_Last,
    &PageMemory::self, Item::Active, Page::NormalTitle, OnOpenClose_RAM, AfterDraw_RAM, HandlerKey_RAM
)

const Page *const PageRAM::self = static_cast<const Page *>(&pRAM);
