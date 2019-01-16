#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "FPGA/FPGATypes.h"
#include "Hardware/FSMC.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "FPGA/FPGA.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Settings::LoadHoldfOff()
{
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_ENABLE, TRIG_HOLDOFF_ENABLED ? 1U : 0U);

    uint value = (uint)(0 - TRIG_HOLDOFF + 1);

    BitSet32 bs(value);

    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_LOW, bs.byte0);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_MID, bs.byte1);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_HI, bs.byte2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::TBase::Change(int delta)
{
    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&SET_TBASE), (uint8)(TBase::Size - 1));
    }
    else
    {
        if (SET_PEAKDET_EN &&                               // ���� ������ ����� �������� ���������
            SET_TBASE == TBase::MIN_PEAK_DET)               // � ���������� ������� �� �������, ��������������� ����������� � ������ �������� ��������� :
        {
            ::Display::ShowWarning(Warning::EnabledPeakDet);  // ������� ��������� �� ����
            return;                                         // � �������
        }

        ::Math::LimitationDecrease<uint8>((uint8 *)(&SET_TBASE), 0);
    }

    Load();
    FPGA::Start();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Osci::Settings::TBase::ToString() const
{
    /// ��������� ��� �������� ��������� �������� �� �������.
    static const struct StructTBase
    {
        const char *name[Language::Size];    // �������� ��������� � ��������� ����, ��������� ��� ������ �� �����.
        StructTBase(pString nRU, pString nEN)
        {
            name[Language::RU] = nRU;
            name[Language::EN] = nEN;
        };
    }
    tBases[TBase::Size] =
    {
        StructTBase("2\x10��",     "2\x10ns"),
        StructTBase("5\x10��",     "5\x10ns"),
        StructTBase("10\x10��",    "10\x10ns"),
        StructTBase("20\x10��",    "20\x10ns"),
        StructTBase("50\x10��",    "50\x10ns"),
        StructTBase("0.1\x10���",  "0.1\x10us"),
        StructTBase("0.2\x10���",  "0.2\x10us"),
        StructTBase("0.5\x10���",  "0.5\x10us"),
        StructTBase("1\x10���",    "1\x10us"),
        StructTBase("2\x10���",    "2\x10us"),
        StructTBase("5\x10���",    "5\x10us"),
        StructTBase("10\x10���",   "10\x10us"),
        StructTBase("20\x10���",   "20\x10us"),
        StructTBase("50\x10���",   "50\x10us"),
        StructTBase("0.1\x10��",   "0.1\x10ms"),
        StructTBase("0.2\x10��",   "0.2\x10ms"),
        StructTBase("0.5\x10��",   "0.5\x10ms"),
        StructTBase("1\x10��",     "1\x10ms"),
        StructTBase("2\x10��",     "2\x10ms"),
        StructTBase("5\x10��",     "5\x10ms"),
        StructTBase("10\x10��",    "10\x10ms"),
        StructTBase("20\x10��",    "20\x10ms"),
        StructTBase("50\x10��",    "50\x10ms"),
        StructTBase("0.1\x10�",    "0.1\x10s"),
        StructTBase("0.2\x10�",    "0.2\x10s"),
        StructTBase("0.5\x10�",    "0.5\x10s"),
        StructTBase("1\x10�",      "1\x10s"),
        StructTBase("2\x10�",      "2\x10s"),
        StructTBase("5\x10�",      "5\x10s"),
        StructTBase("10\x10�",     "10\x10s")
    };

    return tBases[value].name[LANG];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Settings::TPos::InBytes()
{
    static const int m[][2][3] =
    {
        {{0, 256, 510}, {0, 512, 1022}},
        {{0, 512, 1022}, {0, 1024, 2046}},
        {{0, 1024, 2046}, {0, 2048, 4094}},
        {{0, 2048, 4094}, {0, 4096, 8190}},
        {{0, 4096, 8190}, {0, 8192, 16382}},
        {{0, 8192, 16382}, {0, 8192, 16382}},
        {{0, 16384, 32766}, {0, 16384, 32766}}
    };
    return m[(int)FPGA_ENUM_POINTS][SET_PEAKDET][value];
}
