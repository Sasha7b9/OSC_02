#pragma once


#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void ErrorHandler(const char *, int);
};


/// ������������ ��� ���������� �������� �������
struct HAL_DAC2
{
    static void Init();
    /// �������� value �� 0 �� 100
    static void SetValue(uint value);
};


struct HAL_FSMC
{
    static void Init();

    static void SendToPanel(uint8 *data, uint size);

    static bool Receive();
};
