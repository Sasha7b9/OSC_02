#pragma once


class Battery
{
public:
    // �������������
    static void Init();

    static void Draw(int x, int y);

private:
    
    // �������� �������� ���������� �������
    static float GetVoltageAKK();
    
    // �������� �������� ���������� ��������� ��������� ����������
    static float GetVoltagePOW();
    
    // ������� ���������� �������� ��� ��������� � ������
    static float PowerADC_ToVoltage(uint value);
    
    // ������� ���������� �������� ��� ������� � ������
    static float BatADC_ToVoltage(uint value);
    
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
