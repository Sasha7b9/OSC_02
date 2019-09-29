#pragma once


namespace Osci
{
    namespace Measurements
    {
        namespace Cursors
        {
            /// ������������ ����������� ��������.
            struct Movement
            {
                enum E
                {
                    Pixels,    ///< �� �������� ������
                    Percents   ///< �� ���������
                } value;
                explicit Movement(E v) : value(v) {};
            };
        }
    }
}
