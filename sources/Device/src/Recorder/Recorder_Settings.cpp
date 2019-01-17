#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Recorder/Recorder_Settings.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"

#include "Recorder/Recorder.h"
#include "Hardware/FSMC.h"
#include "FPGA/FPGA.h"
#endif


#ifdef WIN32
#pragma warning(disable:4310)
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Recorder::Settings::ScaleX::Load()
{
    static const uint8 values[ScaleX::Size] =
    {
        BIN_U8(01010110),  // -V2501  // 5 ��   100ms  
        BIN_U8(01010111),  // -V2501  // 10 ��  200ms  
        BIN_U8(01011001),  // -V2501  // 25 ��  500ms  
        BIN_U8(01011010),  // -V2501  // 50 ��  1s     
        BIN_U8(01011011),  // -V2501  // 100 �� 2s     
        BIN_U8(01011101),  // -V2501  // 200 �� 5s     
        BIN_U8(01011110),  // -V2501  // 500 �� 10s
        BIN_U8(01011110),  // -V2501  // 1 �    10s
        BIN_U8(01011110),  // -V2501  // 2 �    10s
        BIN_U8(01011110),  // -V2501  // 5 �    10s
        BIN_U8(01011110),  // -V2501  // 10 �   10s
        BIN_U8(01011110),  // -V2501  // 20 �   10s
        BIN_U8(01011110),  // -V2501  // 30 �   10s
        BIN_U8(01011110)   // -V2501  // 60 �   10s
    };

    FSMC::WriteToFPGA8(WR_TBASE, values[SET_TBASE]);

    if (Recorder::IsRunning())
    {
        Stop();
        Start();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Settings::ScaleX::Change(int delta)
{
    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&set.rec_scaleX), (uint8)(ScaleX::Size - 1));
    }
    else
    {
        ::Math::LimitationDecrease<uint8>((uint8 *)(&set.rec_scaleX), 0);
    }

    Load();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Recorder::Settings::ScaleX &Recorder::Settings::ScaleX::Current()
{
    return set.rec_scaleX;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Recorder::Settings::ScaleX::ToString() const
{
    static const struct StructScaleX
    {
        const char *name[Language::Size];
        StructScaleX(pString nRU, pString nEN)
        {
            name[Language::RU] = nRU;
            name[Language::EN] = nEN;
        };
    }
    scales[ScaleX::Size] =
    {
        StructScaleX("5\x10��",   "5\x10ms"),
        StructScaleX("10\x10��",  "10\x10ms"),
        StructScaleX("25\x10��",  "25\x10ms"),
        StructScaleX("50\x10��",  "50\x10ms"),
        StructScaleX("100\x10��", "100\x10ms"),
        StructScaleX("250\x10��", "250\x10ms"),
        StructScaleX("500\x10��", "500\x10ms"),
        StructScaleX("1\x10�",    "1\x10s"),
        StructScaleX("2\x10�",    "2\x10s"),
        StructScaleX("5\x10�",    "5\x10s"),
        StructScaleX("10\x10�",   "10\x10s"),
        StructScaleX("20\x10�",   "20\x10s"),
        StructScaleX("30\x10�",   "30\x10s"),
        StructScaleX("60\x10�",   "60\x10s"),
    };

    return scales[value].name[LANG];
}
