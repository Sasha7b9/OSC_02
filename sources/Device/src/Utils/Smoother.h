#pragma once
#include "Settings/Settings.h"
#include <cstring>


/// ����������� ����� ��� ������������� �������
class Smoother
{
public:
    /// �������� ������ in � �������� �� � out
    static void Run(const uint8 *in, uint8 *out, uint numBytes, uint _numSmooth)
    {
        numSmooth = _numSmooth;
        size = static_cast<int>(numBytes);
        data = in;

        if (numSmooth < 2)
        {
            std::memcpy(out, in, numBytes);
        }
        else
        {
            for (uint i = 0; i < numBytes; i++)
            {
                out[i] = CalculatePoint(static_cast<int>(i));
            }
        }
    }
private:
    /// ������������ ���� ���������� �����
    static uint8 CalculatePoint(int index)
    {
        uint sum = 0U;          // ����� ����� ��������� �����

        uint parts = 0U;        // ����� ���������� ��� ���������������� �����

        index -= numSmooth / 2;

        do
        {
            if (index >= 0 && index < size)
            {
                sum += data[index++];
            }
        } while (++parts < numSmooth);

        return static_cast<uint8>(sum / parts);
    }

    static int size;
    const static uint8 *data;
    static uint numSmooth;
};
