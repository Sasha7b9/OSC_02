#include "defines.h"
#include <stm32f4xx.h>
#include "log.h"
#include "HandlersKeys.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTime.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/Pages/Include/PageDisplay.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KeyEvent Handlers::event;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Handlers::Process(KeyEvent e)
{
    event = e;

    static const pFuncVV func[Key::Number][4] =
    {                   // Press        Repead       Release        Long
        /* None        */ {E,           E,           E,             E},
        /* Function    */ {E,           E,           Function,      E},
        /* Measure    */  {Measure,     Measure,     Measure,       Measure},
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

    uint8 code = event.key;
    uint8 type = event.type;

    if (code < Key::Number && type < TypePress::None)
    {
        func[event.key][event.type]();
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
void Handlers::RShiftLessA()
{
    FPGA::RShiftChange(Chan::A, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RShiftMoreA()
{
    FPGA::RShiftChange(Chan::A, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RShiftLessB()
{
    FPGA::RShiftChange(Chan::B, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RShiftMoreB()
{
    FPGA::RShiftChange(Chan::B, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RangeLessA()
{
    FPGA::DecreaseRange(Chan::A);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RangeMoreA()
{
    FPGA::IncreaseRange(Chan::A);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RangeLessB()
{
    FPGA::DecreaseRange(Chan::B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::RangeMoreB()
{
    FPGA::IncreaseRange(Chan::B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TShiftLess()
{
    FPGA::TShiftChange(-1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TShiftMore()
{
    FPGA::TShiftChange(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TBaseLess()
{
    FPGA::DecreaseTBase();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::TBaseMore()
{
    FPGA::IncreaseTBase();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::FuncRelease()
{
    Menu::ReleaseFunctionalButton(event.key);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::FuncLong()
{
    Menu::LongFunctionalButton(event.key);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::Arrow()
{
    Control *item = Menu::OpenedItem();

    TypePress type = event.type;

    if(type != TypePress::Press)
    {
        return;
    }

    if(IS_PAGE(item))
    {
        Page *page = (Page *)item;

        switch (event.key)
        {
            case Key::Left:
                if(!page->funcRegSet(-1))
                {
                    page->ChangeOpened(-1);
                }
                break;
            case Key::Right:
                if(!page->funcRegSet(1))
                {
                    page->ChangeOpened(1);
                }
                break;
            case Key::Up:
            case Key::Down:
                break;
        }
    }
    else if(IS_CHOICE(item))
    {
        Choice *choice = (Choice *)item;

        switch(event.key)
        {
            case Key::Up:
                choice->ChangeIndex(-1);
                break;
            case Key::Down:
                choice->ChangeIndex(1);
                break;
            case Key::Left:
            case Key::Right:
                break;
        }
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
    OpenPage(PageMemory::pointer);
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
    ((Page *)page)->SetCurrentPage();
    ((Page *)page)->Open(true);
    Menu::Show(true);
}
