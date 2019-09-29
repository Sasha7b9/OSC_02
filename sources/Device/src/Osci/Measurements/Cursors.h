#pragma once
#include "Utils/String.h"
#include "Osci/Osci_Settings.h"


#define CURsT_CNTRL_CH(ch)          (set.curs.cntrlT[ch])
#define CURsT_CNTRL                 (CURsT_CNTRL_CH(set.curs.source))
#define CURsT_CNTRL_1               (CURsT_CNTRL == CursorsControl::_1)
#define CURsT_CNTRL_2               (CURsT_CNTRL == CursorsControl::_2)
#define CURsT_CNTRL_1_2             (CURsT_CNTRL == CursorsControl::_1_2)
#define CURsT_DISABLED              (CURsT_CNTRL == CursorsControl::Disable)
#define CURsT_ENABLED               (!CURsT_DISABLED)


namespace Osci
{
    namespace Measurements
    {
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
