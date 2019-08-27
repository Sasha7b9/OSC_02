#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Hardware/Beeper.h"
#include "Osci/Display/Osci_Display.h"
#include "Hardware/Timer.h"
#include "Hardware/Clock.h"
#include "Display/Symbols.h"
#include "Menu/Pages/Include/PageFunction.h"


typedef enum
{
    NONE,
    INCREASE,
    DECREASE
} DIRECTION;

/// ��������� ������������ ��� �������� ��������� ��� Choice � Governor
typedef struct
{
    void       *address;    ///< ����� ��������. ���� 0 - �� ��������
    uint        timeStart;  ///< ����� ������ �������� � �������������
    DIRECTION   dir;        ///< ����������� ��������� ��������
    uint8       notUsed0;
    uint8       notUsed1;
    uint8       notUsed2;
} TimeStruct;

static TimeStruct tsChoice = { 0, 0, NONE, 0, 0, 0 };

static TimeStruct tsGovernor = { 0, 0, NONE, 0, 0, 0 };

Item Item::empty;

int8 Governor::currentDigit = 0;

#define NAME_FROM_INDEX(index) (names[index])

/// ����� �������� ����� �����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0,  ���� �� ���� ������ �� ������.
static const Item *pressedItem = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Item::KeyPress() const
{
    pressedItem = this;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::KeyRelease() const
{
    pressedItem = nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::KeyAutoRelease() const
{
    pressedItem = nullptr;

    if (!IsActive())
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::HeightOpened() const
{
    return Height();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsCurrentItem() const
{
    return (this == Menu::CurrentItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::Open(bool open) const
{
    Page *parent = (Page *)Keeper();
    if (parent)
    {
        parent->SetPosActItem(open ? (parent->PosCurrentItem() | 0x80) : (parent->PosCurrentItem() & 0x7f));
        if (!open)
        {
            SetCurrent(false);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsOpened() const
{
    const Page *parent = Keeper();

    if (parent == nullptr)
    {
        return false;
    }

    if (Is(Type::Page))
    {
        return parent->CurrentItemIsOpened();
    }
    return (MENU_POS_ACT_ITEM(parent->name) & 0x80) != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::SetCurrent(bool active) const
{
    Page *page = (Page *)Keeper();

    if (page == nullptr)
    {
        return;
    }

    if (!active)
    {
        page->SetPosActItem(0x7f);
    }
    else
    {
        for (int i = 0; i < page->NumItems(); i++)
        {
            if (page->GetItem(i) == this)
            {
                page->SetPosActItem((int8)i);
                return;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsPressed() const
{
    return (this == pressedItem);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Item::Title() const
{
    return String(titleHint[0]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::ExistKeeper(const Page *_keeper) const
{
    const Page *item = Keeper();
    while (item)
    {
        if (item == _keeper)
        {
            return true;
        }
        item = item->Keeper();
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::Height() const
{
    return 23;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::Width(int pos) const
{
    int width = 320 / 5 + 1;

    if (pos != -1)
    {
        return ((pos + 1) % 5 == 0) ? width - 1 : width;
    }

    int position = PositionInKeeperList();

    if (position != -1)
    {
        return (position + 1) % 5 == 0 ? width - 1 : width;
    }

    return width;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::PositionOnScreenX() const
{
    int position = PositionInKeeperList();

    if (position != -1)
    {
        while (position > 4)
        {
            position -= 5;
        }

        return (Width(0) - 1) * position;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::PositionInKeeperList() const
{
    const Page *parent = Keeper();

    if (parent)
    {
        for (int i = 0; i < parent->NumItems(); i++)
        {
            if (this == parent->items[i])
            {
                return i;
            }
        }
    }

    return -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Page::NumSubPages() const
{
    return (NumItems() - 1) / Item::NUM_ON_DISPLAY + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems() const //-V2506
{
    if (name == Page::Name::Main)
    {
        return SHOW_DEBUG_MENU ? num : (num - 1);
    }
    return num;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ShortPress() const
{
    funcOnEnterExit(true);

    SetAsCurrent();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::KeyRelease() const
{
    Item::KeyRelease();

    ShortPress();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    if (!IsCurrentItem())
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::IsSubPage(const Page *parent)
{
    const Page *keep = Keeper();

    while (keep)
    {
        if (keep == parent)
        {
            return true;
        }

        keep = ((Item *)keep)->Keeper();
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Page::Name::E Page::GetName() const
{
    return (Page::Name::E)name;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::ProcessKey(KeyEvent event)
{
    if (funcKey && funcKey(event))
    {
        return true;
    }
    else if (event.type == TypePress::Press)
    {
        ChangeSubPage((event.key == Key::Left) ? -1 : 1);
        return true;
    }
    else
    {
        // ��������� ���� ������� �� ��������������
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetAsCurrent() const
{
    if (IsActive())
    {
        SetCurrent(true);
        Open(!IsOpened());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::CurrentItemIsOpened() const
{
    return _GET_BIT(MENU_POS_ACT_ITEM(name), 7) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetPosActItem(int8 pos) const
{
    MENU_POS_ACT_ITEM(name) = pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int8 Page::PosCurrentItem() const
{
    return MENU_POS_ACT_ITEM(name) & 0x7f;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Item *Page::GetItem(int numItem) const
{
    if (numItem >= num)
    {
        return &Item::empty;
    }

    return (Item *)items[numItem];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ChangeSubPage(int delta)
{
    if (delta > 0 && CurrentSubPage() < NumSubPages() - 1)
    {
        Beeper::RegulatorSwitchRotate();
        SetCurrentSubPage(CurrentSubPage() + 1);
    }
    else if (delta < 0 && CurrentSubPage() > 0)
    {
        Beeper::RegulatorSwitchRotate();
        SetCurrentSubPage(CurrentSubPage() - 1);
    }
    else
    {
        // ����� ������ �� ������
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int8 Page::CurrentSubPage() const
{
    return set.menu_currentSubPage[name];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetCurrentSubPage(int8 pos) const
{
    set.menu_currentSubPage[name] = pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::PosItemOnLeft() const
{
    return CurrentSubPage() * Item::NUM_ON_DISPLAY;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const Item *Page::ItemForFuncKey(Key::E key) const
{
    return GetItem(PosItemOnLeft() + key - Key::F1);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Button::KeyRelease() const
{
    Item::KeyRelease();

    if (IsActive())
    {
        SetCurrent(true);

        if (funcOnPress)
        {
            funcOnPress();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    KeyRelease();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GraphButton::KeyRelease() const
{
    Item::KeyRelease();

    if (IsActive())
    {
        funcOnPress();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphButton::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    KeyRelease();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Governor::KeyRelease() const
{
    Item::KeyRelease();

    if (IsActive())
    {
        if (Menu::OpenedItem() == this)
        {
            NextPosition();
        }
        else
        {
            SetCurrent(!IsCurrentItem());
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    if (!IsCurrentItem())
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::NextPosition() const
{
    if (Menu::OpenedItem() == this)
    {
        Math::CircleIncrease<int8>(&currentDigit, 0, (int8)(NumDigits() - 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::NextValue() const
{
    return (GetValue() < maxValue) ? (GetValue() + 1) : minValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::PrevValue() const
{
    return (GetValue() > minValue) ? (GetValue() - 1) : maxValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Governor::Step() const
{
    static const float speed = 0.05F;
    static const int numLines = 10;
    float delta = 0.0F;

    if (tsGovernor.address == this)
    {
        delta = speed * (TIME_MS - tsGovernor.timeStart);

        if (tsGovernor.dir == DECREASE)
        {
            delta *= -1.0F;
            if (delta == 0.0F)  // -V550 //-V2550 //-V550
            {
                return -0.001F;
            }
            if (delta < -numLines)
            {
                tsGovernor.dir = NONE;
                SetValue(PrevValue());
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0F;
                tsGovernor.address = 0;
            }
        }
        else if (tsGovernor.dir == INCREASE)
        {
            if (delta == 0.0F)  // -V550 //-V2550 //-V550
            {
                return 0.001F;
            }
            if (delta > numLines)
            {
                tsGovernor.dir = NONE;
                SetValue(NextValue());
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0F;
                tsGovernor.address = 0;
            }
        }
        else
        {
            // NONE
        }

    }
    return delta;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Governor::NumDigits() const
{
    int min = Integer(Math::Abs(minValue)).NumDigits();
    int max = Integer(Math::Abs(maxValue)).NumDigits();
    if (min > max)
    {
        max = min;
    }
    return (uint)max;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::ChangeValue(int16 delta)
{
    if (!IsOpened())
    {
        currentDigit = 0;
    }

    int16 oldValue = GetValue();

    int16 newValue = GetValue() + (int16)(Math::Sign(delta) * Math::Pow10(currentDigit));

    LIMITATION(newValue, minValue, maxValue); //-V2516

    SetValue(newValue);

    if (GetValue() != oldValue)
    {
        if (funcOfChanged)
        {
            funcOfChanged();
        }
        Beeper::GovernorChangedValue();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Governor::ProcessKey(KeyEvent event)
{
    if (event.IsLeft())
    {
        if (event.IsPress())
        {
            NextPosition();
        }
    }
    else if (event.IsRight())
    {
        if (event.IsPress())
        {
            PrevPosition();
        }
    }
    else if (event.IsUp() || event.IsDown())
    {
        if (event.IsPress() || event.IsRepeat())
        {
            ChangeValue(event.IsUp() ? 1 : -1);
        }
    }
    else
    {
        // ��������� ������ �� ������������
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::PrevPosition()
{
    if (Menu::OpenedItem() == this)
    {
        Math::CircleDecrease<int8>(&currentDigit, 0, (int8)(NumDigits() - 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::GetValue() const
{
    return *cell;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::SetValue(int16 v) const
{
    *cell = v;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Governor::GetSymbol() const
{
    static const char chars[] =
    {
        SYMBOL_GOVERNOR_SHIFT_0,
        SYMBOL_GOVERNOR_SHIFT_1,
        SYMBOL_GOVERNOR_SHIFT_2,
        SYMBOL_GOVERNOR_SHIFT_3
    };
    int value = GetValue();
    while (value < 0)
    {
        value += 4;
    }
    return chars[value % 4];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Choice::ProcessKey(KeyEvent event)
{
    if (event.type == TypePress::Press)
    {
        Key::E key = event.key;

        int delta = (key == Key::Down || key == Key::Right) ? 1 : -1;

        ChangeIndex(Menu::IsShown() ? delta : -delta);

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::HeightOpened() const
{
    return MOI_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * MOSI_HEIGHT - 5;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::ChangeIndex(int delta) const
{
    int index = *cell;
    if (delta > 0)
    {
        ++index;
        if (index == NumSubItems())
        {
            index = 0;
        }
    }
    else
    {
        --index;
        if (index < 0)
        {
            index = NumSubItems() - 1;
        }
    }
    *cell = (int8)index;
    Change(IsActive());
    Beeper::GovernorChangedValue();
    Osci::Display::SetFlagRedraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::KeyRelease() const
{
    Item::KeyRelease();

    if (!IsActive())
    {
        Change(false);
    }
    else if (!IsOpened())
    {
        StartChange(1);
    }
    else
    {
        ChangeIndex(1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    if (!IsCurrentItem())
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::StartChange(int delta) const
{
    if (tsChoice.address == 0)
    {
        Beeper::GovernorChangedValue();

        if (HINT_MODE_ENABLED)
        {
            Menu::SetItemForHint(this);
        }
        else if (!IsActive())
        {
            Change(false);
        }
        else
        {
            tsChoice.address = (void *)this;
            tsChoice.timeStart = TIME_MS;
            tsChoice.dir = delta > 0 ? INCREASE : DECREASE;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Choice::GetSymbol()
{
    return ((Governor*)this)->GetSymbol();  // -V1027
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Choice::Step() const //-V2506
{
    static const float speed = 0.1F;
    static const int numLines = 12;

    if (tsChoice.address == this)
    {
        float delta = speed * (TIME_MS - tsChoice.timeStart);
        if (delta == 0.0F)  // -V550 //-V2550 //-V550
        {
            delta = 0.001F; // ������ � ��������� ������ ������ ����� ��������, ��� ������ 0 ��, �� �� ���������� ������� �����, ������ ��� ���� ����� �������� � ���, ��� �������� ���
        }
        int8 index = *cell;

        if (tsChoice.dir == INCREASE)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            Math::CircleIncrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        else if (tsChoice.dir == DECREASE)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }
            Math::CircleDecrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        else
        {
            // NONE
        }

        *cell = index;
        tsChoice.address = 0;
        Change(IsActive());
        Osci::Display::SetFlagRedraw();
        tsChoice.dir = NONE;
        return 0.0F;
    }

    return 0.0F;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Choice::NameCurrentSubItem() const
{
    return ((int8 *)cell == 0) ? String("") : String(NAME_FROM_INDEX(*cell));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameNextSubItem() const
{
    if (cell == 0)
    {
        return "";
    }

    int index = *((int8 *)cell) + 1;

    if (index == NumSubItems())
    {
        index = 0;
    }
    return NAME_FROM_INDEX(index);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NamePrevSubItem() const
{
    if (cell == 0)
    {
        return "";
    }

    int index = *((int8 *)cell) - 1;

    if (index < 0)
    {
        index = NumSubItems() - 1;
    }
    return NAME_FROM_INDEX(index);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Choice::NameSubItem(int i) const
{
    return String(NAME_FROM_INDEX(i));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Choice::ColorMenuField(const Choice *choice)
{
    // ��������� �� �������
    if (choice == PageFrequencyCounter::GetChoiceTimeF())
    {
        return Color(Color::WHITE);
    }
    // ��������� �� �������
    else if (choice == PageFrequencyCounter::GetChoiceFreqClc() || choice == PageFrequencyCounter::GetChoiceNumPeriods())
    {
        return Color(Color::YELLOW);
    }
    else
    {
        // ����� ������ �� ������
    }

    return Color::MenuItemField();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GovernorColor::KeyRelease() const
{
    Item::KeyRelease();

    if (IsActive())
    {
        if (Menu::OpenedItem() == this)
        {
            Math::CircleIncrease<int8>(&ct->currentField, 0, 3);
        }
        else
        {
            if (!IsCurrentItem())
            {
                SetCurrent(true);
            }
            Open(!IsOpened());
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    if (!IsCurrentItem())
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
}

