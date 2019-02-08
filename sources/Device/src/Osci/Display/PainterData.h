#pragma once
#include "Data/Reader.h"


#define SHIFT_IN_MEMORY     Osci::Display::PainterData::FirstPointOnScreen()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{
    namespace Display
    {
        namespace PainterData
        {
            void DrawData();
            /// ������ ������ �����, ��������� ������ �����
            int FirstPointOnScreen();
            /// ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
            BitSet64 PointsOnDisplay();
            /// \brief ��������� ����� ������ � ��������� ����� � ����������� ������
            BitSet64 BytesOnDisplay();
            /// ��� ������� ���� �������� ��� ������������ TPos ��� ����������� �������� ������� ���������� ����� ������������ ������ ���� ������
            void ChangeTPos();
        };
    };
};
