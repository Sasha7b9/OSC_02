#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (*pFuncTester)();


class Message;


class FSMC
{
public:
    static void Init();

    static void WriteToPanel(Message *message);

    static void WriteToPanel(const uint8 *data, uint length);         /// \todo ��� ������� ������ ���� ���������
    /// �������� � ������ �������, ��������� �� ���� ����
    static void WriteToPanel2bytes(uint8 byte0, uint8 byte1);

    static void WriteToFPGA8(uint8 *address, uint8 value);

    static void WriteToFPGA16(uint8 *address, uint16 value);

    static uint8 ReadFromFPGA(const uint8 *address);
    /// ���������� true, ���� ��� ����� � �������
    static bool InterchangeWithPanel();
    /// ���������� �������, ������� ����� ��������� ����������� ����� ���������� ������ � �������
    static void RunFunctionAfterInteractionWitchPanel(pFuncTester func);
    /// ������ ���� ����� ��
    static uint8 ReadByteNow();
};
