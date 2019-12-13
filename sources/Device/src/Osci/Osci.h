#pragma once
#include "Settings/Settings.h"


#ifdef OLD_VERSION
#define PORT_P2P HPort::_G
#define PIN_P2P  HPin::_1
#else
#define PORT_P2P HPort::_F
#define PIN_P2P  HPin::_11
#endif


struct Osci
{
    static void Init();

    static void Update();

    static void OnPressStart();

    static void DeInit();

    static void Start();

    static void Stop(bool pause = false);

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
};

struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *dataNew, uint size);
    static void SettingChanged();
    static void Draw();
};
