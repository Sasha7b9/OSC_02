#pragma once
#include "Utils/Stack.h"


class FreqMeter
{
public:
    
    // �������������.
    static void Init();
    
    // ������� ��������� ��� �������� �������
    static void LoadFreqSettings();
    
    // ������� �������� ��� �������� �������
    static void LoadPeriodSettings();

    static void Update();
    
    // ���������� ���������� �������� �������
    static float GetFreq();

    static BitSet32 freqActual;             // ����� �������� ��������� �������������� �������� �������. ��� ������ � ������ �����������. 0 ��������, ��� �������� �������� �� ����
    static BitSet32 periodActual;           // ����� �������� ��������� �������������� �������� �������. ��� ������ � ������ �����������. 0 ��������, ��� �������� �������� �� ����
    static uint     timeStartMeasureFreq;   // ����� ������ ��������� �������
    static uint     timeStartMeasurePeriod; // ����� ������ ��������� �������
    static uint     lastFreqRead;           // ��������� ����� ����������� ������ �������
    static uint     lastPeriodRead;         // ��������� ����� ���������� ������ �������
    static uint     lastFreqOver;           // ��������� ����� ������������ �������
    static uint     lastPeriodOver;         // ��������� ����� ����������� �������

    struct FPGA
    {
        static void ResetCounterFreq();
        static void ResetCounterPeriod();
    };

private:
    
    // ���������� ��������� �������� ��������� � ���������, ��������������� �������� �������
    static void SetStateLamps();

    static void SetStateLampFreq();

    static void SetStateLampPeriod();

    static void ReadFreq();

    static void ReadPeriod();

    static void LoadSettings();

    static float FreqSetToFreq(const BitSet32 *fr);

    static float PeriodSetToFreq(const BitSet32 *period);

public:

    /// ������� ����������� ��������� ��� ����� �������.
    struct FreqClc
    {
        enum E
        {
            _100kHz,
            _1MHz,
            _10MHz,
            _100MHz,
            Count
        };
    };

    /// ���������� ��������.
    struct NumberPeriods
    {
        enum E
        {
            _1,
            _10,
            _100,
            _1k,
            _10k,
            _100k,
            Count
        };
    };

    /// ��� ���������� - ������ ��� �������
    struct ModeView
    {
        enum E
        {
            Frequency,
            Period,
            Count
        };
    };

    /// ����� ����� �������.
    struct TimeCounting
    {
        enum E
        {
            _100ms,
            _1s,
            _10s,
            Count
        };
    };
};
