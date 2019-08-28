#include "defines.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Memory.h"
#include "Hardware/Beeper.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Settings/Settings.h"
#include "Settings/SettingsMemory.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


using namespace Display::Primitives;

extern const Page pageROM;

const Page * const PageROM::self = (const Page *)&pageROM;

/// ���������� ����� ������ ���������� ��������
static void DrawMemoryMap(int num, bool exist);
/// ���������� ������� ������ ��� �������� ��������
static bool HandlerKey_Internal(KeyEvent event);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Next()
{
    HandlerKey_Internal(KeyEvent(Key::Right, TypePress::Release));
}

static void Draw_Next(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_ARROW_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bNext,
    "���������",
    "������� � ���������� �������",
    &PageROM::self, E_BtV, OnPress_Next, Draw_Next
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Prev()
{
    HandlerKey_Internal(KeyEvent(Key::Left, TypePress::Release));
}

static void Draw_Prev(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_ARROW_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bPrev,
    "����������",
    "������� � ����������� �������",
    &PageROM::self, E_BtV, OnPress_Prev, Draw_Prev
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_Delete()
{
    Display::FuncOnWaitStart("������ ���������� ������", false);
    Memory::DeleteData(NUM_ROM_SIGNAL);
    Display::FuncOnWaitStop();
}

static void Draw_Internal_Delete(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_DELETE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bDelete,                                                                                                                              //--- ������ - ����� �� - ������� ---
    "�������",
    "������� ��������� ������ �� ����������� ������������� ����������",
    &PageROM::self, E_BtV, OnPress_Internal_Delete, Draw_Internal_Delete
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SaveSignalToIntMemory()
{
    // ������� � ��������� DS, DATA_A, DATA_B ������ �� ��� ��� ��������� ��������� ������, � ����������� �� ����, �� ������ ������ ����� � 
    // "������-����� ��"
    //Reader::ReadFromRAM(EXIT_FROM_ROM_TO_RAM ? NUM_RAM_SIGNAL : 0, 0, false);

    if (DS)                                             // ���� ���� ��� ���������
    {
//        Memory::SaveData(NUM_ROM_SIGNAL, DS, IN_A, IN_B);   // �� ��������� ������ �� DS, DATA_A, DATA_B �� ����� NUM_ROM_SIGNAL � ����
        Display::ShowWarning(Warning::SignalIsSaved);
    }
}

static void OnPress_Internal_SaveToMemory()
{
    Display::FuncOnWaitStart("��������� � ������", false);
    SaveSignalToIntMemory();
    Display::FuncOnWaitStop();
}

static void Draw_Internal_SaveToMemory(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSave,                                                                                                                   //--- ������ - ����� �� - ��������� � ������ ---
    "��������� � ������",
    "��������� ������ �� ���������� ������������ ����������",
    &PageROM::self, E_BtV, OnPress_Internal_SaveToMemory, Draw_Internal_SaveToMemory
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnOpenClose_Internal(bool)
{
    MODE_WORK = ModeWork::ROM;
}

static void OnDraw_Internal()
{
    // ������ �������� ��������� ������

    bool exist[MAX_NUM_SAVED_WAVES] = { false };

    Memory::GetDataInfo(exist);

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryMap(i, exist[i]);
    }
}

static void DrawMemoryMap(int num, bool exist)
{
    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 13;
    int width = 12;
    Region(width, 10).Fill(x, y, num == NUM_ROM_SIGNAL ? Color::FLASH_10 : Color::BACK);
    Rectangle(width, 10).Draw(x, y, Color::FILL);
    Color::SetCurrent(num == NUM_ROM_SIGNAL ? Color::FLASH_01 : Color::FILL);

    if (exist)
    {
        Integer(num + 1).ToString(false, 2).Draw(x + 2, y + 1);
    }
    else
    {
        String("\x88").Draw(x + 3, y + 1);
    }
}

static bool HandlerKey_Internal(KeyEvent event)
{
    if (event.type == TypePress::Release || event.type == TypePress::Long)
    {
        Key::E key = event.key;

        Beeper::RegulatorSwitchRotate();

        if (key == Key::Left || key == Key::Down)
        {
            Math::CircleDecrease<int8>((int8 *)&NUM_ROM_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
        }
        else if (key == Key::Right || key == Key::Up)
        {
            Math::CircleIncrease<int8>((int8 *)&NUM_ROM_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
        }
        else
        {
            // ��������� ������ �� ������������
        }

        Color::ResetFlash();
    }

    return true;
}


DEF_PAGE_4( pageROM, // -V641                                                                                                                                     //--- ������ - ����� �� ---
    "����� ��",
    "������� � ����� ������ � ���������� �������",
    &bPrev,         ///< ������ - ����� �� - ����������
    &bNext,         ///< ������ - ����� �� - ���������
    &bSave,         ///< ������ - ����� �� - ���������
    &bDelete,       ///< ������ - ����� �� - �������
    PageName::Memory_Internal,
    &PageMemory::self, nullptr, OnOpenClose_Internal, OnDraw_Internal, HandlerKey_Internal
)
