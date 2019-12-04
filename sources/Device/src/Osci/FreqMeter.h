#pragma once
#include "Menu/Pages/Include/PageFreqMeter.h"
#include "Utils/Stack.h"


class FreqMeter
{
    friend struct DisplayFreqMeter;
public:
    /// �������������.
    static void Init();
    /// ������� ��������� ��� �������� �������
    static void LoadFreqSettings();
    /// ������� �������� ��� �������� �������
    static void LoadPeriodSettings();

    static void Update();
    /// ���������� ���������� �������� �������
    static float GetFreq();

private:
    /// ���������� ��������� �������� ��������� � ���������, ��������������� �������� �������
    static void SetStateLamps();

    static void SetStateLampFreq();

    static void SetStateLampPeriod();

    static void ReadFreq();

    static void ReadPeriod();

    static void LoadSettings();

    static float FreqSetToFreq(const BitSet32 *fr);

    static float PeriodSetToFreq(const BitSet32 *period);   
    /// ������������� � true �������� ��������, ��� ������� ����� ������� �� �������� �������
    static bool readPeriod;

    static float prevFreq;

    static float frequency;

    static float period;
    /// ����� �������� ��������� �������������� �������� �������. ��� ������ � ������ �����������. 0 ��������, ��� �������� �������� �� ����
    static BitSet32 freqActual;
    /// ����� �������� ��������� �������������� �������� �������. ��� ������ � ������ �����������. 0 ��������, ��� �������� �������� �� ����
    static BitSet32 periodActual;
    /// ��������� ����� ����������� ������ �������
    static uint lastFreqRead;
    /// ��������� ����� ���������� ������ �������
    static uint lastPeriodRead;
    /// ��������� ����� ������������ �������
    static uint lastFreqOver;
    /// ��������� ����� ����������� �������
    static uint lastPeriodOver;
    /// ���� true - ����� �������� ����� �������
    static bool lampFreq;
    /// ���� false - ����� �������� ����� �������
    static bool lampPeriod;
};


struct DisplayFreqMeter
{
    /// ��������� ��������
    static void Draw();

private:

    static void DrawFrequency(int x, int _y);

    static void DrawPeriod(int x, int y);
    /// ������� ���������� ����������
    static void DrawDebugInfo();

    static pString FreqSetToString(const BitSet32 *fr);

    static pString PeriodSetToString(const BitSet32 *pr);
    /// ���������� ������� �������� ������� ���������� �������� �������� ������� ��� ������ ����������
    static int LowOrder(FreqMeterFreqClc::E freqCLC, FreqMeterNumberPeriods::E numPeriods);
    /// ����������� 6 �������� �����, ���������� � �����, � ��������� ������ �������. ������� �������� ������ �������� �� ������� �����. order - ��� �������
    static pString StackToString(Stack<uint> *stack, int order);
    /// ���������� 6 �������� �� ����� stack � ����� buffer. ������� ������ �� ������� �����. ����� ������� �� point �������, ������� � buffer[0]
    static void WriteStackToBuffer(Stack<uint> *stack, int point, const char *suffix);
};
