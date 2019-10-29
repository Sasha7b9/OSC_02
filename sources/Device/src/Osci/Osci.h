#pragma once
#include "Settings/Settings.h"


struct Osci
{
    static void Init();

    static void Update();

    static void DeInit();

    static void Start();

    static void Stop(bool pause = false);

    static void SetModeWork(ModeWork::E mode);

    static bool IsRunning();
    /// ��� �������� � ������ ��������� ���������
    static void Restart();
    /// ���������� true, ���� ��������� � ���������� ������
    static bool InModeP2P();
    /// ���������� true, ���� �������� � ������ �������������
    static bool InModeRandomizer();
    /// ��� ������� ���� �������� ��� ��������� ����� ������. �� ��� ������ �������� ���������
    static void OnChangedPoints();

    static void ReadPointP2P();
    /// ������������� �����
    static void Balance(Chan::E ch);
    /// ��������� �������� ��������� �������������
    static void LoadHoldfOff();

    static int addShift;

    static const int Kr[TBase::Count];

    /// ��������� ��� �������� ����������, ����������� ��� ������ � ������ �������������
    struct StructReadRand
    {
        int step;       ///< ��� ����� �������
        int posFirst;   ///< ������� ������ ��������� �����
    };
    /// ���������� ������, ����������� ��� ������ ������ � ������� �������������.
    /// ���� Tsm == 0, �� ��������� ����� �������������� �� ��� ������ ������, � ��� ����������� ���������.
    /// � ���� ������
    static StructReadRand GetInfoForReadRand(int Tsm = NULL_TSHIFT, const uint8 *address = nullptr);
};

struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *dataNew, int size);
    static void SettingChanged();
    static void Draw();
};
