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

    static void AddNewData(const char *buffer, uint length);
    /// ���������� true, ���� ��������� ��������� �� ����������� ������������������
    static bool IsLineEnding(const char *bufer);
};
