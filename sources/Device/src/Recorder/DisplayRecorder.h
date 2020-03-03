#pragma once


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

private:
    
    // ���������� ������������� ���������
    static void DrawSettings(int x, int y);
    
    // ���������� ������
    static void DrawData(Record *record);

    // ���������� ������ ������
    static void DrawChannel(Record *record, Chan::E ch);

    // ���������� ������ �������
    static void DrawSensor(Record *record);


    static void DrawMemoryWindow();

    // ���������� �������� Y ������ ��� value �����
    static int Y(int value);


    static void DrawCursors();


    static void DrawParametersCursors();


    static char *TimeCursor(int numCur, char buffer[20]);


    static char *VoltageCursor(Chan::E, int, char[20]);


    static char *DeltaTime(char buffer[20]);

    // true, ���� � ������ ������ ���������� ���������
    static bool inProcessUpdate;


    static uint16 posCursor[2];

    // � ���� ����� ���������� �����
    static int startPoint;
};
