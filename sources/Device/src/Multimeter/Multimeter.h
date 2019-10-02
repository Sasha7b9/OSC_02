#pragma once

namespace Multimeter
{
    /// �������������
    void Init();

    void DeInit();

    void Update();
    /// �������� �����������, ��� ��������� ��������� ������ � ����� ��������� ���������� ��������� ����� �������
    void ChangeMode();

    void Calibrate(int calibr);

    void ChangeAVP();

    /// ������������ ��� ���������
    class Display
    {
    public:
        static void Update();
        /// ����� ��� ������� ��������� ��������� �� �������
        static void SetMeasure(const uint8 buffer[13]);

        static void ChangedMode();
    };
};
