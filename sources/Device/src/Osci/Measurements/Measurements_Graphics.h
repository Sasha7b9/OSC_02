#pragma once


namespace Osci
{
    namespace Measurements
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        namespace Graphics
        {
            /// ���������� ���������� �������������� ���������
            void Draw();

            int NumCols();

            int NumRows();
            /// �� ������� ������� ����� �� �����������
            int GetDeltaGridLeft();
            /// ���������� ������ ���� ��� ������ ��������������� ���������
            int DY();
            /// ���������� ������ ���� ��� ������ ��������������� ���������
            int DX();
        };
    }
}
