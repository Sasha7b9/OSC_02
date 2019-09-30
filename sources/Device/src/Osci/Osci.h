#pragma once
#include "Settings/SettingsOsci.h"



namespace Osci
{
    void Init();

    void Update();

    void DeInit();

    void Start();

    void Stop(bool pause = false);
    /// ��� �������� � ������ ��������� ���������
    void Restart();
    /// ���������� true, ���� ��������� � ���������� ������
    bool InModeP2P();
    /// ���������� true, ���� �������� � ������ �������������
    bool InModeRandomizer();
    /// ��� ������� ���� �������� ��� ��������� ����� ������. �� ��� ������ �������� ���������
    void OnChangedPoints();

    void ReadPointP2P();
    /// ������������� �����
    void Balance(Chan::E ch);
    /// ��������� �������� ��������� �������������
    void LoadHoldfOff();

    extern int addShift;

    extern const int Kr[TBase::Size];

    /// ��������� ��� �������� ����������, ����������� ��� ������ � ������ �������������
    struct StructReadRand
    {
        int step;       ///< ��� ����� �������
        int posFirst;   ///< ������� ������ ��������� �����
    };
    /// ���������� ������, ����������� ��� ������ ������ � ������� �������������.
    /// ���� Tsm == 0, �� ��������� ����� �������������� �� ��� ������ ������, � ��� ����������� ���������.
    /// � ���� ������
    StructReadRand GetInfoForReadRand(int Tsm = NULL_TSHIFT, const uint8 *address = nullptr);
};
