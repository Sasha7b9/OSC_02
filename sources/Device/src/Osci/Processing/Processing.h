#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Processing
{
    friend class Measure;

public:
    /// ���������������� ��������� ��������� ������ ������������� �������� sinX/X
    static void InterpolationSinX_X(uint8 *data, int numPoints, Osci::Settings::TBase::E tBase);
    /// ���������� ������ ��������������� ���������
    static String GetStringMeasure(Measure::Type::E measure, Chan::E ch, char *buffer, int lenBuf);
    /// ��������� ��� �������������� ���������
    static void CalculateMeasures();

    static int markerTime[Chan::Size][2];

    static int markerVoltage[Chan::Size][2];

private:
    /// ���������� ������� � ������������� � ������� ����������
    static void CountedToCurrentSettings();
    /// ���������� ������� � ������� Range � rShift
    static void CountedRange(Chan::E ch);
    /// ���������� ������� � �������� TBase
    static void CountedTBase();
    /// ���������� ������� � �������� tShift
    static void CountedTShift();
    /// ���������� ���������� ����� � ������� � ������������� ����������
    static void CountedEnumPoints();
};
