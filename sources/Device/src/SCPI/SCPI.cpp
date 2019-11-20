#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


const char *ErrorSCPI::prolog = "!!! ERROR !!! ";


/// ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
/// ����� ���������� nullptr.
static const char *BeginWith(const char *buffer, const char *word);
/// ����������� ������� ��������� ������� �������� StructSCPI.
/// � ������ ��������� ���������� ���������� ����� �������, �������������� �� ��������� ������������ ��������.
/// � ������ ����������� ���������� - ���������� nullptr. ��� ������ ��������� � *error
static const char *Process(const char *buffer, const StructSCPI structs[], ErrorSCPI *error); //-V2504
/// ��������� ���� ������ node
static const char *ProcessNode(const char *begin, const StructSCPI *node, ErrorSCPI *error);
/// ��������� ����� node
static const char *ProcessLeaf(const char *begin, const StructSCPI *node, ErrorSCPI *error);
/// ������� ��������� ������� �� begin �� data
static void RemoveFromBegin(const char *begin);
/// ������� ��� ������� �� �����������
static void RemoveSymbolsBeforeSeparator();


static String data;


void SCPI::AppendNewData(const char *buffer, uint)
{
    data.Append(buffer);

    SU::ToUpper(data.c_str());
}


void SCPI::Update()
{
    RemoveSymbolsBeforeSeparator();

    if(data.Size() == 0)
    {
        return;
    }

    ErrorSCPI error;

    const char *end = Process(data.c_str(), head, &error);

    error.SendMessage();

    if(end)
    {
        RemoveFromBegin(end);
    }
}


static const char *Process(const char *buffer, const StructSCPI strct[], ErrorSCPI *error) //-V2504
{
    while (strct->type != StructSCPI::Empty)
    {
        const char *end = BeginWith(buffer, strct->key);

        if (end)
        {
            if (strct->type == StructSCPI::Node)
            {
                return ProcessNode(end, strct, error);
            }
            else if (strct->type == StructSCPI::Leaf)
            {
                return ProcessLeaf(end, strct, error);
            }
            else
            {
                LOG_WRITE("���� �� ������� �� ����� �� �����");
            }
        }

        strct++;
    }

    return nullptr;
}


static const char *BeginWith(const char *buffer, const char *word)
{
    while (*word)
    {
        if (*buffer == '\0')
        {
            return nullptr;
        }

        if (*word == *buffer)
        {
            ++word;
            ++buffer;
        }
        else
        {
            break;
        }
    }

    return (*word == '\0') ? buffer : nullptr;
}


static const char *ProcessNode(const char *begin, const StructSCPI *node, ErrorSCPI *error)
{
    const StructSCPI *params = reinterpret_cast<const StructSCPI *>(node->param);

    return Process(begin, params, error);
}


static const char *ProcessLeaf(const char *begin, const StructSCPI *node, ErrorSCPI *error)
{
    FuncSCPI func = reinterpret_cast<FuncSCPI>(node->param);

    return func(begin, error);
}


bool SCPI::IsLineEnding(const char *buffer)
{
    return (*buffer == 0x0D);
}


void ErrorSCPI::SendMessage()
{
    if(state == Success)
    {
        return;
    }

    static const char *names[] =
    {
        "",
        "Invalid sequence"
    };

    if (state == InvalidSequence)
    {
        String invalidSequence;

        const char *p = startInvalidSequence;

        while(p < endInvalidSequence)
        {
            invalidSequence.Append(*p++);
        }

        String message("%s %s : %s", prolog, names[state], invalidSequence.c_str());

        SCPI::SendAnswer(message.c_str());
    }
}


static void RemoveFromBegin(const char *begin)
{
    data.RemoveFromBegin(static_cast<uint>(begin - data.c_str()));
}


static void RemoveSymbolsBeforeSeparator()
{
    if (data.Size() && data[0] != SCPI::SEPARATOR && data[0] != '*')
    {
        ErrorSCPI error(ErrorSCPI::InvalidSequence);

        error.startInvalidSequence = data.c_str();
        error.endInvalidSequence = data.c_str();

        for(uint i = 0; i < data.Size() && data[i] != SCPI::SEPARATOR && data[i] != '*'; i++)
        {
            error.endInvalidSequence++;
        }

        error.SendMessage();

        data.RemoveFromBegin(static_cast<uint>(error.endInvalidSequence - error.startInvalidSequence));
    }
}


void SCPI::SendAnswer(char *message)
{
    if(message[std::strlen(message) - 1] != 0x0D)
    {
        String msg(message);
        msg.Append(0x0D);
        VCP::SendStringAsynch(msg.c_str());
    }
    else
    {
        VCP::SendStringAsynch(message);
    }
}
