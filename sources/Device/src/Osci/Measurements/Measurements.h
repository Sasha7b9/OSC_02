#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"



namespace Osci
{
    namespace Measurements
    {
        /// \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
        /// �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
        void SetData();
        /// ���������� true, ���� ������ �����������
        bool DataIsSetting();
        /// ��������� ��� �������������� ���������
        void CalculateMeasures();
        /// ���������� �������� ���������
        Measure GetActiveMeasure();

        char* Freq2String(float, bool, char buffer[20]);

        char* Float2String(float, bool, char buffer[20]);

        extern int markerTime[Chan::Size][2];

        extern int markerVoltage[Chan::Size][2];
        /// ������� ��������� ��������� (�� ������� ������)
        extern int8 posActive;

        
        /// �������� ������ ���������
        namespace PageChoice
        {
            /// ��������� ������� ��� �������� ��������
            void OnKeyEvent(const KeyEvent &event);
            /// ����� �������� ��� ��������/�������� �������� ������
            void OnOpenCloseEvent();
            /// ��������/��������� �����������
            void ChangeState();

            void Draw();
        };

        
        namespace Cursors
        {
            /// ���������� ������� � ���������� ��������� ���������
            void Draw();
            /// ���������� true,���� ����� �������� �������
            bool  NecessaryDraw();
            /// �������� ������� ������� ����������
            float PosU(Chan::E ch, int numCur);
            /// ���������� �������� ������� �������
            float PosT(Chan::E ch, int num);
            /// �������� ������ ������� ����������
            String Voltage(Chan::E source, int numCur);
            /// ������� ������ ������� �������
            String Time(Chan::E source, int numCur);
            /// ���������� ������ ���������� ��������� �� ����������
            String PercentsU(Chan::E source);
            /// ���������� ������ ���������� ��������� �� �������
            String PercentsT(Chan::E source);

            void SetCursPosT_temp(Chan::E ch, int num, float value);
        };

        namespace Table
        {
            /// ���������� ���������� �������������� ���������
            void Draw();

            int NumCols();

            int NumRows();
            /// �� ������� ������� ����� �� �����������
            int GetDeltaGridLeft();
            /// ���������� ������ ���� ��� ������ ��������������� ���������
            int DY();
            /// ���������� ������ ���� ��� ������ ��������������� ���������
            int DX();


            class Cell
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
    };
};
