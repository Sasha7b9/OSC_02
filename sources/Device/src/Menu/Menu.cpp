#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/BufferButtons.h"
#include "Hardware/Timer.h"
#include "Menu/MenuTriggers.h"
#include "Keyboard/HandlersKeys.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "Settings/SettingsMemory.h"
#include "Settings/Settings.h"
#include "Pages/Include/PageMemory.h"
#include "Pages/Include/PageChannels.h"
#include "Pages/Include/PageHelp.h"
#include "Pages/Include/PageMeasures.h"
#include "Pages/Include/PageDisplay.h"
#include "Pages/Include/PageFunction.h"
#include "Hardware/Beeper.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Utils/Math.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include <cstdio>

#include "Utils/Debug.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/DecoderDevice.h"
#include "Utils/Dictionary.h"


using namespace Display::Primitives;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   Control    *Menu::itemUnderKey = 0;
const char    *Menu::stringForHint = 0;
   Control    *Menu::itemHint = 0;
/// true, ���� ����� ��������� ����� ������ �� ������
static bool needSaveScreen = false;

/// �������� ����������, ����������� � ������ ������ ��������������� �������
static Control *underButton[Key::Number];

/// ��������� �������� ������� �� ������ �������� page
static Control *LastOpened(Page *page);
/// ��������� ������� ������� ��������������� �������� ����
static void OnTimerAutoHide();
/// �������, ������� �������� ����� ������ ��������� ����
static void OnTimerStrNaviAutoHide();

static void ProcessButtonForHint(Key::E button);

static void ResetItemsUnderButton();
/// ���������� true, ���� ������ ������ ������������� � ������ ������
static bool IsProcessed(const KeyEvent *event);
/// ����� ���������� ������� ������. ����� ��� ����, ����� ������������ ��������� ���������
static uint timeLastPressedButton = MAX_UINT;
/// ������� ������� ��������
static PageBase *mainPage = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void CreateFileName(char name[256])
{
    std::strcpy(name, "screen.bmp");
}

