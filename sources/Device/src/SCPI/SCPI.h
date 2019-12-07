#pragma once


/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/


typedef const char *(*FuncSCPI)(const char *);
typedef bool (*FuncTestSCPI)();
typedef void (*FuncHint)(uint);


/// ���������, �������������� ���� ������.
struct StructSCPI
{
    const char *key;            /// �������� ����� ���� (�������)

    const StructSCPI *strct;    /// ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.

    FuncSCPI  func;             /// ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI

    FuncTestSCPI test;

    const char *hint;

    FuncHint funcHint;

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };   /// ��������� �������� "�����" ������, ����� ���� ������ �� ������ ����� structs
    bool IsLeaf() const { return func != nullptr; };    /// ��������� �������� "������" ������, ����� ��������� ������� func
};


#define SCPI_NODE(key, strct)                      {key, strct,   nullptr, nullptr, nullptr}
#define SCPI_LEAF(key, func, test, hint, funcHint) {key, nullptr, func,    test,    hint,   funcHint}
#define SCPI_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::IsLineEnding(&t)) { SCPI::SendBadSymbols();
#define SCPI_EPILOG(t)  return t; } return nullptr;

#define SCPI_RUN_IF_END(func) if(end) { SCPI_PROLOG(end) func; SCPI_EPILOG(end) }

#define SCPI_REQUEST(func)                          \
    const char *end = SCPI::BeginWith(buffer, "?"); \
    SCPI_RUN_IF_END(func)

#define SCPI_PROCESS_ARRAY(names, func)             \
    for(int i = 0; i < names[i][0] != 0; i++)       \
    {                                               \
        end = SCPI::BeginWith(buffer, names[i]);    \
        SCPI_RUN_IF_END(func)                       \
    }                                               \
    return nullptr;

#define SCPI_EXIT_ERROR()   LOG_WRITE("������ ����� SCPI %s:%d", __FILE__, __LINE__); return false;

#define SCPI_APPEND_STRING(string) SCPI::AppendNewData(string.c_str(), std::strlen(string.c_str())); SCPI::Update()

#define FUNC_HINT(size, names)                  \
    String message;                             \
    for(uint i = 0; i < size; i++)              \
    {                                           \
        message.Append(' ');                    \
    }                                           \
    for(int i = 0; i < names[i][0] != 0; i++)   \
    {                                           \
        message.Append(names[i]);               \
        message.Append(" |");                   \
    }                                           \
    SCPI::SendAnswer(message.c_str());

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
    void SendAnswer(const char *message);
    /// ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
    /// ����� ���������� nullptr.
    const char *BeginWith(const char *buffer, const char *word);
    /// ������� ��������� �� ��������� ��������, ���� ������� �������
    void SendBadSymbols();

    bool Test();
};
