#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"


struct TableMeasures
{
    /// ���������� ���������� �������������� ���������
    static void Draw();

    static int NumCols();

    static int NumRows();
    /// �� ������� ������� ����� �� �����������
    static int GetDeltaGridLeft();
    /// ���������� ������ ���� ��� ������ ��������������� ���������
    static int DY();
    /// ���������� ������ ���� ��� ������ ��������������� ���������
    static int DX();


    struct Cell
    {
    public:
        Cell(int _row, int _col) : row(_row), col(_col) {};
        void Draw(int x, int y);

    private:
        void DrawStringMeasure(int x, int y);

        int row;
        int col;
    };
};


/// �������� ������ ���������
struct PageChoiceMeasures
{
    /// ��������� ������� ��� �������� ��������
    static void OnKeyEvent(const KeyEvent &event);
    /// ����� �������� ��� ��������/�������� �������� ������
    void OnOpenCloseEvent();
    /// ��������/��������� �����������
    static void ChangeState();

    static void Draw();
};


struct AutoMeasurements
{
    /// \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
    /// �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
    static void SetData();
    /// ���������� true, ���� ������ �����������
    static bool DataIsSetting();
    /// ��������� ��� �������������� ���������
    static void CalculateMeasures();
    /// ���������� �������� ���������
    static Measure GetActiveMeasure();

    static char *Freq2String(float, bool, char buffer[20]);

    static char *Float2String(float, bool, char buffer[20]);

    static int markerTime[Chan::Count][2];

    static int markerVoltage[Chan::Count][2];
    /// ������� ��������� ��������� (�� ������� ������)
    static int8 posActive;
};
