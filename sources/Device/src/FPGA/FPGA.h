#pragma once
#include "Settings/Settings.h"
#include "FPGA/TypesFPGA.h"
#include "FPGA/MathFPGA.h"


struct FlagFPGA
{
    static uint16 flag;

    FlagFPGA() { flag = 0; }

    static bool DataReady();
    static bool TrigReady();
    static bool Pred();
    static bool FreqReady();
    static bool HoldOff();
    static bool PeriodReady();
    static bool FreqOverflow();
    static bool PeriodOverflow();
    static bool FreqInProcess();
    static bool PeriodInProcess();
};


struct FPGA
{
    friend struct ContextFreqMeter;
    friend struct ContextOsci;
    friend struct ContextRecorder;
    friend struct ContextTester;

    friend struct Bandwidth;
    friend struct Device;
    friend struct Range;
    friend struct RShift;
    friend struct TBase;
    friend struct TrigInput;
    friend struct TrigPolarity;
    friend struct TShift;
    friend struct TrigLevel;
    friend struct Osci;

    static bool IsRunning();
    /// ��������/��������� ����������.
    static void LoadCalibratorMode();

    static void SetValueADC(uint16 value);

    static bool InStateWait() { return (fpgaStateWork == StateWorkFPGA_Wait); }

    static bool InStateStop() { return (fpgaStateWork == StateWorkFPGA_Stop); }

private:

    static void Init();

    static void OnPressStart();
    /// �������������� ������ �������������
    static void GiveStart();

    static void Reset();

    static void ReadData();

    static uint16 ReadLastRecord(Chan::E ch);

    static bool ReadDataChanenl(Chan::E ch, uint8 *data, uint numBytes);

    static void ClearDataRand();

    static void LoadRegUPR();

    static void ReadFlag();

    static bool ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data);

    static int CalculateShift();

    static uint16 post;
    static uint16 pred;
    /// ����� ������ ������
    static uint timeStart;
    /// ��������, ��������� �� handleADC
    static uint16 valueADC;

    static bool isRunning;

    static StateWorkFPGA fpgaStateWork;

    static FlagFPGA flag;
    /// ����� �������� �����, ������� � �������� ����� ������ ������ �� �������. ���� addrRead == 0xffff, �� ����� ������� ����� �������
    static uint16 addrRead;
};

struct GPIO
{
    static void Init();
    static uint16 GetPin(FPin::E pin);
    static void WritePin(FPin::E pin, int enable);
    static void SetPin(FPin::E pin);
    static void ResetPin(FPin::E pin);
    static void WriteRegisters(FPin::E cs, uint16 value);
};
