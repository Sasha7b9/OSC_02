#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


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

    SU::ToUpper(data.CString());
}


void SCPI::Update()
{
    RemoveSymbolsBeforeSeparator();

    ErrorSCPI error(ErrorSCPI::Success);

    const char *end = Process(data.CString(), head, &error);

    if (end)
    {
        RemoveFromBegin(end);
    }
    else
    {
        error.SendMessage();
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
    if (state == Success)
    {
        return;
    }

    if (state == UnknownCommand)
    {
        VCP::SendStringAsynch("UNKNOWN COMMAND");
        VCP::SendStringAsynch(additionalMessage.CString());
    }
}


static void RemoveFromBegin(const char *begin)
{
    data.RemoveFromBegin(static_cast<uint>(begin - data.CString()));
}


static void RemoveSymbolsBeforeSeparator()
{
    if (data.Size() && data[0] != SCPI::SEPARATOR)
    {
        ErrorSCPI error(ErrorSCPI::UnknownCommand);

        while (data.Size() && data[0] != SCPI::SEPARATOR)
        {
            char symbols[2] = { data[0], '\0' };
            error.additionalMessage.Append(symbols);

            uint size = data.Size();
            size = size;

            data.RemoveFromBegin(1);
        }

        error.SendMessage();
    }
}
