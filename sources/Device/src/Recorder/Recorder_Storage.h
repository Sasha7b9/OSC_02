#pragma once
#include "Data/DataSettings.h"


namespace Recorder
{
    namespace Storage
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// ����� �� �������
        struct Point
        {
            Point(BitSet16 _data) : data(_data) {};
            static Point CreateEmpty();
            bool IsEmpty();
            int Min();
            int Max();
        private:
            BitSet16 data;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// ��������� ������ ������������ - ������� ������ �����
        struct Frame
        {
            Frame() : start(0), numPoints(0), pointer(0) {} //-V730

            void SetDataAddress(uint16 *address);
            /// ���������� �������� �����
            void AddPoint(BitSet16 dataA, BitSet16 dataB);
            /// ����� ����� � ��������������
            uint NumPoints();
            /// �������� ����� � ������� position
            Point GetPoint(uint position);
            /// �������� ��������� �����
            Point NextPoint();
            /// ����� ������ ������ �����
            PackedTime timeStart;
        private:
            /// ��������� �� ����� ������ - ���������� ����� ������ ���������� �����
            BitSet16 *start;
            /// ���������� ���������� �����
            uint numPoints;
            /// ��������� �� ��������� ���������� ������
            uint pointer;
        };


        /// �������������. ����������� ��� ��������� ������ "�����������"
        void Init();

        void CreateNewFrame();

        Frame &CurrentFrame();
    };
}
