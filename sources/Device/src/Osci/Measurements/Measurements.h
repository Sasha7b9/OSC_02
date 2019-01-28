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
        void SetData(bool needSmoothing);
        /// ��������� ��� �������������� ���������
        void CalculateMeasures();
        /// ���������� ������ ��������������� ���������
        String GetStringMeasure(Measurements::Measure::Type::E measure, Chan::E ch, char *buffer, int lenBuf);
        /// ���������������� ��������� ��������� ������ ������������� �������� sinX/X
        void InterpolationSinX_X(uint8 *data, int numPoints, Osci::Settings::TBase::E tBase);
        /// ���������� �������� ���������
        Measure GetActiveMeasure();

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

            static bool IsActive();
            /// ��������/��������� �����������
            static void ChangeState();
            /// ������� �������
            static int8 posCursor;
        };
    };
};
