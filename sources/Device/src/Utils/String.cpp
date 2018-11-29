#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "String.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String::String() : buffer(0)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String(char *format, ...)
{
    char buf[100];

    va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    if (Allocate(std::strlen(buf) + 1))
    {
        std::strcpy(buffer, buf);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String::~String()
{
    std::free(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *String::CString() const
{
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::Allocate(uint size)
{
    buffer = (char *)std::malloc(size);
    if (buffer)
    {
        return true;
    }
    else
    {
        LOG_ERROR("�� ������� ������");
    }
    return false;
}
