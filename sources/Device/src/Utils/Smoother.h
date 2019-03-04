#pragma once
#include "Display/Display_Types.h"
#include "Settings/Settings.h"
#include <cstring>


/// ����������� ����� ��� ������������� �������
class Smoother
{
public:
    /// �������� ������ in � �������� �� � out
    static void Run(const uint8 *in, uint8 *out, uint numBytes)
    {
        size = (int)numBytes;
        data = in;

        if (ENUM_SMOOTHING.ToNumber() < 2)
        {
            std::memcpy(out, in, numBytes);
        }
        else
        {
            for (uint i = 0; i < numBytes; i++)
            {
                out[i] = CalculatePoint((int)i);
            }
        }
    }
private:
    /// ������������ ���� ���������� �����
    static uint8 CalculatePoint(int index)
    {
        uint sum = 0U;          // ����� ����� ��������� �����

        uint parts = 0U;        // ����� ���������� ��� ���������������� �����

        index -= ENUM_SMOOTHING.ToNumber() / 2;

        do
        {
            if (index >= 0 && index < size)
            {
                sum += data[index++];
            }
        } while (++parts < ENUM_SMOOTHING.ToNumber());

        return (uint8)(sum / parts);
    }

    static int size;
    const static uint8 *data;
};
