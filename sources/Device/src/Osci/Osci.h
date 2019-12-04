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

    /// ��������� ��� �������� ����������, ����������� ��� ������ � ������ �������������
    struct StructReadRand
    {
        uint step;       ///< ��� ����� �������
        uint posFirst;   ///< ������� ������ ��������� �����
    };
    /// ���������� ������, ����������� ��� ������ ������ � ������� �������������.
    /// ���� Tsm == 0, �� ��������� ����� �������������� �� ��� ������ ������, � ��� ����������� ���������.
    /// � ���� ������
    static StructReadRand GetInfoForReadRand(int Tsm = NULL_TSHIFT, const uint8 *address = nullptr);

private:
    /// ���������� true, ���� ��� ����� ������ ������
    static bool CanReadData();

    static void UpdateFPGA();

    static void BalanceChannel(Chan::E ch, Range::E range);
};

struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *dataNew, uint size);
    static void SettingChanged();
    static void Draw();
};
