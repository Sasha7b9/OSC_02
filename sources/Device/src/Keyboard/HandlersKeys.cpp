#include "defines.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/HandlersKeys.h"
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Osci/Display/DisplayOsci.h"


// �������������� �������
static KeyEvent event;

// �����, ��������� �������� ����� �������� ��������
static Chan drawingChan(Chan::A);

// ����� ���������� ��������� ��������� ������ - �������� ��� ��������
static void OnChangeParameterChannel(pFuncVChI16, Chan::E, int16);

// ������ ����������
static void Empty();

// ����� ���������� ��������� ��������. ���������� true, ���� ��������� � ���������� ��������� ������� ���������� �� ���������.
static bool CommonHandlerPage();

// ����� ���������� ��������� ��������� ����������
static void OnChangeParameterTime(pFuncVI, int);

// ��������� �������� ��� ��������� ���� � ����������� �� ����, ����� �������� ������ ��������
static void ShowHidePage(const Page *page);

static void ChangeRange(Chan::E ch, int16 delta);

static void ChangeTShift(int delta);

static void ChangeTBase(int delta);

static void ChangeRShift(Chan::E ch, int16 delta);

// ����������� ������� ������
static void OnArrow();     // Key::Left, Key::Up, Key::Right, Key::Down
static void OnEnter();     // Key::Enter
static void OnTime();      // Key::Time
static void OnStart();     // Key::Start
static void OnFunction();  // Key::Function
static void OnService();   // Key::Service
static void OnMeasures();  // Key::Measure
static void OnDisplay();   // Key::Display
static void OnMemory();    // Key::Memory
static void OnChannelA();  // Key::ChannelA
static void OnChannelB();  // Key::ChannelB
static void OnTrig();      // Key::Trig
static void OnTrigLev();   // Key::TrigLevLess, Key::TrigLevMore
static void OnRangeA();    // Key::RangeLessA, Key::RangeMoreA
static void OnRangeB();    // Key::RangeLessB, Key::RangeMoreB
static void OnRShiftA();   // Key::RShiftLessA, Key::RShiftMoreB
static void OnRShiftB();   // Key::RShiftLessB, Key::RShiftMoreB
static void OnTBase();     // Key::TBaseLess, Key::TBase::More
static void OnTShift();    // Key::TShiftLess, Key::TShiftMore
static void OnFX();        // Key::F1, Key::F2, Key::F3, Key::F4, Key::F5


void Handlers::Process(KeyEvent e)
{
    event = e;

    static const pFuncVV func[Key::Count][4] =
    { // Press       Repead       Release      Long
        {Empty,      Empty,       Empty,       Empty},       // None       
        {Empty,      Empty,       OnFunction,  Empty},       // Function   
        {Empty,      Empty,       OnMeasures,  Empty},       // Measure    
        {Empty,      Empty,       OnMemory,    Empty},       // Memory     
        {Empty,      Empty,       OnService,   Empty},       // Service    
        {Empty,      Empty,       OnChannelA,  OnChannelA},  // ChannelA   
        {Empty,      Empty,       OnChannelB,  OnChannelB},  // ChannelB   
        {Empty,      Empty,       OnTime,      OnTime},      // Time       
        {OnStart,    Empty,       Empty,       Empty},       // Start      
        {Empty,      Empty,       OnTrig,      OnTrig},      // Trig       
        {Empty,      Empty,       OnDisplay,   Empty},       // Display    
        {OnRangeA,   Empty,       Empty,       Empty},       // RangeMoreA 
        {OnRangeA,   Empty,       Empty,       Empty},       // RangeLessA 
        {OnRShiftA,  OnRShiftA,   Empty,       Empty},       // RShiftMoreA
        {OnRShiftA,  OnRShiftA,   Empty,       Empty},       // RShiftLessA
        {OnRangeB,   Empty,       Empty,       Empty},       // RangeMoreB 
        {OnRangeB,   Empty,       Empty,       Empty},       // RangeLessB 
        {OnRShiftB,  OnRShiftB,   Empty,       Empty},       // RShiftMoreB
        {OnRShiftB,  OnRShiftB,   Empty,       Empty},       // RShiftLessB
        {OnTBase,    Empty,       Empty,       Empty},       // TBaseMore
        {OnTBase,    Empty,       Empty,       Empty},       // TBaseLess
        {OnTShift,   OnTShift,    Empty,       Empty},       // TShiftMore 
        {OnTShift,   OnTShift,    Empty,       Empty},       // TShiftLess 
        {OnTrigLev,  OnTrigLev,   Empty,       Empty},       // TrigLevMore
        {OnTrigLev,  OnTrigLev,   Empty,       Empty},       // TrigLevLess
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Left       
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Right      
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Up         
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Down       
        {Empty,      Empty,       OnEnter,     OnEnter},     // Enter      
        {OnFX,       Empty,       OnFX,        OnFX},        // F1
        {OnFX,       Empty,       OnFX,        OnFX},        // F2
        {OnFX,       Empty,       OnFX,        OnFX},        // F3
        {OnFX,       Empty,       OnFX,        OnFX},        // F4
        {OnFX,       Empty,       OnFX,        OnFX}         // F5
    };

    Key::E       key = e.key;
    TypePress::E type = e.type;

    if (key < Key::Count && type < TypePress::None)
    {
        if (!CommonHandlerPage())
        {
            func[key][type]();
        }
    }
}

