#pragma once


struct DisplayRecorder
{
    static void Update();
    /// �������� ���� ��������� �����
    static void MoveLeft();
    /// �������� ���� ��������� ������
    static void MoveRight();
    /// ����������� ������� ������ �����
    static void MoveCursorLeft();
    /// ����������� ������� ������ ������
    static void MoveCursorRight();
};
