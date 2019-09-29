#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    class MultimeterWorker;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Multimeter
{
    class DisplayWorker;

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
    friend class DisplayWorker;
    friend class ::Display::MultimeterWorker;
    public:
        static void Update();
        /// ����� ��� ������� ��������� ��������� �� �������
        static void SetMeasure(const uint8 buffer[13]);

        static void ChangedMode();
    };
};
