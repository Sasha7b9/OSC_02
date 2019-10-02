#pragma once


namespace Osci
{
    namespace Display
    {
        void Update();

        void DrawCursorTrigLevel();

        void DrawScaleLine(int x, bool forTrigLev);
        /// ���������� ������� ����, ��� ������� ��������� � �����������
        void SetFlagRedraw();

        namespace HiPart
        {
            void Draw();
        };

        namespace BottomPart
        {
            void Draw(int x, int y);

            void WriteCursors();
        };

        class Accumulator
        {
        public:
            /// ��� ������� ����� �������� ����� ������ ��������� ��������
            static void NextFrame();
            /// ����� ����������
            static void Reset();
        };
    }
}
