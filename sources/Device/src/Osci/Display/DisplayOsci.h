#pragma once


#define SHIFT_IN_MEMORY     DisplayOsci::PainterData::FirstPointOnScreen()


struct DisplayOsci
{
    static void Update();

    static void DrawCursorTrigLevel();

    void DrawScaleLine(int x, bool forTrigLev);
    /// ���������� ������� ����, ��� ������� ��������� � �����������
    static void SetFlagRedraw();

    struct HiPart
    {
        static void Draw();
    };

    struct BottomPart
    {
        static void Draw(int x, int y);

        void WriteCursors();
    };

    struct Accumulator
    {
        /// ��� ������� ����� �������� ����� ������ ��������� ��������
        static void NextFrame();
        /// ����� ����������
        static void Reset();
    };

    struct PainterData
    {
        static void DrawData();
        /// ������ ������ �����, ��������� ������ �����
        static int FirstPointOnScreen();
        /// ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
        static BitSet64 PointsOnDisplay();
        /// \brief ��������� ����� ������ � ��������� ����� � ����������� ������
        static BitSet64 BytesOnDisplay();
        /// ��� ������� ���� �������� ��� ������������ TPos ��� ����������� �������� ������� ���������� ����� ������������ ������ ���� ������
        static void ChangeTPos();
    };

    struct MemoryWindow
    {
        static void Draw();

        static int X();

        static int Y();

        static int Width();

        static int Height();
    };
};
