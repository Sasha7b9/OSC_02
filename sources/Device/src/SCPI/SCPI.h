#pragma once


/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/


typedef const char *(*FuncSCPI)(const char *);


/// ���������, �������������� ���� ������.
struct StructSCPI
{
    const char *key;            /// �������� ����� ���� (�������)

    const StructSCPI *strct;    /// ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.

    FuncSCPI  func;             /// ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };   /// ��������� �������� "�����" ������, ����� ���� ������ �� ������ ����� structs
    bool IsLeaf() const { return func != nullptr; };    /// ��������� �������� "������" ������, ����� ��������� ������� func
};


#define DEF_NODE(key, strct) {key, strct, nullptr}
#define DEF_LEAF(key, func) {key, nullptr, func}
#define DEF_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::IsLineEnding(&t)) { SCPI::SendBadSymbols();
#define SCPI_EPILOG(t)  return t; } return nullptr;


namespace SCPI
{
    /// ������-��������� ������ �������
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(const char *buffer, uint length);

    void Update();
    /// ���������� true, ���� ��������� ��������� �� ����������� ������������������
    bool IsLineEnding(const char **bufer);
    /// ������� �����
    void SendAnswer(char *message);
    /// ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
    /// ����� ���������� nullptr.
    const char *BeginWith(const char *buffer, const char *word);
    /// ������� ��������� �� ��������� ��������, ���� ������� �������
    void SendBadSymbols();
};
