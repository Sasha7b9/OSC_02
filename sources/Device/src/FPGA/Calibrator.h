#pragma once
#include "Settings/SettingsOsci.h"


struct Calibrator
{
    /// ��������� ��������� ����������
    static void Calibrate();
    /// ������������� ����� �� ���� ����������
    static bool Balance(Chan::E ch, bool showHint = true);

private:
    /// ������������� ����� Ch
    static bool Calibrate(Chan::E ch);
    /// ������������� ����� �� ����� ���������
    static void Balance(Chan::E ch, Range::E range);
    /// "���������" �����
    static bool Stretch(Chan::E ch);
    /// ����� ����������� �������� ������
    static float FindStretchK(Chan::E ch);

    static void NormalExit();

    static void BadExit();

public:

    struct Mode
    {
        enum E
        {
            Freq,
            DC,
            GND
        };
        Mode() {}
        static Mode::E &Ref();
        operator Mode::E() { return Ref(); }
    };
};
