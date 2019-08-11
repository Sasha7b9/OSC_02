#include "defines.h"
#include "Transceiver.h"
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


using namespace Display::Primitives;
using namespace Osci::Settings;
using namespace Transceiver;

const Page    *Menu::mainPage = nullptr;

/// ����, ��� �������� ����� �������� ���������
static Item   *itemHint = nullptr;
/// ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
const char    *stringForHint = nullptr;
/// true, ���� ����� ��������� ����� ������ �� ������
static bool needSaveScreen = false;
/// ����� ���������� ������� ������. ����� ��� ����, ����� ������������ ��������� ���������
static uint timeLastPressedButton = MAX_UINT;

/// ��������� �������� ������� �� ������ �������� page
static Item *LastOpened(Page *page);
/// ��������� ������� ������� ��������������� �������� ����
static void OnTimerAutoHide();
/// �������, ������� �������� ����� ������ ��������� ����
static void OnTimerStrNaviAutoHide();

static void ProcessButtonForHint(Key::E button);
/// �������� ���������
static void DrawHint();


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

    Transmitter::Send(Command::Screen, row);

    while (numRow == -1)
    {
        uint8 data = 0;
        Transmitter::Send(data);
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

    Display::FuncOnWaitStart("���� �������", false);

    uint timeStart = TIME_MS;

    while (TIME_MS - timeStart < 1500)
    {

    };
    
    Display::FuncOnWaitStop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update()
{
    while(!BufferButtons::IsEmpty())                            // ���� ���� ������� ����������
    {
        timeLastPressedButton = TIME_MS;                        // �� ��������� ����� ���������� �������, ����� �����, ����� ��������� ���������

        KeyEvent event = BufferButtons::Extract();              // ��������� ��������� �������

        if (!Keyboard::KeyIsActive(event.key))                        // ���� ������ �� ��������� ��� ��������� ������:
        {
            continue;                                           // �������� � ���������
        }

        if (HINT_MODE_ENABLED)                                  // ���� ������� ����� ���������
        {
            ProcessButtonForHint(event.key);                    // �� ������� ��������� ��� ��������������� ������
            continue;                                           // � ��������� � ��������� ������
        }

        if (TriggerDebugMenu::Triggered(event))                 // ���� ���������� ���� �������
        {
            continue;                                           // �� �������
        }

        Handlers::Process(event);                           // �� ������������ ���
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
static void ProcessButtonForHint(Key::E button)
{
    if (button == Key::Enter)
    {
        stringForHint =
            "������ ���� ��������� ��������� �������:\n"
            "1. ��� �������� ���� ������� ���� ������� � ���������� � ������� 0.5� ��������� ����.\n"
            "2. ��� �������� ���� ��������� ������ � ������� 0.5� ��������� ����.\n"
            "3. ��� ��������� \"������\x99����� �� ����\x99���������\" ������� ���������� �������� ����������� ������ ����. ���� ������� �������� "
            "�������� ��������, ���� �����������.\n"
            "4. ��� ��������� \"������\x99����� �� ����\x99�����������\" ������� ���������� �������� �������� ������ ����. ���� ������� �������� "
            "��������� � ������� ������, ���������� ������� �� ���������� ������� ����.\n"
            "5. ���� ���� ��������� � ������ ����� ������, �� ������� ��������� ��������.";
    } 
    else if (button == Key::Display)
    {
        stringForHint = "������ ������� ��������� ���� �������� �������.";
    }
    else if (button == Key::Memory)
    {
        stringForHint =
            "1. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����\" ��������� ���� ������ � �������.\n"
            "2. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����������\" ��������� ������ �� ����-����.";
    }
    else if (button == Key::Measure)
    {
        stringForHint = "������ ����� ��������� ���� �������������� ���������.";
    }
    else if (button == Key::Service)
    {
        stringForHint = "������ ������ ��������� ���� ��������� ������������.";
    }
    else if (button == Key::Start)
    {
        stringForHint = "������ ����/�TO� ��������� � ������������� ������� ����� ����������.";
    }
    else if (button == Key::ChannelA)
    {
        stringForHint =
            "1. ������ �����1 ��������� ���� �������� ������ 1.\n"
            "2. ������� � ��������� ������ �����1 � ������� 0.5� ������������� �������� ������ 1 �� ��������� 0�.";
    }
    else if (button == Key::ChannelB)
    {
        stringForHint =
            "1. ������ �����2 ��������� ���� �������� ������ 2.\n"
            "2. ������� � ��������� ������ �����2 � ������� 0.5� ������������� �������� ������ 2 �� ��������� 0�.";
    }
    else if (button == Key::Time)
    {
        stringForHint =
            "1. ������ ���� ��������� ���� �������� ���������.\n"
            "2. ������� � ��������� ������ ���� � ������� 0.5� ������������� �������� �� ����������� 0�.";
    }
    else if (button == Key::Trig)
    {
        stringForHint =
            "1. ������ ����� ��������� ���� �������� �������������.\n"
            "2. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99�����������\" ���������� �������������� "
            "��������� ������ �������������.\n"
            "3. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99����� ������\" ������������� ������� "
            "������������� 0�.";
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
            ((Button *)OpenedItem())->ShortPress();
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
    return set.menu_show && mainPage != nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ClosePage(Page *page)
{
    if (IS_PAGE_SB(page))
    {
        SMALL_BUTTON_FROM_PAGE(page, 0)->funcOnPress();
    }
    else
    {
        if(((Page *)page)->funcOnEnterExit)
        {
            ((Page *)page)->funcOnEnterExit(false);
        }
    }

    Page *keeper = (Page *)page->Keeper();

    if (keeper)
    {
        keeper->SetPosActItem(0x7f);
    }

    if (page == Menu::mainPage)    // -V1027
    {
        Menu::Show(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ������� parent, ���� �� �������� ���������� page
static void CloseIfSubPage(Page *parent, Page *page)
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
            page = (Page *)page->Keeper();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Init()
{
    PageDisplay::Init();
    PageMultimeter::Init();

    if ((Page *)LastOpened((Page *)PageFunction::self) == PageMultimeter::self) //-V1027
    {
        ClosePage((Page *)PageMultimeter::self);
    }

    Page *opened = (Page *)LastOpened((Page *)PageFunction::self); //-V1027

    CloseIfSubPage((Page *)PageMultimeter::self, opened);
    CloseIfSubPage((Page *)PageRecorder::self, opened);
    CloseIfSubPage((Page *)PageTester::self, opened);
    CloseIfSubPage((Page *)PageFrequencyCounter::self, opened);
    CloseIfSubPage((Page *)PageFFT::self, opened);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::CloseOpenedItem()
{
    Item *item = OpenedItem();

    if (IS_PAGE(item))
    {
        ClosePage((Page *)item); //-V1027
    }
    else
    {
        item->Open(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Item *Menu::OpenedItem()
{
    return LastOpened((Page *)Menu::mainPage);
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
static Item *LastOpened(Page *page)
{
    if (page == nullptr)
    {
        return &emptyControl;
    }

    if (page->CurrentItemIsOpened())
    {
        int8 posActItem = page->PosCurrentItem();
        Item *item = page->GetItem(posActItem);

        if (IS_PAGE(page->GetItem(posActItem)))
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
Item *Menu::CurrentItem()
{
    Item *opened = OpenedItem();

    int8 pos = ((const Page *)opened)->PosCurrentItem();

    if (IS_PAGE(opened) && pos != 0x7f)
    {
        return ((const Page *)opened)->GetItem(pos);
    }

    return (Item *)opened;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHintItem(int x, int y, int width)
{
    if (!itemHint)
    {
        return;
    }

    //DEF__STRUCT(StructName, pString) names[Item::Type::Number] =
    static pString names[Item::Type::Number] =
    {
        "",                   // Item_None
        "",                   // Item::Type::Choice
        "������",             // Item::Type::Button
        "��������",           // Item::Type::Page
        "���������",          // Item::Type::Governor
        "",                   // Item::Type::Time
        "",                   // Item::Type::GovernorColor
        "",                   // Item::Type::ChoiceReg
        "������",             // Item::Type::DrawButton
        "����� ���������"     // Item::Type::ChoiceParameter
    };

    Page *item = (Page *)itemHint;

    const int SIZE = 100;
    char title[SIZE];
    std::snprintf(title, SIZE, "%s \"%s\"", names[itemHint->type], item->titleHint[0]);

    if (item->type == Item::Type::DrawButton)
    {
        y -= 9;
    }

    Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);

    y = Text(item->titleHint[1]).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);

    if (item->type == Item::Type::DrawButton)
    {
        ((SButton*)item)->DrawHints(x, y, width);   // -V1027
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::Y0()
{
    return Display::HEIGHT - Item::HEIGHT - 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Draw()
{
    if (Menu::IsShown())
    {
        Item *item = OpenedItem();

        if (!IS_PAGE(item))
        {
            item = (Item *)item->Keeper();
        }

        item->Draw(0, Y0(), true);
    }

    DrawHint();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHint()
{
    if (HINT_MODE_ENABLED)
    {
        int x = 1;
        int y = 0;
        int width = 318;

        //Painter::DrawTextInBoundedRectWithTransfers(x, y, width,
        //       LANG_RU ? "������� ����� ���������. � ���� ������ ��� ������� �� ������ �� ����� ��������� ���������� � � ����������. "
        //       "����� ��������� ���� �����, ������� ������ ������ � ����������� � � ������� 0.5�." :
        //       "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
        //       "To disable this mode, press the button HELP and hold it for 0.5s.",
        //       Color::BACK, Color::FILL);

        Text("������� ����� ���������. � ���� ������ ��� ������� �� ������ �� ����� ��������� ���������� � � ����������. "
            "����� ��������� ���� �����, ������� ������ ������ � ����������� � � ������� 0.5�."
        ).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::FILL);

        y += 49;
        if (stringForHint)
        {
            Text(stringForHint).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::WHITE);
        }
        else if (itemHint)
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
void Menu::SetItemForHint(const Item *item)
{
    stringForHint = nullptr;
    itemHint = (Item *)item;
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