static void Empty()
{
}

static void ChangeRShift(Chan::E ch, int16 delta)
{
    if (!Device::InModeRecorder())
    {
        static bool stop[Chan::Count] = { false, false };      // ������� ����, ��� �������� �������� �� ����� - ��� ����� ���� � ������ ���� �������
        static uint timeStop[Chan::Count] = { 0, 0 };          // ����� �������� �������� stop

        if (stop[ch])
        {
            if (TIME_MS - timeStop[ch] > 500)
            {
                stop[ch] = false;
            }
        }
        else
        {
            RShift(ch).Change(delta);

            if (RShift(ch) == 0)
            {
                stop[ch] = true;
                timeStop[ch] = TIME_MS;
            }

            DisplayOsci::SetFlagRedraw();
        }
    }
}


static void OnRShiftA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, (event.key == Key::RShiftMoreA) ? 1 : -1);
}


static void OnRShiftB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, (event.key == Key::RShiftMoreB) ? 1 : -1);
}


static void ChangeRange(Chan::E ch, int16 delta)
{
    Range(ch).Change(delta);
}


static void OnRangeA()
{
    OnChangeParameterChannel(ChangeRange, Chan::A, (event.key == Key::RangeMoreA) ? 1 : -1);
}


static void OnRangeB()
{
    OnChangeParameterChannel(ChangeRange, Chan::B, (event.key == Key::RangeMoreB) ? 1 : -1);
}


static void OnChangeParameterChannel(pFuncVChI16 func, Chan::E ch, int16 delta)
{
    if (Device::InModeOsci())
    {
        drawingChan = Chan(ch);
    }

    func(ch, delta);
}


static void OnChangeParameterTime(pFuncVI func, int delta)
{
    func(delta);
}


static void ChangeTShift(int delta)
{
    static int prevDelta = 0;               // ���������� ����������� �����������
    static uint timeStartBrake = 0;         // ����� ������ ����������

    if (event.IsRepeat() &&                 // ���� ��������� �� ����������� �������� ������
        (prevDelta == delta) &&             // � ��� �� �����������, ��� � � ������� ���
        (timeStartBrake != 0) &&            // � "������" �������
        (TIME_MS - timeStartBrake < 500))   // � ������ ��� ���� �������
    {
        return;                             // �� ������ �� ������
    }

    prevDelta = delta;
    timeStartBrake = 0;

    TShift().Change(delta);

    if ((set.time.shift == 0) && event.IsRepeat())   // ���� ����� ������� �������� - ����, �� �������� ����������
    {
        timeStartBrake = TIME_MS;
    }
}


static void OnTShift()
{
    OnChangeParameterTime(ChangeTShift, (event.key == Key::TShiftMore) ? 1 : -1);
}


static void ChangeTBase(int delta)
{
    TBase().Change(delta);
}


