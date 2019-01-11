#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TYPE_GRID       (set.disp_typeGrid)
#define TYPE_GRID_1     (TYPE_GRID == Grid::Type::_1)
#define TYPE_GRID_2     (TYPE_GRID == Grid::Type::_2)
#define TYPE_GRID_3     (TYPE_GRID == Grid::Type::_3)
#define TYPE_GRID_4     (TYPE_GRID == Grid::Type::_4)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Grid
{
    /// ��������� ������������ ��� ������� ��������, ������� �������� ����������� ��� ����� �������� � ���������� ����� - ��������, ��������� ����
    static const int WIDTH = 280;
    static const int HEIGHT = 200;
    static const int SIZE_CELL = 20;

    /// ��� �������� ����� �������� � ����������� �� ������������� ������
    int Left();
    int Top();
    int Width();
    int Height();
    int Bottom();
    int Right();
    int ChannelCenterHeight();
    int FullBottom();
    int MathBottom();
    int MathHeight();
    int ChannelBottom();
    static int MathTop();

    /// ���������� �����
    void Draw();

    static float DeltaY();

    static float DeltaX();

    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY);

    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);

    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX);

    static int  DeltaVforLineGrid();
    /// ���������� ���������� ����� 
    static int  DeltaHforLineGrid();

    /// ��� ����� �� ������.
    struct Type
    {
        enum E
        {
            _1,
            _2,
            _3,
            _4,
            Size
        } value;
    };
};
