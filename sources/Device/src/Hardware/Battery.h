#pragma once


class Battery
{
public:
    /// �������������
    static void Init();

    static void Draw(int x, int y);
    /// �������� �������� ���������� �������
    static float GetVoltageAKK(uint *adc);
    /// �������� �������� ���������� ��������� ��������� ����������
    static float GetVoltagePOW(uint *adc);
private:
    /// ������� ���������� �������� ��� ��������� � ������
    static float PowerADC_ToVoltage(float value);
    /// ������� ���������� �������� ��� ������� � ������
    static float BatADC_ToVoltage(float value);
    /// ���������� ������� ������������ ������
    static float CalculatePercents(float volts);
    /// ���������� ����� ������� � ����������� ����
    static void DrawUGO(int x, int y, float procents);

    /// ������������ ��������, ������� �������� ������� � ���
    static const float MAX_ADC_REL;
    /// ����������, ��������������� MAX_ADC_REL
    static const float MAX_ADC_ABS;

    static const float VOLTAGE_100_PERCENTS;

    static const float VOLTAGE_0_PERCENTS;
};
