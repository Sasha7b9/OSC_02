#include "Buffer.h"
#include <cstdlib>
#include <cstring>



Buffer::Buffer(uint s)
{
    Malloc(s);
}


Buffer::~Buffer()
{
    Free();
}


void Buffer::Realloc(uint _size)
{
    Free();
    Malloc(_size);
}


void Buffer::Free()
{
    std::free(data);
    data = nullptr;
    size = 0U;
}


void Buffer::Malloc(uint s)
{
    if (s > 0)
    {
        data = static_cast<uint8 *>(std::malloc(s));
        size = (data) ? s : 0U;

        if (data == nullptr)
        {
            LOG_ERROR("��� ������");
        }
    }
    else
    {
        data = nullptr;
        size = 0U;
    }
}
