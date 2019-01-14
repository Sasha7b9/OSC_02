#pragma once


namespace FPGA
{
    namespace SET
    {
        void Load();

        void RangeChange(Chan::E ch, int delta);

        void RangesLoad();

        void RShiftChange(Chan::E ch, int delta);

        void SetRShift(Chan::E ch, uint16 rShift);

        void LoadRShift(Chan::E ch);

        void LoadTrigSource();
        /// ���������� � �������������� ��������� ������, ���������� �� �������� � ���� �������������
        void LoadTrigSourceInput();

        void TrigLevChange(int delta);

        void LoadTrigLev();

        void LoadTrigPolarity();

        void TBaseChange(int delta);

        void LoadTBase();

        void SetTShift(int tShift);

        void LoadTShift();

        void TShiftChange(int delta);

        void SetModeCouple(Chan::E ch, ModeCouple::E couple);
        /// ��������/��������� ����������.
        void LoadCalibratorMode();
        /// ���������� �������� ��������� �������������
        void LoadHoldfOff();

        struct Range
        {
            enum E
            {
                _2mV,
                _5mV,
                _10mV,
                _20mV,
                _50mV,
                _100mV,
                _200mV,
                _500mV,
                _1V,
                _2V,
                _5V,
                _10V,
                _20V,
                Number
            } value;
            explicit Range(E v) : value(v) {};
            pString Name() const;
            pString ToString(Divider::E divider);
        };
    };
}
