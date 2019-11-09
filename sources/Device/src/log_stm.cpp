#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include "Utils/String.h"


#define SIZE_BUFFER 100


void Log::Message(const char *message)
{
    Console::AddString(const_cast<char *>(message));
}


void Log::Message(const char *file, int line, const char *message)
{
    Console::AddString(String("%s %d %s", file, line, message).CString());
}
