#include "defines.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Tables.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Memory.h"
#include "Hardware/Beeper.h"
#include "FlashDrive/FileManager.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/Definition.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include "Utils/CommonFunctions.h"

#include "Osci/Display/PainterData.h"
#include "Osci/Osci.h"


using namespace Display::Primitives;
using namespace Osci::Settings;

extern const PageBase pageMemory;
extern const PageBase ppDrive;
extern const PageBase pppDrive_Manager;
extern const PageBase pppDrive_Mask;
extern const PageBase pSetName;

const Page * const PageMemory::self = (const Page *)&pageMemory;
const Page * const PageSetName::self = (const Page *)&pSetName;
const Page * const PageDrive::self = (const Page *)&ppDrive;
const Page * const PageDrive::PageManager::self = (const Page *)&pppDrive_Manager;
const Page * const PageDrive::PageMask::self = (const Page *)&pppDrive_Mask;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawSetMask();  // ��� ������� ������, ����� ������ ����� ������� �����.
static void DrawFileMask(int x, int y);
static void DrawSetName();  // ��� ������� ������, ����� ����� ������ ��� ����� ��� ����������

//static void SaveSignalToIntMemory();
       void OnMemExtSetMaskNameRegSet(int angle, int maxIndex);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Points()
{
    return SET_PEAKDET_DIS;
}

void PageMemory::OnChanged_Points(bool active)
{
    // ���� ������� ������� ��������, �� �� ����� ����������� ������
    if (SET_PEAKDET_EN && !active)
    {
        Display::ShowWarning(Warning::WrongModePeackDet);
        return;
    }

    Osci::OnChangedPoints();
}

static pString namesLengthMemory[] = {"512", "1k", "2k", "4k", "8k", "16k", "32k"};

