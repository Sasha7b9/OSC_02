#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// �������� ������ ���������
        class PageChoice
        {
        public:
            /// ��������� ������� ��� �������� ��������
            static void OnKeyEvent(const KeyEvent &event);
            /// ����� �������� ��� ��������/�������� �������� ������
            static void OnOpenCloseEvent();
            /// ��������/��������� �����������
            static void ChangeState();

            static void Draw();
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Cursors
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
    };
};
