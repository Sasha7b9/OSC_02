#pragma once


struct DisplayMultimeter
{
    static void Update();
    /// ����� ��� ������� ��������� ��������� �� �������
    static void SetMeasure(const uint8 buffer[13]);

    static void ChangedMode();
};


struct Multimeter
{
    /// �������������
    static void Init();

    static void DeInit();

    static void Update();
    /// �������� �����������, ��� ��������� ��������� ������ � ����� ��������� ���������� ��������� ����� �������
    static void ChangeMode();

    static void Calibrate(int calibr);

    static void ChangeAVP();
};
