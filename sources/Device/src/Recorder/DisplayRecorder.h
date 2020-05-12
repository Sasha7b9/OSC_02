#pragma once
#include "Osci/ParametersOsci.h"


struct Record;


class DisplayRecorder
{
public:

    static void Update();

    // �������� ���� ��������� �����
    static void MoveLeft();

    // �������� ���� ��������� ������
    static void MoveRight();

    // ����������� ������� ������ �����
    static void MoveCursorLeft();

    // ����������� ������� ������ ������
    static void MoveCursorRight();

    // 
    static bool InProcessUpdate();
};
