#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "DataBuffer.h"
#include "Storage.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Storage::Init()
{
    DataBuffer::Init();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Storage::GetData(DataSettings * /*ds*/)
{
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Storage::NumElementsInStorage()
{
    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DataSettings *Storage::DataSettingsFromEnd(int /*fromEnd*/)
{
    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Storage::GetAverageData(Chan /*ch*/)
{
    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::GetDataFromEnd(int /*fromEnd*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Storage::GetLimitation(Chan /*ch*/, uint8 * /*data*/, int /*direction*/)
{
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::Push(DataSettings *ds)
{
    DataBuffer::Push(ds);
}