DEF_CHOICE_5( cPoints,                                                                                                                                               //--- ������ - ����� ---
    "����� ������",
    "����� ���������� �������� ��� ����������� ��������. ��� ���������� ���������� �������� ����������� ���������� ���������� � ������ ��������.",
    namesLengthMemory[0],
    namesLengthMemory[1],
    namesLengthMemory[2],
    namesLengthMemory[3],
    namesLengthMemory[4],
    FPGA_ENUM_POINTS,
    &PageMemory::self, IsActive_Points, PageMemory::OnChanged_Points, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Drive_Manager_Tab(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TAB).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON( bDrive_Manager_Tab,                                                                                                             //--- ������ - ����� �� - ������� - Tab ---
    "Tab",
    "������� ����� ���������� � �������",
    &PageDrive::PageManager::self, 0, FileManager::PressSB_Tab, Draw_Drive_Manager_Tab
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Drive_Manager_LevelUp(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x48').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON( bDrive_Manager_LevelUp,                                                                                           //--- ������ - ����� �� - ������� - ����� �� �������� ---
    "����� �� ��������",
    "������� � ������������ �������",
    &PageDrive::PageManager::self, 0, FileManager::PressSB_LevelUp, Draw_Drive_Manager_LevelUp
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Drive_Manager_LevelDown(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x4a').Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON( bDrive_Manager_LevelDown,                                                                                           //--- ������ - ����� �� - ������� - ����� � ������� ---
    "����� � �������",
    "������� � ��������� �������",
    &PageDrive::PageManager::self, 0, FileManager::PressSB_LevelDown, Draw_Drive_Manager_LevelDown
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Drive_Manager()
{
    return FDrive::IsConnected();
}

void PageMemory::OnPress_Drive_Manager(bool)
{
    if (FDrive::IsConnected())
    {
        FDrive::Mount();
        Display::SetDrawMode(Display::DrawMode::Auto, FileManager::Draw);
        FM_NEED_REDRAW = FM_REDRAW_FULL;
    }
    else
    {
        Display::ShowWarning(Warning::WarnNeedForFlashDrive);
    }
}

/*
DEF_PAGE_SB(        pppDrive_Manager,                                                                               // ������ - ����� �� - ������� ///
    "�������", "DIRECTORY",
    "��������� ������ � �������� ������� ������������� ����������",
    "Provides access to the file system of the connected drive",
    &bDrive_Manager_Exit,       // ������ - ����� �� - ������� - �����
    &bDrive_Manager_Tab,        // ������ - ����� �� - ������� - Tab
    0,
    0,
    &bDrive_Manager_LevelUp,    // ������ - ����� �� - ������� - ����� �� ��������
    &bDrive_Manager_LevelDown,  // ������ - ����� �� - ������� - ����� � �������
    Page::Name::SB_Memory_Drive_Manager, &ppDrive, IsActive_Drive_Manager, PageMemory::OnPress_Drive_Manager, FuncDrawPage, FileManager::RotateRegSet
)
*/

DEF_PAGE_3( pppDrive_Manager, // -V641                                                                                                                  //--- ������ - ����� �� - ������� ---
    "�������",
    "��������� ������ � �������� ������� ������������� ����������",
    &bDrive_Manager_Tab,        // ������ - ����� �� - ������� - Tab
    &bDrive_Manager_LevelUp,    // ������ - ����� �� - ������� - ����� �� ��������
    &bDrive_Manager_LevelDown,  // ������ - ����� �� - ������� - ����� � �������
    Page::Name::SB_Memory_Drive_Manager,
    &PageDrive::self, IsActive_Drive_Manager, PageMemory::OnPress_Drive_Manager, 0, FileManager::HandlerKey
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDrive_Name,                                                                                                                            //--- ������ - ����� �� - ��� ����� ---
    "��� �����"
    ,
    "����� ����� ������������ ������ ��� ���������� �� ������� ����������:\n"
    "\"�� �����\" - ����� ��������� ������������� �� ������� �������� ����� (����. ����� ����),\n"
    "\"�������\" - ������ ��� ��� ����� ����� �������� �������"
    ,
    "�� �����",
    "�������",
    FILE_NAMING_MODE,
    &PageDrive::self, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Drive_Mask_Delete()
{
    FILE_NAME_MASK[0] = '\0';
}

static void Draw_Delete(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_DELETE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON( bDrive_Mask_Delete,                                                                                                           //--- ������ - ����� �� - ����� - ������� ---
    "�������",
    "������� ��� �������� �������",
    &PageDrive::PageMask::self, 0, OnPress_Drive_Mask_Delete, Draw_Delete
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Drive_Mask_Backspace()
{
    int size = (int)std::strlen(FILE_NAME_MASK);
    if (size > 0)
    {
        if (size > 1 && FILE_NAME_MASK[size - 2] == 0x07)
        {
            FILE_NAME_MASK[size - 2] = '\0';
        }
        else
        {
            FILE_NAME_MASK[size - 1] = '\0';
        }
    }
}

static void Draw_Backspace(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_BACKSPACE).Draw(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON( bDrive_Mask_Backspace,                                                                                                      //--- ������ - ����� �� - ����� - Backspace ---
    "Backspace",
    "������� ��������� �������� ������",
    &PageDrive::PageMask::self, 0, OnPress_Drive_Mask_Backspace, Draw_Backspace
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Drive_Mask_Insert()
{
    int index = INDEX_SYMBOL;
    int size = (int)std::strlen(FILE_NAME_MASK);
    if (size == MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        return;
    }
    if (index < 0x41)
    {
        FILE_NAME_MASK[size] = Tables::Get(index)[0];
        FILE_NAME_MASK[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;
        if (index == 0x07)  // ��� %nN - ��������� ������
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (FILE_NAME_MASK[size - 1] >= 0x30 && FILE_NAME_MASK[size - 1] <= 0x39) // ���� ����� ������� �����
                {
                    FILE_NAME_MASK[size] = FILE_NAME_MASK[size - 1] - 0x30;
                    FILE_NAME_MASK[size - 1] = 0x07;
                    FILE_NAME_MASK[size + 1] = '\0';
                }
            }
        }
        else
        {
            FILE_NAME_MASK[size] = (char)index;
            FILE_NAME_MASK[size + 1] = '\0';
        }
    }
}

static void Draw_Insert(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_INSERT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON( bDrive_Mask_Insert,                                                                                                          //--- ������ - ����� �� - ����� - �������� ---
    "��������",
    "��������� ��������� ������",
    &PageDrive::PageMask::self, 0, OnPress_Drive_Mask_Insert, Draw_Insert
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDrive_SaveAs,                                                                                                                      //--- ������ - ����� �� - ��������� ��� ---
    "��������� ���"
    ,
    "���� ������ ������� \"�����������\", ������ ����� ������� � ������� �������� � ����������� ����� � ����������� BMP\n"
    "���� ������ ������� \"�����\", ������ ����� ������� � ������� �������� � ��������� ���� � ����� � ���������� TXT"
    ,
    "�����������",
    "�����",
    MODE_SAVE,
    &PageDrive::self, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDrive_ModeBtnMemory,                                                                                                               //--- ������ - ����� �� - ��� �� ������ ---
    "��� �� ������",
    "",
    "����",
    "����������",
    MODE_BTN_MEMORY,
    &PageDrive::self, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Drive_Mask()
{
    return FILE_NAMING_MODE_MASK;
}

static void OnPress_Drive_Mask(bool)
{
    Display::SetAddDrawFunction(DrawSetMask);
}

static void DrawSetMask()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::Top() + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (Tables::Get(index)[0] != ' ')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (Tables::Get(index)[0] != 'a')
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (Tables::Get(index)[0] != '%')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    // ������ ������ �����������
    position = 0;
    while (index < (Tables::Size() / 4))
    {
        Tables::DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static pString strings[] =
    {
        "%y - ���, %m - �����, %d - ����",
        "%H - ����, %M - ������, %S - �������",
        "%nN - ���������� �����, ���",
        "n - ����������� ���������� ������ ��� N"
    };

    deltaY--;
    Color::FILL.SetAsCurrent();
    for (int i = 0; i < (int)sizeof(strings) / 4; i++)
    {
        String(strings[i]).Draw(x0 + deltaX, y0 + 100 + deltaY * i);
    }
}

static void DrawFileMask(int x, int y)
{
    char *ch = FILE_NAME_MASK;

    Color::FILL.SetAsCurrent();
    while (*ch != '\0')
    {
        if (*ch >= 32)
        {
            Char(*ch).Draw(x, y);
        }
        else
        {
            if (*ch == 0x07)
            {
                x = Char('%').Draw(x, y);
                x = Char((char)(0x30 | *(ch + 1))).Draw(x, y);
                x = Char('N').Draw(x, y);
                ch++;
            }
            else
            {
                x = String(Tables::Get(*ch + 0x40)).Draw(x, y);
            }
        }
        ch++;
    }
    Region(5, 8).Fill(x, y, Color::FLASH_10);
}

static bool HandlerKey_Drive_Mask(KeyEvent event)
{
    Key::E key = event.key;

    int angle = (key == Key::Up || key == Key::Right) ? 1 : -1;

    OnMemExtSetMaskNameRegSet(angle, Tables::Size() / 4);

    return true;
}


/*
DEF_PAGE_SB( pppDrive_Mask,                                                                                                                               //--- ������ - ����� �� - ����� ---
    "�����", "MASK",
    "����� ����� ����� ��� ��������������� ���������� ������",
    "Input mode mask for automatic file naming",
    &bDrive_Mask_Exit,      // ������ - ����� �� - ����� - �����
    &bDrive_Mask_Delete,    // ������ - ����� �� - ����� - �������
    0,
    0,
    &bDrive_Mask_Backspace, // ������ - ����� �� - ����� - Backspace
    &bDrive_Mask_Insert,    // ������ - ����� �� - ����� - ��������
    Page::Name::SB_Memory_Drive_Mask, &ppDrive, IsActive_Drive_Mask, OnPress_Drive_Mask, FuncDrawPage, OnRegSet_Drive_Mask
)
*/


DEF_PAGE_3( pppDrive_Mask, // -V641                                                                                                                       //--- ������ - ����� �� - ����� ---
    "�����",
    "����� ����� ����� ��� ��������������� ���������� ������",
    &bDrive_Mask_Delete,    // ������ - ����� �� - ����� - �������
    &bDrive_Mask_Backspace, // ������ - ����� �� - ����� - Backspace
    &bDrive_Mask_Insert,    // ������ - ����� �� - ����� - ��������
    Page::Name::SB_Memory_Drive_Mask,
    &PageDrive::self, IsActive_Drive_Mask, OnPress_Drive_Mask, 0, HandlerKey_Drive_Mask
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDrive_Autoconnect,                                                                                                               //--- ������ - ����� �� - ��������������� ---
    "���������.",
    "E��� \"���\", ��� ����������� �������� ���������� ���������� �������������� ������� �� �������� ������ - ����� ��",
    DISABLE_RU,
    ENABLE_RU,
    FLASH_AUTOCONNECT,
    &PageDrive::self, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_6( ppDrive,  //-V641 //-V1027                                                                                                                            //--- ������ - ����� �� ---
    "����� ��",
    "������ � ������� ������������ �����������.",
    PageDrive::PageManager::self,   ///< ������ - ����� �� - �������
    &cDrive_Name,                   ///< ������ - ����� �� - ��� �����
    PageDrive::PageMask::self,      ///< ������ - ����� �� - �����
    &cDrive_SaveAs,                 ///< ������ - ����� �� - ��������� ���
    &cDrive_ModeBtnMemory,          ///< ������ - ����� �� - ��� �� ������
    &cDrive_Autoconnect,            ///< ������ - ����� �� - ���������������
    Page::Name::Memory_Drive,
    &PageMemory::self,               ///< ������
    0, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageMemory, // -V641 // -V1027                                                                                                                                   //--- ������ ---
    "������",
    "������ � ������� � ���������� �������.",
    &cPoints,               ///< ������ - �����
    PageRAM::self,          ///< ������ - ���������
    PageROM::self,          ///< ������ - ����� ��
    PageDrive::self,        ///< ������ - ����� ��
    Page::Name::Memory, nullptr, 0, 0, 0, 0
)

void PageMemory::SaveSignalToFlashDrive()
{
    if (FDrive::IsConnected())
    {
        if (FILE_NAMING_MODE_MANUAL)
        {
            Display::SetAddDrawFunction(DrawSetName);
        }
        else
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }
    }
    else
    {
        EXIT_FROM_SETNAME_TO = 0;
    }
}

static void DrawSetName()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::Top() + 60;
    int width = Grid::Width() - 80;
    int height = 80;

    Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (Tables::Get(index)[0] != ' ')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (Tables::Get(index)[0] != 'a')
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (Tables::Get(index)[0] != '%')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    int x = String(FILE_NAME).Draw(x0 + deltaX, y0 + 65, Color::FILL);

    Region(5, 8).Fill(x, y0 + 65, Color::FLASH_10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Exit()
{
    OnPressSB_Exit();

    if (EXIT_FROM_SETNAME_TO == RETURN_TO_DISABLE_MENU)
    {
        //Menu::PagePointerFromName(Page::Name::SB_Memory_SetName)->ShortPressOnItem(0);
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_LAST_MEM)
    {
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_INT_MEM)
    {
    }
    else
    {
        //
    }

    EXIT_FROM_SETNAME_TO = RETURN_TO_DISABLE_MENU;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Delete()
{
    FILE_NAME[0] = '\0';
}

DEF_SMALL_BUTTON( bSetName_Delete,                                                                                                                              //--- ��� ����� - ������� ---
    "�������",
    "������� ��� �������� �������",
    &PageSetName::self, 0, OnPress_SetName_Delete, Draw_Delete
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Backspace()
{
    int size = (int)std::strlen(FILE_NAME);
    if (size > 0)
    {
        FILE_NAME[size - 1] = '\0';
    }
}

DEF_SMALL_BUTTON( bSetName_Backspace,                                                                                                                         //--- ��� ����� - Backspace ---
    "Backspace",
    "������� ��������� ������",
    &PageSetName::self, 0, OnPress_SetName_Backspace, Draw_Backspace
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Insert()
{
    int size = (int)std::strlen(FILE_NAME);
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        FILE_NAME[size] = Tables::Get(INDEX_SYMBOL)[0];
        FILE_NAME[size + 1] = '\0';
    }
}

DEF_SMALL_BUTTON( bSetName_Insert,                                                                                                                             //--- ��� ����� - �������� ---
    "��������",
    "������ ��������� ������",
    &PageSetName::self, 0, OnPress_SetName_Insert, Draw_Insert
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Save()
{
    if (FDrive::IsConnected())
    {
        OnPress_SetName_Exit();
        NEED_SAVE_TO_FLASHDRIVE = 1;
    }
}

static void Draw_SetName_Save(int x, int y)
{
    if (FDrive::IsConnected())
    {
        Font::SetCurrent(Font::Type::_UGO2);
        Char(SYMBOL_FLASH_DRIVE_BIG).Draw4SymbolsInRect(x + 2, y + 1);
        Font::SetCurrent(Font::Type::_8);
    }
}

DEF_SMALL_BUTTON( bSetName_Save,                                                                                                                              //--- ��� ����� - ��������� ---
    "���������",
    "���������� �� ���� ��� �������� ������",
    &PageSetName::self, 0, OnPress_SetName_Save, Draw_SetName_Save
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool HandlerKey_SetName(KeyEvent event)
{
    OnMemExtSetMaskNameRegSet(event.Delta(), Tables::Size() / 4 - 7);

    return true;
}

void OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{
    static pFuncVpI8I8I8 func[3] =
    {
        Math::CircleDecrease<int8>,
        Math::CircleDecrease<int8>,
        Math::CircleIncrease<int8>
    };

    Color::ResetFlash();
    if (INDEX_SYMBOL > maxIndex)
    {
        INDEX_SYMBOL = (int8)(maxIndex - 1);
    }
    func[Math::Sign(angle) + 1](&INDEX_SYMBOL, 0, (int8)(maxIndex - 1));
    Beeper::RegulatorSwitchRotate();

}

DEF_PAGE_4(pSetName, // -V641                                                                                                                // �������� ���������� ��� ����� ����� ����� ///
    "",
    "",
    &bSetName_Delete,       /// ���� ����� ����� - �������
    &bSetName_Backspace,    /// ���� ����� ����� - Backspace
    &bSetName_Insert,       /// ���� ����� ����� - ��������
    &bSetName_Save,         /// ���� ����� ����� - ���������
    Page::Name::SB_Memory_SetName, 0, 0, 0, 0, HandlerKey_SetName
)
