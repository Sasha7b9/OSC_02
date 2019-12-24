#pragma once
#include "Settings/Settings.h"


struct Osci
{
    static void Init();

    static void Update();

    static void OnPressStart();

    static void DeInit();
    /// (���� button == true) - ��� ������ �������
    static void Start(bool button);

    static void Stop();

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
    /// ��������� �������� ��������� �������������
    static void LoadHoldfOff();
    /// ��� ������� ����� �������� ��� ��������� ������ �������
    static void ChangedTrigStartMode();
    /// ��� ������� ����� �������� ��� ��������� TBase
    static void ChangedTBase();

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

    /// ���������� �������� ������
    struct InputController
    {
        static void Init();
    };

private:
    /// ���������� true, ���� ��� ����� ������ ������
    static bool CanReadData();

    static void UpdateFPGA();

    /// ������� ������
    static void (*funcStart)(bool);
    static void StartNormal(bool button);
    static void StartP2P(bool button);
    static void StartWaitP2P(bool button);
    static void StartSingleP2P(bool button);

    /// ������� �����
    static void (*funcStop)();
    static void StopNormal();
    static void StopWaitP2P();
    static void StopSingleP2P();

    static void SetFunctionsStartStop();
    /// � ����������� �� ��������� ����� ���������� ������ ������ ������ � ���������� ���� ������������� ���������� ������� ����� ����������
    static bool ProcessFlagReady();
    /// ���������� ���� �����������
    static void ProcessFlagPred();
};

struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *dataNew, uint size);
    static void SettingChanged();
    static void Draw();
};
