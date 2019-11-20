#pragma once


/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/


struct ErrorSCPI
{
    enum State
    {
        Success,
        UnknownCommand
    };

    ErrorSCPI(State s) : state(s) {};

    void SendMessage();
    
    State state;

    String additionalMessage;

private:
    const char *prolog = "ERROR !!! ";
};


typedef const char *(*FuncSCPI)(const char *, ErrorSCPI *);


/// ���������, �������������� ���� ������.
struct StructSCPI
{
    enum Type
    {
        Empty,
        Node,           /// ��������� �������� "�����" ������, ����� ���� ������ �� ������ ����� structs
        Leaf            /// ��������� �������� "������" ������, ����� ��������� ������� func
    };

    Type type;          /// ��� ���������

    const char  *key;   /// �������� ����� ���� (�������)

    void *param;        /// ���� ��������� ������� ���������������� � ����������� �� ���� ���������.
                        /// ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.
                        /// ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI
};


struct SCPI
{
    /// ������-��������� ������ �������
    static const char SEPARATOR = ':';

    static void AppendNewData(const char *buffer, uint length);

    static void Update();

    /// ���������� true, ���� ��������� ��������� �� ����������� ������������������
    static bool IsLineEnding(const char *bufer);
};
