#include "defines.h"
#include "device.h"
#include <stm32f4xx.h>
#include "log.h"
#include "HandlersKeys.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTime.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/BottomPart.h"
#include "Settings/Settings.h"
#include "Utils/String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void(*pFuncVCh)(Chan::E);
typedef void(*pFuncVChI)(Chan::E, int);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �������������� �������
static KeyEvent event;
/// �����, ��������� �������� ����� �������� ��������
static Chan drawingChan = Chan(Chan::A);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void RangeMoreB();
/// ���������� ���������� �������������� ������
static void FuncRelease();
/// ���������� ����������� ������� �������������� ������
static void FuncLong();
/// ���������� ������� �������
static void Arrow();
/// ����� ���������� ��������� ��������� ������ - �������� ��� ��������
static void OnChangeParameterChannel(pFuncVChI, Chan::E, int);
/// ������� ��������� ���������� ������
static void DrawParametersChannel();
/// ����� ���������� ��������� ��������� ����������
static void OnChangeParameterTime(pFuncVI, int);
/// ������� ��������� ��������� ����������
static void DrawParametersTime();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Handlers::Process(KeyEvent e)
{
    event = e;

    static const pFuncVV func[Key::Number][4] =
    {                   // Press        Repead       Release        Long
        /* None        */ {E,           E,           E,             E},
        /* Function    */ {E,           E,           Function,      E},
        /* Measure     */ {Measure,     Measure,     Measure,       Measure},
        /* Memory      */ {Memory,      Memory,      Memory,        Memory},
        /* Service     */ {Service,     Service,     Service,       Service},
        /* ChannelA    */ {ChannelA,    E,           E,             E},
        /* ChannelB    */ {ChannelB,    E,           E,             E},
        /* Time        */ {Time,        Time,        Time,          Time},
        /* Start       */ {Start,       E,           E,             E},
        /* Trig        */ {Trig,        Trig,        Trig,          Trig},
        /* Display     */ {Display,     Display,     Display,       Display},
        /* RangeMoreA  */ {RangeMoreA,  E,           E,             E},
        /* RangeLessA  */ {RangeLessA,  E,           E,             E},
        /* RShiftMoreA */ {RShiftMoreA, RShiftMoreA, E,             E},
        /* RShiftLessA */ {RShiftLessA, RShiftLessA, E,             E},
        /* RangeMoreB  */ {RangeMoreB,  E,           E,             E},
        /* RangeLessB  */ {RangeLessB,  E,           E,             E},
        /* RShiftMoreB */ {RShiftMoreB, RShiftMoreB, E,             E},
        /* RShiftLessB */ {RShiftLessB, RShiftLessB, E,             E},
        /* TBaseMore   */ {TBaseMore,   E,           E,             E},
        /* TBaseLess   */ {TBaseLess,   E,           E,             E},
        /* TShiftMore  */ {TShiftMore,  TShiftMore,  E,             E},
        /* TShiftLess  */ {TShiftLess,  TShiftLess,  E,             E},
        /* TrigLevMore */ {TrigLevMore, TrigLevMore, E,             E},
        /* TrigLevLess */ {TrigLevLess, TrigLevLess, E,             E},
        /* Left        */ {Arrow,       Arrow,       Arrow,         Arrow},
        /* Right       */ {Arrow,       Arrow,       Arrow,         Arrow},
        /* Up          */ {Arrow,       Arrow,       Arrow,         Arrow},
        /* Down        */ {Arrow,       Arrow,       Arrow,         Arrow},
        /* Enter       */ {E,           E,           EnterRelease,  EnterLong},
        /* F1          */ {E,           E,           FuncRelease,   FuncLong},
        /* F2          */ {E,           E,           FuncRelease,   FuncLong},
        /* F3          */ {E,           E,           FuncRelease,   FuncLong},
        /* F4          */ {E,           E,           FuncRelease,   FuncLong},
        /* F5          */ {E,           E,           FuncRelease,   FuncLong}
    };

    Key::E code = event.key;
    TypePress::E type = event.type;

    if (code < Key::Number && type < TypePress::None)
    {
        func[code][type]();
    }
    else
    {
        /// \todo ��������� �������. ��������, ����� ������ �� ����
        //LOG_WRITE("������ ����� ������� ������");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::E()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ChangeRShift(Chan::E ch, int delta)
{
    static bool stop[Chan::Number] = { false, false };      // ������� ����, ��� �������� �������� �� ����� - ��� ����� ���� � ������ ���� �������
    static uint timeStop[Chan::Number] = { 0, 0 };          // ����� �������� �������� stop

    if (stop[ch])
    {
        if (TIME_MS - timeStop[ch] > 500)
        {
            stop[ch] = false;
        }
        return;
    }
    else
    {
        FPGA::RShiftChange(ch, delta);
    }

    if (SET_RSHIFT(ch) == RShift::ZERO)
    {
        stop[ch] = true;
        timeStop[ch] = TIME_MS;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RShiftLessA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RShiftMoreA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RShiftLessB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RShiftMoreB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RangeLessA()
{
    OnChangeParameterChannel(FPGA::ChangeRange, Chan::A, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RangeMoreA()
{
    OnChangeParameterChannel(FPGA::ChangeRange, Chan::A, +1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RangeLessB()
{
    OnChangeParameterChannel(FPGA::ChangeRange, Chan::B, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RangeMoreB()
{
    OnChangeParameterChannel(FPGA::ChangeRange, Chan::B, +1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawParametersTime()
{
    int x = Grid::Left() + 5;
    int y = Grid::Top() + 5;
    int width = 126;
    int height = 15;

    Painter::DrawBoundedRegion(x, y, width, height, Color::BACK, Color::FILL);

    String("���� : %s", TBase(SET_TBASE).Name()).Draw(x + 3, y + 3, Color::FILL);

    TShift shift(SET_TSHIFT);

    shift.ToString(SET_TBASE).Draw(x + 64, y + 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawParametersChannel()
{
    int x = Grid::Left() + 5;
    int y = Grid::Top() + 5;
    int width = 126;
    int height = 15;
    Chan::E ch = drawingChan.value;

    Painter::DrawBoundedRegion(x, y, width, height, Color::BACK, Color::Channel(ch));

    char buffer[50];
    sprintf(buffer, "%s : %s %s",
        drawingChan.Name(),
        ModeCouple(SET_COUPLE(ch)).UGO(),
        Range(SET_RANGE(ch)).Name()
    );

    Painter::DrawBigText(x + 3, y + 3, 1, buffer, Color::Channel(ch));

    Painter::DrawBigText(x + 80, y + 3, 1, RShift::ToString(SET_RSHIFT(ch), SET_RANGE(ch), SET_DIVIDER(ch)).CString());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChangeParameterChannel(pFuncVChI func, Chan::E ch, int delta)
{
    if (Device::CurrentMode() == Device::Mode::Osci)
    {
        drawingChan = Chan(ch);

        Display::SetAddDrawFunction(DrawParametersChannel, 5000);
    }

    func(ch, delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChangeParameterTime(pFuncVI func, int delta)
{
    if (Device::CurrentMode() == Device::Mode::Osci)
    {
        Display::SetAddDrawFunction(DrawParametersTime, 5000);
    }

    func(delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TShiftLess()
{
    OnChangeParameterTime(FPGA::TShiftChange, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TShiftMore()
{
    OnChangeParameterTime(FPGA::TShiftChange, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TBaseLess()
{
    OnChangeParameterTime(FPGA::TBaseChange, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TBaseMore()
{
    OnChangeParameterTime(FPGA::TBaseChange, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncRelease()
{
    Menu::ReleaseFunctionalButton(event.key);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncLong()
{
    Menu::LongFunctionalButton(event.key);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Arrow()
{
    if(!Menu::CurrentItem()->ProcessKey(event))
    {
        Menu::OpenedItem()->ProcessKey(event);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::EnterRelease()
{
    if (!Menu::IsShown())
    {
        Menu::Show(true);
    }
    else
    {
        Menu::CloseOpenedItem();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::EnterLong()
{
    Menu::Show(!Menu::IsShown());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TrigLevMore()
{
    FPGA::TrigLevChange(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TrigLevLess()
{
    FPGA::TrigLevChange(-1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::ChannelA()
{
    //SET_ENABLED_A = !SET_ENABLED_A;

    OpenPage(PageChannelA::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::ChannelB()
{
    //SET_ENABLED_B = !SET_ENABLED_B;

    OpenPage(PageChannelB::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Function()
{
    OpenPage(PageFunction::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Measure()
{
    OpenPage(PageMeasures::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Memory()
{
    if (MODE_BTN_MEMORY_IS_SAVE)
    {
        if(event.type == TypePress::Press)
        Painter::SaveScreenToDrive();
    }
    else
    {
        OpenPage(PageMemory::pointer);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Service()
{
    OpenPage(PageService::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Time()
{
    OpenPage(PageTime::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Start()
{
    FPGA::OnPressStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Trig()
{
    OpenPage(PageTrig::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Display()
{
    OpenPage(PageDisplay::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::OpenPage(const PageBase *page)
{
    ((Page *)page)->SetAsCurrent();
    ((Page *)page)->Open(true);
    Menu::Show(true);
}
