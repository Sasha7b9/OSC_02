#include "defines.h"
#include "log.h"
#include "Osci/ParametersOsci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include <cstdlib>
#include <cstring>


#define EXTRACT_CHANNEL(x) Chan::E ch = (*(buffer - (x)) == '1') ? ChanA : ChanB;    /* (buffer - 7) ��������� �� ����� ������ - 1 ��� 2 */


// :CHANNEL{1|2}:DISPLAY
static const char *FuncDisplay(const char *);
static bool TestDisplay();
static void HintDisplay(String *);


// :CHANNEL{1|2}:SCALE
static const char *FuncScale(const char *);
static bool TestScale();
static void HintScale(String *);


static const char *displays[] =
{
    " OFF",
    " ON",
    ""
};


static const char *const rangeName[] =
{
    " 2MV",
    " 5MV",
    " 10MV",
    " 20MV",
    " 50MV",
    " 100MV",
    " 200MV",
    " 500MV",
    " 1V",
    " 2V",
    " 5V",
    " 10V",
    " 20V",
    ""
};


static const StructSCPI chan[] =
{
    SCPI_LEAF("DISPLAY", FuncDisplay, TestDisplay, "Turns channel display on/off", HintDisplay),
    SCPI_LEAF("SCALE",   FuncScale,   TestScale,   "Vertical zoom control",        HintScale),
    SCPI_EMPTY()
};


const StructSCPI SCPI::channels[] =
{
    SCPI_NODE("1:", chan),
    SCPI_NODE("2:", chan),
    SCPI_EMPTY()
};


static const char *FuncDisplay(const char *buffer)
{
    EXTRACT_CHANNEL(9);

    SCPI_REQUEST(SCPI::SendAnswer(displays[S_CHANNEL_ENABLED(ch)]));

    SCPI_PROCESS_ARRAY(displays, PageChannel::Enable(ch, i));
}


static const char *FuncScale(const char *buffer)
{
    EXTRACT_CHANNEL(7);

    SCPI_REQUEST(SCPI::SendAnswer(rangeName[S_RANGE(ch)]));

    SCPI_PROCESS_ARRAY(rangeName, Range::Set(ch, static_cast<Range::E>(i)))
}


static void HintDisplay(String *message)
{
    SCPI::ProcessHint(message, displays);
}


static void HintScale(String *message)
{
    SCPI::ProcessHint(message, rangeName);
}


static bool TestDisplay()
{
    return true;
}


static bool TestScale()
{
    for(int i = 0; i < 5; i++)
    {
        Range::E range = static_cast<Range::E>(std::rand() % Range::Count);
        String commandA(":channel1:range%s%c", rangeName[range], 0x0D);

        SCPI_APPEND_STRING(commandA);

        if(S_RANGE_A != range)
        {
            SCPI_EXIT_ERROR();
        }

        range = static_cast<Range::E>(std::rand() % Range::Count);
        String commandB(":channel2:range%s%c", rangeName[range], 0x0D);

        SCPI_APPEND_STRING(commandB);

        if(S_RANGE_B != range)
        {
            SCPI_EXIT_ERROR();
        }

    }

    return true;
}
