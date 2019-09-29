#pragma once
#include "Settings/SettingsOsci.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Measure
{
public:
    // ���� ���������
    struct Type
    {
        enum E
        {
            None,
            VoltageMax,
            VoltageMin,
            VoltagePic,
            VoltageMaxSteady,
            VoltageMinSteady,
            VoltageAmpl,
            VoltageAverage,
            VoltageRMS,
            VoltageVybrosPlus,
            VoltageVybrosMinus,
            Period,
            Freq,
            TimeNarastaniya,
            TimeSpada,
            DurationPlus,
            DurationMinus,
            SkvaznostPlus,
            SkvaznostMinus,
            DelayPlus,
            DelayMinus,
            PhazaPlus,
            PhazaMinus,
            Number
        } value;
        explicit Type(E v) : value(v) {};
    };

    Measure(int r, int c) : row(r), col(c) {};

    Type::E GetType();

    String GetStringMeasure(Chan::E ch, char *buffer, int lenBuf);
    /// ������������� �������� ��������� ��� ���������� ���������
    static void ChangeActive(int delta);
    /// ���������� ������� ������� ����������, �������������� �������� ������� ������� posCurT
    static float CalculateCursorU(Chan::E ch, float posCurT);
    /// ���������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU
    static float CalculateCursorT(Chan::E ch, float posCurU, int numCur);

    static void SetMarkerVoltage(Chan::E ch, int num, float value);

    static void SetMarkerTime(Chan::E ch, int num, int value);

    static char GetChar(Type::E measure);

    static void ShortPressOnSmallButonMarker();
    /// ���������� true, ���� ��������� �������� - ������� ������
    bool IsActive();

    String Name();

    static String GetName(Type::E type);

private:

    /// ������� ��������
    static void SetActive(int row, int col);
    /// ������ � �������, � ������� ��������� ������ ���������
    int row;
    /// ������� � �������, � ������� ��������� ������ ���������
    int col;
};
