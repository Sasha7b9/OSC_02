#pragma once


namespace FPGA
{
    namespace Settings
    {
        void Load();

        void LoadTrigSource();
        /// ���������� � �������������� ��������� ������, ���������� �� �������� � ���� �������������
        void LoadTrigSourceInput();

        void ChangeRange(Chan::E ch, int delta);

        void LoadRanges();

        void RShiftChange(Chan::E ch, int delta);

        void LoadRShift(Chan::E ch);

        void TrigLevChange(int delta);

        void LoadTrigLev();

        void TBaseChange(int delta);

        void LoadTBase();

        void SetTShift(int tShift);

        void LoadTShift();

        void TShiftChange(int delta);
        /// ��������/��������� ����������.
        void LoadCalibratorMode();
        /// ���������� �������� ��������� �������������
        void LoadHoldfOff();
    };
}
