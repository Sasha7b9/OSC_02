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
            Point(BitSet16 dataA, BitSet16 dataB)
            {
                data[Chan::A] = dataA;
                data[Chan::B] = dataB;
            }
            static Point CreateEmpty();
            bool IsEmpty();
            int Min(Chan::E ch);
            int Max(Chan::E ch);
        private:
            BitSet16 data[2];
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
            Point GetPoint(uint position, uint maxPoints);
            /// �������� ��������� �����
            Point NextPoint(uint maxPoints);
            /// ����� ������ ������ �����
            PackedTime timeStart;
            /// ���������� ������ ������, ����������� �������
            int Size();
        private:
            /// ��������� �� ����� ������ - ���������� ����� ������ ���������� �����
            Point *start;
            /// ���������� ���������� �����
            uint numPoints;
            /// ��������� �� ��������� ���������� ������
            uint pointer;
        };


        /// �������������. ����������� ��� ��������� ������ "�����������"
        void Init();

        void CreateNewFrame();

        Frame *CurrentFrame();
    };
}
