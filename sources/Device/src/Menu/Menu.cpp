#include "defines.h"
#include "device.h"
#include "Transceiver.h"
#include "Display/Primitives.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/HandlersKeys.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cstdio>


using namespace Transceiver;

const Page    *Menu::mainPage = nullptr;

/// ����, ��� �������� ����� �������� ���������
static Item   *itemHint = nullptr;
/// ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
const char    *stringForHint = nullptr;
/// ����� ��� ����, ����� ������������ ��������� ���������
static uint timeLastKeyboardEvent = MAX_UINT;
/// ��������� �������� ������� �� ������ �������� page
static Item *LastOpened(Page *page);
/// ��������� ������� ������� ��������������� �������� ����
static void OnTimerAutoHide();

static void ProcessButtonForHint(Key::E button);
/// �������� ���������
static void DrawHint();



void Menu::ProcessingAllKeyboardEvents()
{
    while(!BufferButtons::IsEmpty())                // ���� ���� ������� ����������
    {
        timeLastKeyboardEvent = TIME_MS;            // �� ��������� ����� ���������� �������, ����� �����, ����� ��������� ���������

        KeyEvent event = BufferButtons::Extract();  // ��������� ��������� �������

        if (!Keyboard::KeyIsActive(event.key))      // ���� ������ �� ��������� ��� ��������� ������:
        {
            continue;                               // �������� � ���������
        }

        if (HINT_MODE_ENABLED)                      // ���� ������� ����� ���������
        {
            ProcessButtonForHint(event.key);        // �� ������� ��������� ��� ��������������� ������
            continue;                               // � ��������� � ��������� ������
        }

        Handlers::Process(event);                   // �� ������������ ���
    }
}


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


void Menu::SetAutoHide(bool)
{
    if (Menu::IsShown())
    {
        if (Display::TimeMenuAutoHide() == 0)
        {
            Timer::Disable(Timer::Type::MenuAutoHide);
        }
        else
        {
            Timer::SetAndStartOnce(Timer::Type::MenuAutoHide, OnTimerAutoHide, static_cast<uint>(Display::TimeMenuAutoHide()));
        }
    }
}


static void OnTimerAutoHide()
{
    Menu::Hide();
}


void Menu::ChangeStateFlashDrive()
{
    if(!FDrive::IsConnected())
    {
        if(Menu::OpenedItem() == PageDrive::PageManager::self)
        {
            OpenedItem()->HandlerFX(TypePress::Release);
        }
    }
    else if(set.mem.flashAutoConnect)
    {
        PageMemory::OnOpenClose_Drive_Manager();
    }
    else
    {
        // ��������� �������� �� ��������������
    }
}


void Menu::Show()
{
    set.menu.show = true;

    Menu::SetAutoHide(true);
}


void Menu::Hide()
{
    set.menu.show = false;

    Menu::SetAutoHide(true);
}


bool Menu::IsShown()
{
    return set.menu.show && mainPage != nullptr;
}


static void ClosePage(Page *page)
{
    page->OwnData()->funcOnOpenClose(false);

    Page *keeper = const_cast<Page *>(page->Keeper());

    if (keeper)
    {
        keeper->SetPosActItem(0x7f);
    }

    if (page == Menu::mainPage)    // -V1027
    {
        Menu::Hide();
    }
}


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


void Menu::Init()
{
    PageMultimeter::Init();
    PageFreqMeter::Init();

    if ((Page *)LastOpened((Page *)PageFunction::self) == PageMultimeter::self) //-V1027
    {
        ClosePage((Page *)PageMultimeter::self);
    }

    Page *opened = (Page *)LastOpened((Page *)PageFunction::self); //-V1027

    CloseIfSubPage(const_cast<Page *>(PageMultimeter::self), opened);
    CloseIfSubPage(const_cast<Page *>(PageRecorder::self), opened);
    CloseIfSubPage(const_cast<Page *>(PageTester::self), opened);
    CloseIfSubPage(const_cast<Page *>(PageFreqMeter::self), opened);
    CloseIfSubPage(const_cast<Page *>(PageFFT::self), opened);
}


void Menu::CloseOpenedItem()
{
    Item *item = OpenedItem();

    if (item->Is(Item::Type::Page))
    {
        ClosePage((Page *)item); //-V1027
    }
    else
    {
        item->Open(false);
    }
}


Item *Menu::OpenedItem()
{
    return LastOpened((Page *)Menu::mainPage);
}


static Item *LastOpened(Page *page)
{
    Item *result = &Item::empty;

    if (page)
    {
        if (page->CurrentItemIsOpened())
        {
            int8 posActItem = page->PosCurrentItem();
            Item *item = page->GetItem(posActItem);

            if (page->GetItem(posActItem)->Is(Item::Type::Page))
            {
                result = LastOpened(static_cast<Page *>(item));
            }
            else
            {
                result = item;
            }
        }
        else
        {
            result = page;
        }
    }

    return result;
}


Item *Menu::CurrentItem()
{
    Item *result = OpenedItem();

    int8 pos = ((const Page *)result)->PosCurrentItem();

    if (result->Is(Item::Type::Page) && pos != 0x7f)
    {
        result = ((const Page *)result)->GetItem(pos);
    }

    return result;
}


static void DrawHintItem(int x, int y, int width)
{
    if (itemHint)
    {
        //DEF__STRUCT(StructName, pString) names[Item::Type::Number] =
        static pString names[Item::Type::Number] =
        {
            "",                   // Item_None
            "",                   // Item::Type::Choice
            "������",             // Item::Type::Button
            "��������",           // Item::Type::Page
            "���������",          // Item::Type::Governor
            "",                   // Item::Type::GovernorColor
            "������"              // Item::Type::DrawButton
        };

        Page *item = (Page *)itemHint;

        const int SIZE = 100;
        char title[SIZE];
        std::snprintf(title, SIZE, "%s \"%s\"", names[itemHint->data->type], item->data->title);

        Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);

        y = Text(item->data->hint).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);

        if (item->Is(Item::Type::GraphButton))
        {
            ((GraphButton *)item)->DrawHints(x, y, width);   // -V1027
        }
    }
}


int Menu::Y0()
{
    return Display::HEIGHT - 25;
}


void Menu::Draw()
{
    if (Menu::IsShown())
    {
        Item *item = OpenedItem();

        if (item)
        {
            if (!item->Is(Item::Type::Page))
            {
                item = (Item *)item->Keeper();
            }

            if (item)
            {
                item->Draw(0, Y0(), true);
            }
        }
    }

    DrawHint();
}


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


void Menu::SetItemForHint(const Item *item)
{
    stringForHint = nullptr;
    itemHint = (Item *)item;
}


void Menu::SaveSettings()
{
    if((timeLastKeyboardEvent != MAX_UINT) && (TIME_MS - timeLastKeyboardEvent > 5000))
    {
        timeLastKeyboardEvent = MAX_UINT;
        if(!Device::State::InModeTester())
        {
            Settings::Save();
        }
    }
}


const Item *Menu::ItemUnderFunctionalKey(Key::E key)
{
    const Item *result = &Item::empty;

    if (Key(key).IsFunctional())
    {
        Item *item = Menu::OpenedItem();

        if (item == nullptr)
        {
        }
        else if (item->Is(Item::Type::Page))
        {
            result = ((Page *)item)->ItemForFuncKey(key);
            if (!result->IsActive())
            {
                result = &Item::empty;
            }
        }
        else
        {
            const Page *parent = item->Keeper();
            if (parent && parent->ItemForFuncKey(key) == item && item->IsActive())
            {
                result = item;
            }
        }
    }

    return result;
}
