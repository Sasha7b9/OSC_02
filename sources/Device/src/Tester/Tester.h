#pragma once
#include "common/Command.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Menu/Pages/Include/PageDisplay.h"


class Tester
{
public:
    static const int NUM_STEPS = 5;
    
    // ������������� ���������� �����
    static void Init();

    void DeInit();

    static void Enable();

    static void Disable();

    static void StartStop();

    static void Update();

    static void ProcessStep();
    
    // ��������� ���������� �� Settings
    static void LoadPolarity();
    
    // ������������� ��� ��������� ���������� � ����������� � ����������� Settings
    static void LoadStep();

    static int16 DeltaRShiftA();

    // �������
    struct Scale
    {
        enum E
        {
            _2uA,
            _5uA,
            _10uA,
            _20uA,
            _50uA,
            _100uA,
            _200uA,
            _500uA,
            _1mA,
            _2mA,
            _5mA,
            _10mA,
            _20mA,
            Count
        } value;
        Chan::E ch;
        explicit Scale(Range::E range, Chan::E _ch) : value(static_cast<E>(range)), ch(_ch) {};
        pString ToString() const;
    };

    // ��������
    struct Shift
    {
        Shift(int16 rShift, Chan::E _ch) : shift(rShift), ch(_ch) {};
        String ToString(Scale::E scale);
    private:
        int16 shift;
        Chan::E ch;
    };

    // ��� ����� ��������� � ������-���������� - ����������� ��� �����
    struct Control
    {
        enum E
        {
            Voltage,
            Current
        };
    };

    struct Polarity
    {
        enum E
        {
            Positive,
            Negative
        };
    };

    struct StepI
    {
        enum E
        {
            _4mA,
            _20mA
        };
    };

    struct StepU
    {
        enum E
        {
            _100mV,
            _500mV
        };
    };


    struct ViewMode
    {
        enum E
        {
            Lines,
            Points
        };
    };
};


struct DisplayTester
{
    static void Update();
    
    // ������������� ����� ��� ���������, ��������������� ���� numStep
    static void SetPoints(int numStep, const uint16 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS]);

private:
    
    // ���� true, �� ��� ����� ��� ������
    static bool ready[Tester::NUM_STEPS];
    
    // �������� ������� �����������
    static void DrawLegend(int x, int y);
    
    // ���������� ��������� ������ ������
    static void DrawParametersChannel(Chan::E ch, int x, int y);
    
    // ���������� ����, ������� ����� �������� ��������������� "���������"
    static Color ColorForStep(int step);
    
    // �������� ������ ��������� numStep
    static void DrawData(int step);
    
    // ���������� �������� �������� �������� ��������������� "���������"
    static String ValueForStep(int step);
};