static int numRow = -1;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SaveRow(int row)
{
    numRow = row;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadRow(uint8 row)
{
    numRow = -1;

    uint8 buffer[] = { Command::Screen, row };

    HAL::FSMC::WriteToPanel(buffer, 2);

    while (numRow == -1)
    {
        uint8 data = 0;
        HAL::FSMC::WriteToPanel(&data, 1);
        Decoder::Update();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SaveScreenToFlash()
{
    if (!FDrive::IsConnected())
    {
        return;
    }

#pragma pack(1)
    struct BITMAPFILEHEADER
    {
        char    type0;      // 0
        char    type1;      // 1
        uint    size;       // 2
        uint16  res1;       // 6
        uint16  res2;       // 8
        uint    offBits;    // 10
    }
    bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 240,
        0,
        0,
        14 + 40 + 1024
    };

    // 14

    struct BITMAPINFOHEADER
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    }
    bmIH =
    {
        40, // size;
        320,// width;
        240,// height;
        1,  // planes;
        8,  // bitCount;
        0,  // compression;
        0,  // sizeImage;
        0,  // xPelsPerMeter;
        0,  // yPelsPerMeter;
        0,  // clrUsed;
        0   // clrImportant;
    };

    // 54
#pragma pack(4)

    StructForWrite structForWrite;

    char fileName[255];

    CreateFileName(fileName);

    FDrive::OpenNewFileForWrite(fileName, &structForWrite);

    FDrive::WriteToFile((uint8 *)(&bmFH), 14, &structForWrite);

    FDrive::WriteToFile((uint8 *)(&bmIH), 40, &structForWrite);

    uint8 buffer[320 * 3] = { 0 };

    typedef struct tagRGBQUAD
    {
        uint8    blue;
        uint8    green;
        uint8    red;
        uint8    rgbReserved;
    } RGBQUAD;

    RGBQUAD colorStruct;

    for (int i = 0; i < 32; i++)
    {
        uint color = COLOR(i);
        colorStruct.blue = (uint8)((float)B_FROM_COLOR(color));
        colorStruct.green = (uint8)((float)G_FROM_COLOR(color));
        colorStruct.red = (uint8)((float)R_FROM_COLOR(color));
        colorStruct.rgbReserved = 0;
        ((RGBQUAD*)(buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(buffer, 256, &structForWrite);
    }

    uint8 pixels[320];

    Decoder::SetBufferForScreenRow(pixels);

    for (int row = 239; row >= 0; row--)
    {
        ReadRow((uint8)row);

        FDrive::WriteToFile(pixels, 320, &structForWrite);
    }

    FDrive::CloseFile(&structForWrite);

    Display::FuncOnWaitStart(DICT(DFileSaved), false);

    uint timeStart = TIME_MS;

    while (TIME_MS - timeStart < 1500)
    {

    };
    
    Display::FuncOnWaitStop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update()
{
    while(!BufferButtons::IsEmpty())
    {
        timeLastPressedButton = TIME_MS;

        KeyEvent event = BufferButtons::Extract();

        if (HINT_MODE_ENABLED)
        {
            ProcessButtonForHint(event.key);
            continue;
        }

        if (!Menu::IsShown())
        {
            if(event.type == TypePress::Release && TriggerDebugConsole::Update(event.key))
            {
                continue;
            }
        }
        if(IsProcessed(&event))
        {
            Handlers::Process(event);
        }
    }

    if (needSaveScreen)
    {
        SaveScreenToFlash();

        needSaveScreen = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SaveScreenToDrive()
{
    needSaveScreen = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsProcessed(const KeyEvent *event)
{
    Key::E key = event->key;
    TypePress::E type = event->type;

    if (Device::State::InModeTester())
    {
        if (Key(key).IsControlSignal() || Key(key).IsFunctional() || (key == Key::Enter && !TypePress(type).IsLong()))
        {
            return true;
        }
        return false;
    }
    
    if (Device::State::InModeMultimeter())
    {
        if (Key(key).IsFunctional() ||          // ���������� ��������� �� �������������� ������
            Key(key).IsArrow() ||               // �� �������
            (key == Key::Enter && !TypePress(type).IsLong()))
        {
            return true;
        }
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessButtonForHint(Key::E button)
{
    if (button == Key::Enter)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ���� ��������� ��������� �������:\n"
            "1. ��� �������� ���� ������� ���� ������� � ���������� � ������� 0.5� ��������� ����.\n"
            "2. ��� �������� ���� ��������� ������ � ������� 0.5� ��������� ����.\n"
            "3. ��� ��������� \"������\x99����� �� ����\x99���������\" ������� ���������� �������� ����������� ������ ����. ���� ������� �������� "
            "�������� ��������, ���� �����������.\n"
            "4. ��� ��������� \"������\x99����� �� ����\x99�����������\" ������� ���������� �������� �������� ������ ����. ���� ������� �������� "
            "��������� � ������� ������, ���������� ������� �� ���������� ������� ����.\n"
            "5. ���� ���� ��������� � ������ ����� ������, �� ������� ��������� ��������."
            :
        "���� button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the Menu::\n"
            "2. At the open menu deduction of the button during 0.5s closes the Menu::\n"
            "3. At control \"SERVICE\x99Mode btn MENU\x99\x43lose\" current becomes the page of the previous level of the Menu:: If the root page is "
            "current, the menu is closed.\n"
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the Menu:: If the current page the "
            "last in the current level, happens transition to the previous level of the Menu::\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } 
    else if (button == Key::Display)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ������� ��������� ���� �������� �������."
            :
            "������� button opens the display settings Menu::";
    }
    else if (button == Key::Memory)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����\" ��������� ���� ������ � �������.\n"
            "2. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����������\" ��������� ������ �� ����-����."
            :
            "1. When setting \"������-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"������-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == Key::Measure)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ����� ��������� ���� �������������� ���������."
            :
            "����� button opens a menu of automatic measurements.";
    }
    else if (button == Key::Service)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ������ ��������� ���� ��������� ������������."
            :
            "������ button opens a menu of service options.";
    }
    else if (button == Key::Start)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ����/�TO� ��������� � ������������� ������� ����� ����������."
            :
            "����/�TO� button starts and stops the process of gathering information.";
    }
    else if (button == Key::ChannelA)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ������ �����1 ��������� ���� �������� ������ 1.\n"
            "2. ������� � ��������� ������ �����1 � ������� 0.5� ������������� �������� ������ 1 �� ��������� 0�."
            :
            "1. �����1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button �����1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == Key::ChannelB)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ������ �����2 ��������� ���� �������� ������ 2.\n"
            "2. ������� � ��������� ������ �����2 � ������� 0.5� ������������� �������� ������ 2 �� ��������� 0�."
            :
            "1. �����2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button �����2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == Key::Time)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ������ ���� ��������� ���� �������� ���������.\n"
            "2. ������� � ��������� ������ ���� � ������� 0.5� ������������� �������� �� ����������� 0�."
            :
            "1. ���� button open the settings menu sweep.\n"
            "2. Pressing and holding the button ���� for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == Key::Trig)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ������ ����� ��������� ���� �������� �������������.\n"
            "2. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99�����������\" ���������� �������������� "
            "��������� ������ �������������.\n"
            "3. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99����� ������\" ������������� ������� "
            "������������� 0�."
            :
            "1. ����� button opens a menu settings synchronization.\n"
            "2. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\x41utolevel\" automatically adjust the "
            "trigger level.\n"
            "3. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger "
            "level 0V.";
    }
    else
    {
        // ����� ������ �� ������
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetAutoHide(bool)
{
    if(!Menu::IsShown())
    {
        return;
    }
    if(Display::TimeMenuAutoHide() == 0)
    {
        Timer::Disable(Timer::Type::MenuAutoHide);
    }
    else
    {
        Timer::SetAndStartOnce(Timer::Type::MenuAutoHide, OnTimerAutoHide, (uint)Display::TimeMenuAutoHide());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerAutoHide()
{
    Menu::Show(false);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::TemporaryEnableStrNavi()
{
    if (SHOW_STRING_NAVI_TEMP)
    {
        SHOW_STRING_NAVIGATION = 1;                                            // ������������� ������� ����, ��� ���� �������� ������ ��������� ����
        Timer::SetAndStartOnce(Timer::Type::StrNaviAutoHide, OnTimerStrNaviAutoHide, 3000); // � ��������� ������, ������� ��� ��������
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerStrNaviAutoHide()
{
    SHOW_STRING_NAVIGATION = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ChangeStateFlashDrive()
{
    if(!FDrive::IsConnected())
    {
        if(GetNameOpenedPage() == Page::Name::SB_Memory_Drive_Manager)
        {
            ((Page *)OpenedItem())->ShortPressOnItem(0);
        }
    }
    else if(FLASH_AUTOCONNECT)
    {
        PageMemory::OnPress_Drive_Manager();
    }
    else
    {
        // ��������� �������� �� ��������������
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Show(bool show)
{
    set.menu_show = show;

    if (show)
    {
        Menu::TemporaryEnableStrNavi();
    }
    Menu::SetAutoHide(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsShown()
{
    return set.menu_show && MainPage() != nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ClosePage(PageBase *page)
{
    if (IS_PAGE_SB(page))
    {
        SMALL_BUTTON_FROM_PAGE(page, 0)->funcOnPress();
    }
    else
    {
        ((Page *)page)->funcOnEnterExit(false);
    }

    Page *keeper = (Page *)KEEPER(page);

    if (keeper)
    {
        keeper->SetPosActItem(0x7f);
    }

    if (page == (PageBase *)Menu::MainPage())    // -V1027
    {
        Menu::Show(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ������� parent, ���� �� �������� ���������� page
static void CloseIfSubPage(PageBase *parent, PageBase *page)
{
    if (page == parent)
    {
        ClosePage(parent);
    }

    if (((Page *)page)->IsSubPage(parent))
    {
        while (page)
        {
            ClosePage(page);
            page = KEEPER(page);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Init()
{
    PageDisplay::Init();
    PageFunction::PageMultimeter::Init();

    if ((PageBase *)LastOpened((Page *)PageFunction::pointer) == PageFunction::PageMultimeter::pointer) //-V1027
    {
        ClosePage((PageBase *)PageFunction::PageMultimeter::pointer);
    }

    PageBase *opened = (PageBase *)LastOpened((Page *)PageFunction::pointer); //-V1027

    CloseIfSubPage((PageBase *)PageFunction::PageMultimeter::pointer, opened);
    CloseIfSubPage((PageBase *)PageFunction::PageRecorder::pointer, opened);
    CloseIfSubPage((PageBase *)PageFunction::PageTester::pointer, opened);
    CloseIfSubPage((PageBase *)PageFunction::PageFrequencyCounter::pointer, opened);
    CloseIfSubPage((PageBase *)PageFunction::PageFFT::pointer, opened);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::CloseOpenedItem()
{
    Control *item = OpenedItem();

    if (IS_PAGE(item))
    {
        ClosePage((PageBase *)item); //-V1027
    }
    else
    {
        item->Open(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem()
{
    return LastOpened((Page *)MainPage());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Page::Name::E Menu::GetNameOpenedPage()
{
    const Page *opened = (const Page *)OpenedItem();

    if (opened == nullptr)
    {
        return Page::Name::NoPage;
    }

    return opened->GetName();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PageBase *Menu::OpenedPage()
{
    return (PageBase *)OpenedItem(); //-V1027
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Control *LastOpened(Page *page)
{
    if (page == nullptr)
    {
        return nullptr;
    }

    if (page->CurrentItemIsOpened())
    {
        int8 posActItem = page->PosCurrentItem();
        Control *item = page->Item(posActItem);

        if (page->Item(posActItem)->IsPage())
        {
            return LastOpened((Page *)item);
        }
        else
        {
            return item;
        }
    }
    return page;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::CurrentItem()
{
    Control *opened = OpenedItem();

    int8 pos = ((const Page *)opened)->PosCurrentItem();

    if (opened->IsPage() && pos != 0x7f)
    {
        return ((const Page *)opened)->Item(pos);
    }

    return (Control *)opened;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHintItem(int x, int y, int width)
{
    if (!Menu::itemHint)
    {
        return;
    }

    DEF_STRUCT(StructName, pString) names[Control::Type::Number][2] =
    {
        {"",                ""},                    // Item_None
        {"",                ""},                    // Control::Type::Choice
        {"������",          "Button"},              // Control::Type::Button
        {"��������",        "Page"},                // Control::Type::Page
        {"���������",       "Governor"},            // Control::Type::Governor
        {"",                ""},                    // Control::Type::Time
        {"",                ""},                    // Control::Type::GovernorColor
        {"",                ""},                    // Control::Type::ChoiceReg
        {"������",          "Button"},              // Control::Type::DrawButton
        {"����� ���������", "Choice parameter"},    // Control::Type::ChoiceParameter
    };

    Language::E lang = LANG;
    Page *item = (Page *)Menu::itemHint;

    const int SIZE = 100;
    char title[SIZE];
    std::snprintf(title, SIZE, "%s \"%s\"", names[Menu::itemHint->type][lang].val, item->titleHint[lang]);

    if (item->type == Control::Type::DrawButton)
    {
        y -= 9;
    }

    Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);

    y = Text(item->titleHint[2 + lang]).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);

    if (item->type == Control::Type::DrawButton)
    {
        ((SButton*)item)->DrawHints(x, y, width);   // -V1027
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::Y()
{
    return Display::HEIGHT - Item::HEIGHT - 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Draw()
{
    if (Menu::IsShown())
    {
        ResetItemsUnderButton();
        Control *item = OpenedItem();
        if (Menu::IsShown())
        {
            if (IS_PAGE(item))
            {
                item->Draw(0, Y(), true);
            }
            else
            {
                ((Page *)KEEPER(item))->Draw(0, Y(), true);
            }
        }
    }

    if (HINT_MODE_ENABLED)
    {
        int x = 1;
        int y = 0;
        int width = 318;
        if (Menu::IsShown())
        {
            width = Menu::IsMinimize() ? 289 : 220;
        }

        //Painter::DrawTextInBoundedRectWithTransfers(x, y, width,
        //       LANG_RU ? "������� ����� ���������. � ���� ������ ��� ������� �� ������ �� ����� ��������� ���������� � � ����������. "
        //       "����� ��������� ���� �����, ������� ������ ������ � ����������� � � ������� 0.5�." :
        //       "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
        //       "To disable this mode, press the button HELP and hold it for 0.5s.",
        //       Color::BACK, Color::FILL);

        Text(
            LANG_RU ? 
            "������� ����� ���������. � ���� ������ ��� ������� �� ������ �� ����� ��������� ���������� � � ����������. "
            "����� ��������� ���� �����, ������� ������ ������ � ����������� � � ������� 0.5�."
            :
            "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
            "To disable this mode, press the button HELP and hold it for 0.5s."
        ).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::FILL);

        y += LANG_RU ? 49 : 40;
        if (Menu::stringForHint)
        {
            Text(Menu::stringForHint).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::WHITE);
        }
        else if (Menu::itemHint)
        {
            DrawHintItem(x, y, width);
        }
        else
        {
            // ������ �� ������
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ResetItemsUnderButton()
{
    for (int i = 0; i < Key::Number; i++)
    {
        Menu::SetItemUnderButton((Key::E)i, 0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsMinimize()
{
    return IS_PAGE_SB(Menu::OpenedItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetItemForHint(void *item)
{
    Menu::stringForHint = 0;
    Menu::itemHint = (Control *)item;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ButtonEvent(KeyEvent event)
{
    /// \todo �� ����� ������ ������ ��� ���������� ��������� ������ ��������. ������� �������� ��� �������� ������
    Beeper::Beep(event.type);
    BufferButtons::Push(event);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SaveSettings()
{
    if((timeLastPressedButton != MAX_UINT) && (TIME_MS - timeLastPressedButton > 5000))
    {
        timeLastPressedButton = MAX_UINT;
        if(!Device::State::InModeTester())
        {
            Settings::Save();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ReleaseFunctionalButton(Key::E key)
{
    if(Menu::IsShown())
    {
        Control *control = Menu::ItemUnderButton(key);
        if(control)
        {
            control->ShortPress();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::LongFunctionalButton(Key::E key)
{
    if(Menu::IsShown())
    {
        Control *control = Menu::ItemUnderButton(key);
        if(control)
        {
            control->LongPress();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::ItemUnderButton(Key::E button)
{
    return underButton[button];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetItemUnderButton(Key::E button, Control *control)
{
    underButton[button] = control;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PageBase *Menu::MainPage()
{
    return mainPage;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetMainPage(PageBase *page)
{
    mainPage = page;
}
