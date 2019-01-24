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

            /// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
            struct Active
            {
                enum E
                {
                    U,
                    T,
                    None
                } value;
                explicit Active(E v) : value(v) {};
            };

            /// ����� �������� ��������.
            struct LookMode
            {
                enum E
                {
                    None,      ///< ������� �� ������.
                    Voltage,   ///< ������� ������ �� ����������� �������������.
                    Time,      ///< ������� ������ �� �������� �������������.
                    Both       ///< ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
                } value;
                explicit LookMode(E v) : value(v) {};
            };

            /// ����� �������� ���������
            struct Control
            {
                enum E
                {
                    _1,        ///< ������
                    _2,        ///< ������
                    _1_2,      ///< ������
                    Disable    ///< �������
                } value;
                explicit Control(E v) : value(v) {};
            };
        }
    }
}
