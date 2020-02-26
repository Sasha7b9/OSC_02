#pragma once
#include "Settings/Settings.h"


#define OSCI_IN_MODE_P2P        (set.time.base >= TBase::MIN_P2P)
#define OSCI_IN_MODE_RANDOMIZER (set.time.base <= TBase::_50ns)


struct Osci
{
    friend struct Randomizer;
    friend struct ContextTester;


    static void Init();


    static void Update();


    static void OnPressStart();


    static void DeInit();

    // (���� button == true) - ��� ������ �������
    static void Start(bool button);


    static void Stop();


    static bool IsRunning();
    
    // ��� �������� � ������ ��������� ���������
    static void Restart();
    
    // ��� ������� ���� �������� ��� ��������� ����� ������. �� ��� ������ �������� ���������
    static void OnChangedPoints();
   
    // ��������� �������� ��������� �������������
    static void LoadHoldfOff();
    
    // ��� ������� ����� �������� ��� ��������� ������ �������
    static void ChangedTrigStartMode();
    
    // ��� ������� ����� �������� ��� ��������� TBase
    static void ChangedTBase();
    
    // ������� ������ ������������� ��� ������������ �������
    static void ClearDataRand();

    // ��������, ��������� �� handleADC
    static uint16 valueADC;

    
    static int addShift;

    
    static void ReadData();

    // ��������� ��� �������� ����������, ����������� ��� ������ � ������ �������������
    struct StructReadRand
    {
        uint step;       ///< ��� ����� �������
        uint posFirst;   ///< ������� ������ ��������� �����
    };
    
    // ���������� ������, ����������� ��� ������ ������ � ������� �������������.
    // ���� Tsm == 0, �� ��������� ����� �������������� �� ��� ������ ������, � ��� ����������� ���������.
    // � ���� ������
    static StructReadRand GetInfoForReadRand(int Tsm = NULL_TSHIFT, const uint8 *address = nullptr);

    // ���������� �������� ������
    struct InputController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };

private:
    // ���������� true, ���� ��� ����� ������ ������
    static bool CanReadData();

    static void UpdateFPGA();

    // ������� ������
    static void (*funcStart)(bool);
    static void StartNormal(bool button);
    static void StartP2P(bool button);
    static void StartWaitP2P(bool button);
    static void StartSingleP2P(bool button);

    // ������� �����
    static void (*funcStop)();
    static void StopNormal();
    static void StopWaitP2P();
    static void StopSingleP2P();

    static void SetFunctionsStartStop();

    // � ����������� �� ��������� ����� ���������� ������ ������ ������ � ���������� ���� ������������� ���������� ������� ����� ����������
    static bool ProcessFlagReady();

    // ���������� ���� �����������
    static void ProcessFlagPred();

    // ������ ������ ������ � ������� data
    static bool ReadDataChannel(Chan::E ch, uint8 *data);


    static uint16 ReadLastRecord(Chan::E ch);


    static bool ReadDataChannelRand(uint8 *address, uint8 *data);


    static int CalculateShift();

    // ����� �������� �����, ������� � �������� ����� ������ ������ �� �������. ���� addrRead == 0xffff, �� ����� ������� ����� �������
    static uint16 addrRead;
};


struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *newData, uint size);
    static void SettingChanged();
};


// ��������������� ��������� ��� ������� �������������
struct Randomizer
{
    // ��� ������� ����� �������� �������� �� ����� ���������� ��������� ��� ������ ����� �������������, ���� �� ��������� � ������ �������������
    static void Read();
    // ���������� ��������� ������ �� ���������� ��������� � �����, ��������� ds
    static void MoveReadedData(DataSettings *ds);

private:
    static void MoveReadedDataChannel(DataSettings *ds, Chan::E ch);
};


class Roller
{
public:
    // ������� ����� �������� ����� ������ �������
    static void Prepare();

    // ��� ������� ����� ��������� �������� �� ����� ���������� ��������� ��� ������ ����� ����������� ������, ���� �� ��������� � ���������� ������
    static void ReadPoint();

private:

    // �������� ��� ��������� ��������� �����
    static void AddPoint(BitSet16 dataA, BitSet16 dataB);

    // ��������� �� ��������� ������� ������
    static DataSettings *ds;

    // ������� �����, ������� ����� �������� ���������
    static int currentPoint;
};
