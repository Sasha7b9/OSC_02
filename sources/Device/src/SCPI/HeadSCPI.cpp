#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageService.h"
#include "SCPI/ChannelsSCPI.h"
#include "SCPI/DisplaySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/TBaseSCPI.h"


// *IDN?
static const char *FuncIDN(const char *);
static bool TestIDN();
static void HintIDN(String *);
// *RST
static const char *FuncReset(const char *);
static bool TestReset();
static void HintReset(String *);
// :HELP
static const char *FuncHelp(const char *);
static bool TestHelp();
static void HintHelp(String *);
// :TEST
static const char *FuncTest(const char *);
static bool TestTest();
static void Process(const StructSCPI strct[], String message);
static void HintTest(String *);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",     FuncIDN,        TestIDN,   "ID request", HintIDN),
    SCPI_LEAF("*RST",      FuncReset,      TestReset, "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",     FuncHelp,       TestHelp,  "Output of this help", HintHelp),
    SCPI_LEAF(":TEST",     FuncTest,       TestTest,  "Run all tests", HintTest),
    SCPI_NODE(":CHANNEL",  SCPI::channels),
    SCPI_NODE(":DISPLAY",  SCPI::display),
    SCPI_NODE(":KEY",      SCPI::key),
    SCPI_NODE(":TIMEBASE", SCPI::tBase),
    SCPI_EMPTY()
};


static const char *FuncIDN(const char *buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer("MNIPI, S8-57, v.1.2");

    SCPI_EPILOG(buffer)
}


static void HintIDN(String *message)
{
    SCPI::SendAnswer(message->c_str());
}


static const char *FuncReset(const char *buffer)
{
    SCPI_PROLOG(buffer)
        
    PageService::OnPress_ResetSettings();

    SCPI_EPILOG(buffer)
}


static void HintReset(String *message)
{
    SCPI::SendAnswer(message->c_str());
}


static const char *FuncHelp(const char *buffer)
{
    SCPI_PROLOG(buffer);
    
    String message;

    Process(SCPI::head, message);

    SCPI_EPILOG(buffer);
}


static void HintHelp(String *message)
{
    SCPI::SendAnswer(message->c_str());
}


static const char *FuncTest(const char *buffer)
{
    static const char *const modes[2] =
    {
        " FULL",
        " FAST"
    };

    for (int i = 0; i < 2; i++)
    {
        const char *end = SCPI::BeginWith(buffer, modes[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            PageDebug::PageTests::OnPress_Run();

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintTest(String *message)
{
    SCPI::SendAnswer(message->c_str());
}


static bool TestIDN()
{
    return true;
}


static bool TestReset()
{
    return true;
}


static bool TestHelp()
{
    return true;
}


static bool TestTest()
{
    return true;
}


static void Process(const StructSCPI strct[], String msg)
{
    while(!strct->IsEmpty())
    {
        if(strct->IsNode())
        {
            String message(msg);
            message.Append(strct->key);
            Process(strct->strct, message);
        }
        else if(strct->IsLeaf())
        {
            String message(msg);
            message.Append(strct->key);
            SCPI::SendAnswer(strct->hint);
            strct->funcHint(&message);
            SCPI::SendAnswer("");
        }
        else
        {
            LOG_ERROR("���� �� �� ����� �������");
        }

        strct++;
    }
}
