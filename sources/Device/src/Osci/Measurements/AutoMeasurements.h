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

struct CursorsOsci
{
    /// ���������� ������� � ���������� ��������� ���������
    static void Draw();
    /// ���������� true,���� ����� �������� �������
    static bool  NecessaryDraw();
    /// �������� ������� ������� ����������
    static float PosU(Chan::E ch, int numCur);
    /// ���������� �������� ������� �������
    static float PosT(Chan::E ch, int num);
    /// �������� ������ ������� ����������
    static String Voltage(Chan::E source, int numCur);
    /// ������� ������ ������� �������
    static String Time(Chan::E source, int numCur);
    /// ���������� ������ ���������� ��������� �� ����������
    static String PercentsU(Chan::E source);
    /// ���������� ������ ���������� ��������� �� �������
    static String PercentsT(Chan::E source);

    static void SetCursPosT_temp(Chan::E ch, int num, float value);
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


struct MeasurementsOsci
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

    static int markerTime[Chan::Size][2];

    static int markerVoltage[Chan::Size][2];
    /// ������� ��������� ��������� (�� ������� ������)
    static int8 posActive;
};
