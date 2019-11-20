#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


/// ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
/// ����� ���������� nullptr.
static char *BeginWith(const char *buffer, const char *word);
/// ����������� ������� ��������� ������� �������� StructSCPI.
/// � ������ ��������� ���������� ���������� ����� �������, �������������� �� ��������� ������������ ��������.
/// � ������ ����������� ���������� - ���������� nullptr. ��� ������ ��������� � *error
static const char *Process(const char *buffer, const StructSCPI structs[], ErrorSCPI *error);
/// ��������� ���� ������ node
static const char *ProcessNode(const char *begin, const StructSCPI *node, ErrorSCPI *error);
/// ��������� ����� node
static const char *ProcessLeaf(const char *begin, const StructSCPI *node, ErrorSCPI *error);


void SCPI::AddNewData(const char *buffer, uint length)
{
    String string(buffer);

    SU::ToUpper(string.CString(), length);
    
    Buffer data(length + 1);
    std::memcpy(data.data, buffer, length);
    data.data[length - 1] = 0;
    
    if (SU::EqualsStrings(reinterpret_cast<char *>(data.data), const_cast<char *>("*IDN?")))
    {
        const char *answer = "MNIPI, S8-57, v.1.2";
        VCP::SendDataAsynch(reinterpret_cast<const uint8 *>(answer), std::strlen(answer) + 1);
    }

    ErrorSCPI error(SCPI_Success);

    Process(buffer, head, &error);
}


static const char *Process(const char *buffer, const StructSCPI *structs, ErrorSCPI *error)
{
    const StructSCPI *str = structs;

    while (str->type != StructSCPI::Empty)
    {
        char *end = BeginWith(buffer, str->key);

        if (end)
        {
            if (str->type == StructSCPI::Node)
            {
                return ProcessNode(end, str, error);
            }
            else if (str->type == StructSCPI::Leaf)
            {
                return ProcessLeaf(end, str, error);
            }
            else
            {
                LOG_WRITE("���� �� ������� �� ����� �� �����");
            }
        }

        str++;
    }

    return nullptr;
}


static char *BeginWith(const char *, const char *)
{
    return nullptr;
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