static void OnTBase()
{
    int delta = (event.key == Key::TBaseMore) ? 1 : -1;

    if (Device::InModeRecorder())
    {
        OnChangeParameterTime(Recorder::ScaleX::Change, delta);
    }
    else
    {
        OnChangeParameterTime(ChangeTBase, delta);
    }
}


static void OnFX()
{
    if (Menu::IsShown())
    {
        const Item *underKey = Menu::ItemUnderFunctionalKey(event.key);     // ������� ������� ����, ��������������� ������� ������

        if((underKey == &Item::empty && !Menu::OpenedItem()->IsPage()) && (event.IsRelease() || event.IsLong()) )
        {
            Menu::CloseOpenedItem();
        }
        else
        {
            underKey->HandlerFX(event.type);
        }
    }
}


static void OnArrow()
{
    Item *openedItem = Menu::OpenedItem();

    if (Menu::IsShown())
    {
        if (!openedItem->Is(TypeItem::Page))
        {
            openedItem->HandlerKey(event);
        }
    }
    else if (openedItem == PageFFT::Cursors::self)
    {
        openedItem->HandlerKey(event);
    }
    else
    {
        // ����� ������
    }
}


static bool CommonHandlerPage()
{
    bool result = false;

    if (Menu::IsShown())
    {
        Item *openedPage = Menu::OpenedItem();

        if (!openedPage->Is(TypeItem::Page))
        {
        }
        else if (Menu::CurrentItem()->HandlerKey(event))
        {
            result = true;
        }
        else
        {
            result = openedPage->HandlerKey(event);
        }
    }

    return result;
}


static void OnEnter()
{
    if (event.IsRelease())
    {
        if (!Menu::IsShown())
        {
            Menu::Show();
        }
        else
        {
            Menu::CloseOpenedItem();
        }
    }
    else if (event.IsLong())
    {
        return Menu::IsShown() ? Menu::Hide() : Menu::Show();
    }
    else
    {
        // ����� ������
    }
}


static void OnTrigLev()
{
    TrigLevel().Change(event.key == Key::TrigLevMore ? 1 : -1);
}


static void OnChannelA()
{
    if (event.IsRelease())
    {
        ShowHidePage(PageChannelA::self);
    }
    else if (event.IsLong())
    {
        RShift(Chan::A).Set(0);
    }
    else
    {
        // ����� ������
    }
}


static void OnChannelB()
{
    if (event.IsRelease())
    {
        ShowHidePage(PageChannelB::self);
    }
    else if (event.IsLong())
    {
        RShift(Chan::B).Set(0);
    }
    else
    {
        // ����� ������
    }
}


static void OnFunction()
{
    ShowHidePage(PageFunction::self);
}


static void OnMeasures()
{
    ShowHidePage(PageMeasures::self);
}


static void OnMemory()
{
    if (FDrive::IsConnected())
    {
        Display::SaveScreenToDrive();
    }
    else
    {
        ShowHidePage(PageMemory::self);
    }
}


static void OnService()
{
    ShowHidePage(PageService::self);
}


static void OnTime()
{
    if (event.IsRelease())
    {
        ShowHidePage(PageTime::self);
    }
    else if (event.IsLong())
    {
        TShift().Reset();
    }
    else
    {
        // ����� ������
    }
}


static void OnStart()
{
    if (Device::InModeTester())
    {
        Tester::StartStop();
    }
    else
    {
        Osci::OnPressStart();
    }
}


static void OnTrig()
{
    if (event.IsRelease())
    {
        ShowHidePage(PageTrig::self);
    }
    else if (event.IsLong())
    {
        TrigLevel().Set(0);
    }
    else
    {
        // ����� ������
    }
}


static void OnDisplay()
{
    ShowHidePage(PageDisplay::self);
}


static void ShowHidePage(const Page *page)
{
    if (Menu::OpenedItem() == page)
    {
        Menu::IsShown() ? Menu::Hide() : Menu::Show();
    }
    else
    {
        Menu::SetMainPage(page);

        if (!Menu::IsShown())
        {
            Menu::Show();
        }
    }
}
