#pragma once
#include "Osci/Osci_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    extern int addShift;

    extern const int Kr[Settings::TBase::Size];

    /// ��������� ��� �������� ����������, ����������� ��� ������ � ������ �������������
    struct StructReadRand
    {
        int step;       ///< ��� ����� �������
        int posFirst;   ///< ������� ������ ��������� �����
    };
    /// ���������� ������, ����������� ��� ������ ������ � ������� �������������.
    /// ���� Tsm == 0, �� ��������� ����� �������������� �� ��� ������ ������, � ��� ����������� ���������.
    /// � ���� ������
    StructReadRand GetInfoForReadRand(int Tsm = NULL_TSHIFT);
};
