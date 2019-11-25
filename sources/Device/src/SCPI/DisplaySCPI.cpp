#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "SCPI/DisplaySCPI.h"
#include "Settings/Settings.h"
#include <cstring>


// :DISPLAY:MAPPING
static const char *FuncMapping(const char *);


static bool TestMapping();


static const char *const mapping[] =
{
    " LINES",
    " DOTS",
    ""
};


const StructSCPI SCPI::display[] =
{
    SCPI_LEAF(":MAPPING", FuncMapping, TestMapping),
    SCPI_EMPTY()
};


static const char *FuncMapping(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(mapping[DisplayMapping()]));

    SCPI_PROCESS_ARRAY(mapping, DisplayMapping(static_cast<DisplayMapping::E>(i)));
}


bool TestMapping()
{
    String commandLines(":DISPLAY:MAPPING LINEs%c", 0x0D);
    String commandDots(":DISPLAY:MApping dots%c", 0x0D);

    for(int i = 0; i < 10; i++)
    {
        SCPI_APPEND_STRING(commandLines); //-V814

        if(DisplayMapping() != DisplayMapping::Lines)
        {
            SCPI_EXIT_ERROR();
        }

        SCPI_APPEND_STRING(commandDots); //-V814

        if(DisplayMapping() != DisplayMapping::Dots)
        {
            SCPI_EXIT_ERROR();
        }
    }

    return true;
}
