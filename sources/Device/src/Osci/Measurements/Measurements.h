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
        /// ���������������� ��������� ��������� ������ ������������� �������� sinX/X
        void InterpolationSinX_X(uint8 *data, int numPoints, Osci::Settings::TBase::E tBase);
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
            static void OnKeyEvent(KeyEvent event);
            /// ����� �������� ��� ��������/�������� �������� ������
            static void OnOpenCloseEvent();
            /// ��������/��������� �����������
            static void ChangeState();

            static void Draw();
        };
    };
};
