#pragma once
#include "Utils/String.h"
#include "Osci/Osci_Settings.h"


/// �������� �������� �� ��������� �������
#define CURS_LOOK_T(ch)             (set.curs.lookMode[ch] == CursorsLookMode::Time)
/// �������� �������� �� ��������� ������� � ����������
#define CURS_LOOK_BOTH(ch)          (set.curs.lookMode[ch] == CursorsLookMode::Both)

#define CURsU_CNTRL_CH(ch)          (set.curs.cntrlU[ch])
#define CURsU_CNTRL                 (CURsU_CNTRL_CH(set.curs.source))
#define CURsU_CNTRL_1               (CURsU_CNTRL == CursorsControl::_1)
#define CURsU_CNTRL_2               (CURsU_CNTRL == CursorsControl::_2)
#define CURsU_CNTRL_1_2             (CURsU_CNTRL == CursorsControl::_1_2)
#define CURsU_DISABLED              (CURsU_CNTRL == CursorsControl::Disable)
#define CURsU_ENABLED               (!CURsU_DISABLED)

#define CURsT_CNTRL_CH(ch)          (set.curs.cntrlT[ch])
#define CURsT_CNTRL                 (CURsT_CNTRL_CH(set.curs.source))
#define CURsT_CNTRL_1               (CURsT_CNTRL == CursorsControl::_1)
#define CURsT_CNTRL_2               (CURsT_CNTRL == CursorsControl::_2)
#define CURsT_CNTRL_1_2             (CURsT_CNTRL == CursorsControl::_1_2)
#define CURsT_DISABLED              (CURsT_CNTRL == CursorsControl::Disable)
#define CURsT_ENABLED               (!CURsT_DISABLED)

/// ���������� �������� ����������, ��������������� 100%
#define dUperc(ch)                  (set.curs.deltaU100percents[ch])
/// ���������� �������� �������, �������������� 100%
#define dTperc(ch)                  (set.curs.deltaT100percents[ch])


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
