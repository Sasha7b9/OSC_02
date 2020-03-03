#pragma once


class DisplayRecorder
{
public:

    static void Update();
    /// �������� ���� ��������� �����
    static void MoveLeft();
    /// �������� ���� ��������� ������
    static void MoveRight();
    /// ����������� ������� ������ �����
    static void MoveCursorLeft();
    /// ����������� ������� ������ ������
    static void MoveCursorRight();

private:
    /// ���������� ������������� ���������
    static void DrawSettings(int x, int y);
    /// ���������� ������
    static void DrawData();

    static void DrawMemoryWindow();
};
