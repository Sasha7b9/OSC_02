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
        InvalidSequence
    };

    ErrorSCPI(State s = Success) : state(s), startInvalidSequence(nullptr), endInvalidSequence(nullptr) {};

    void Set(State s, const char *start, const char *end)
    {
        state = s;
        startInvalidSequence = start;
        endInvalidSequence = end;
    };

    void SendMessage();
    
    State state;
    /// � ������ ����������� ������������ ������������������ ���� ��������� ��������� �� ������, ��������� �� ���������
    const char *startInvalidSequence;
    const char *endInvalidSequence;

private:
    static const char *prolog;
};


typedef const char *(*FuncSCPI)(const char *);


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

    static const int SIZE_SEPARATOR = 1;

    static void AppendNewData(const char *buffer, uint length);

    static void Update();

    /// ���������� true, ���� ��������� ��������� �� ����������� ������������������
    static bool IsLineEnding(const char **bufer);
    /// ������� �����
    static void SendAnswer(char *message);
    /// ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
    /// ����� ���������� nullptr.
    static const char *BeginWith(const char *buffer, const char *word);
};
