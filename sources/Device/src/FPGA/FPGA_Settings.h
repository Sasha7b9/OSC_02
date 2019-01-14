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

        void LoadRShift(Chan::E ch);

        void LoadTrigLev();

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
