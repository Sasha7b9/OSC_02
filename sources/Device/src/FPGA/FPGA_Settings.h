#pragma once
#include "Osci/Osci_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define START_MODE              (set.trig_startMode)
#define START_MODE_IS_AUTO      (START_MODE == Trig::StartMode::Auto)
#define START_MODE_IS_WAIT      (START_MODE == Trig::StartMode::Wait)
#define START_MODE_IS_SINGLE    (START_MODE == Trig::StartMode::Single)

#define TRIG_POLARITY           (set.trig_polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig_input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig_TimeDelay)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    namespace Settings
    {
        /// ��������/��������� ����������.
        void LoadCalibratorMode();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// ����� ������ �� �����.
        struct ModeCouple
        {
            enum E
            {
                DC,      ///< �������� ����
                AC,      ///< �������� ����
                GND,     ///< ���� �������.
                Size
            } value;
            explicit ModeCouple(E v) : value(v) {};
            pString UGO() const;

            static void Set(Chan::E ch, ModeCouple::E couple);
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Bandwidth
        {
            enum E
            {
                Full,     ///< ���� ��� �������� ������� � ���� �����, �� ��� ���� ��������� ��������������� ������ �� �������-������-������.
                _20MHz,
            } value;

            explicit Bandwidth(E v) : value(v) {};
            void Load();

        private:
            /// ���������� �����, ��� �������� �������� ����������� ��� ���������
            Chan::E GetChannel() const;
        };
    }
}
