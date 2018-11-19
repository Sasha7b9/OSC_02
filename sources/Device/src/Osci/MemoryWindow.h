#pragma once


class  MemoryWindow
{
public:

    static void Draw();

    static int X();

    static int Y();

    static int Width();

    static int Height();

private:

    static void DrawDataInRect(int x, int y, int width, int height, uint8 *data, uint length);
    /// ���������� ������� ������
    static void DrawScreenArea();
};
