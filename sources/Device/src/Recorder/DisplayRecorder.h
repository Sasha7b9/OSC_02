#pragma once
#include "Osci/ParametersOsci.h"


struct Record;


class DisplayRecorder
{
public:

    static void Update();

    // �������� ���� ��������� �����
    static void MoveWindowLeft();

    // �������� ���� ��������� ������
    static void MoveWindowRight();

    // ����������� ������� ������ �����
    static void MoveCursorLeft();

    // ����������� ������� ������ ������
    static void MoveCursorRight();

    // ���������� true, ���� ��� ��� ���������� ����������� ������
    static bool InProcessUpdate();

    // ���������� ������������ ������
    static void SetDisplayerRecord(Record *record);
};
