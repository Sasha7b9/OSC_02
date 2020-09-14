#include "defines.h"
#include "log.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include <cstring>


// :DISPLAY:ACCUMULATION
static pCHAR FuncAccumulation(pCHAR);
static void HintAccumulation(String *);
static bool TestAccumulation();

// :DISPLAY:AVERAGES
static pCHAR FuncAverages(pCHAR);
static void HintAverages(String *);
static bool TestAverages();

// :DISPLAY:BRIGHTNESS
static pCHAR FuncBrightness(pCHAR);
static void HintBrightness(String *);
static bool TestBrightness();

// :DISPLAY:FPS
static pCHAR FuncFPS(pCHAR);
static void HintFPS(String *);
static bool TestFPS();

// :DISPLAY:GRID
static pCHAR FuncGrid(pCHAR);
static void HintGrid(String *);
static bool TestGrid();

// :DISPLAY:MAPPING
static pCHAR FuncMapping(pCHAR);
static void HintMapping(String *);
static bool TestMapping();

// :DISPLAY:SMOOTHING
static pCHAR FuncSmoothing(pCHAR);
static void HintSmoothing(String *);
static bool TestSmoothing();


static const char *const grids[] =
{
    " TYPE1",
    " TYPE2",
    " TYPE3",
    " TYPE4",
    ""
};


static pString mapping[] =
{
    " LINES",
    " DOTS",
    ""
};


const StructSCPI SCPI::display[] =
{
    SCPI_LEAF(":ACCUMULATION", FuncAccumulation, TestAccumulation, "",                       HintAccumulation),
    SCPI_LEAF(":AVERAGE",      FuncAverages,     TestAverages,     "Number of averages",     HintAverages),
    SCPI_LEAF(":BRIGHTNESS",   FuncBrightness,   TestBrightness,   "",                       HintBrightness),
    SCPI_LEAF(":FPS",          FuncFPS,          TestFPS,          "",                       HintFPS),
    SCPI_LEAF(":GRID",         FuncGrid,         TestGrid,         "Grid type selection",    HintGrid),
    SCPI_LEAF(":MAPPING",      FuncMapping,      TestMapping,      "Signal display control", HintMapping),
    SCPI_LEAF(":SMOOTHING",    FuncSmoothing,    TestSmoothing,    "",                       HintSmoothing),
    SCPI_EMPTY()
};


static pString accumulation[] =
{
    " OFF",
    " 2",
    " 4",
    " 8",
    " 16",
    " 32",
    " 64",
    " 128",
    " INFINITY",
    ""
};


static pCHAR FuncAccumulation(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(accumulation[S_DISP_ENUM_ACCUM]));

    SCPI_PROCESS_ARRAY(accumulation, S_DISP_ENUM_ACCUM = static_cast<ENumAccum::E>(i));
}


static pString averages[] =
{
    " 128",
    " 16",
    " 1",
    " 256",
    " 2",
    " 4",
    " 8",
    " 32",
    " 64",
    ""
};


static pCHAR FuncAverages(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(averages[S_OSCI_ENUM_AVERAGE]));

    SCPI_PROCESS_ARRAY(averages, ENumAverage::Set(static_cast<ENumAverage::E>(i)));
}


static pCHAR FuncBrightness(pCHAR)
{
    return nullptr;
}


static const char *const fps[] =
{
    " 25",
    " 10",
    " 5",
    " 2",
    " 1",
    ""
};

static pCHAR FuncFPS(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(fps[S_DISP_ENUM_FPS]));

    SCPI_PROCESS_ARRAY(fps, S_DISP_ENUM_FPS = static_cast<ENumSignalsInSec::E>(i));
}


static const char *FuncGrid(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(grids[S_DISP_TYPE_GRID]));

    SCPI_PROCESS_ARRAY(grids, PageDisplay::SetTypeGrid(static_cast<TypeGrid::E>(i)));
}


static const char *FuncMapping(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(mapping[S_DISP_MAPPING]));

    SCPI_PROCESS_ARRAY(mapping, S_DISP_MAPPING = static_cast<DisplayMapping::E>(i));
}


static pString smoothings[] =
{
    " 1",
    " 2",
    " 3",
    " 4",
    " 5",
    " 6",
    " 7",
    " 8",
    " 9",
    " 10",
    ""
};


static pCHAR FuncSmoothing(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(smoothings[S_DISP_ENUM_SMOOTH]));

    SCPI_PROCESS_ARRAY(smoothings, S_DISP_ENUM_SMOOTH = static_cast<ENumSmoothing::E>(i));
}


static void HintAccumulation(String *message)
{
    SCPI::ProcessHint(message, accumulation);
}


static void HintAverages(String *message)
{
    SCPI::ProcessHint(message, averages);
}


static void HintBrightness(String *)
{

}


static void HintFPS(String *message)
{
    SCPI::ProcessHint(message, fps);
}


static void HintGrid(String *message)
{
    SCPI::ProcessHint(message, grids);
}


static void HintMapping(String *message)
{
    SCPI::ProcessHint(message, mapping);
}


static void HintSmoothing(String *message)
{
    SCPI::ProcessHint(message, averages);
}


static bool TestAccumulation()
{
    return false;
}


static bool TestAverages()
{
    return false;
}


static bool TestBrightness()
{
    return false;
}


static bool TestFPS()
{
    return false;
}


static bool TestGrid()
{
    return false;
}


static bool TestMapping()
{
    String commandLines(":DISPLAY:MAPPING LINEs%c", 0x0D);
    String commandDots(":DISPLAY:MApping dots%c", 0x0D);

    for (int i = 0; i < 10; i++)
    {
        SCPI_APPEND_STRING(commandLines); //-V814

        if (!S_DISP_MAPPING_IS_LINES)
        {
            SCPI_EXIT_ERROR();
        }

        SCPI_APPEND_STRING(commandDots); //-V814

        if (!S_DISP_MAPPING_IS_DOTS)
        {
            SCPI_EXIT_ERROR();
        }
    }

    return true;
}


static bool TestSmoothing()
{
    return false;
}
