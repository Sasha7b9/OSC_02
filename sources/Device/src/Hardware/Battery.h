#pragma once


class Battery
{
public:

    static const float MIN_ABS;

    // �������������
    static void Init();

    static void Draw(int x, int y);

    // �������� �������� ���������� �������
    static float GetVoltageAKK();

private:   
    
    // �������� �������� ���������� ��������� ��������� ����������
    static float GetVoltagePOW();
    
    // ������� ���������� �������� ��� ��������� � ������
    static float PowerADC_ToVoltage(uint value);
    
    // ������� ���������� �������� ��� ������� � ������
    static float BatADC_ToVoltage(float value);
    
    // ���������� ������� ������������ ������
    static float CalculatePercents(float volts);
    
    // ���������� ����� ������� � ����������� ����
    static void DrawUGO(int x, int y, float procents);

    // ��������, ��������������� 100% � 0%, ��� ������� ���������� ������������� akk � ��������� ������ pow
    static float Voltage100();
    static float Voltage0();

    // ������������ ��������, ������� �������� ������� � ���
    static const float MAX_ADC_REL;
    
    // ����������, ��������������� MAX_ADC_REL
    static const float MAX_ADC_ABS;
};
