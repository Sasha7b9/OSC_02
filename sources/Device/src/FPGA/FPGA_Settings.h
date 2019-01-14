#pragma once


namespace FPGA
{
    namespace Settings
    {
        void Load();

        void LoadTrigSource();
        /// ���������� � �������������� ��������� ������, ���������� �� �������� � ���� �������������
        void LoadTrigSourceInput();

        void LoadRanges();

        void LoadRShift(Chan::E ch);

        void LoadTrigLev();

        void LoadTBase();

        void LoadTShift();
        /// ��������/��������� ����������.
        void LoadCalibratorMode();
        /// ���������� �������� ��������� �������������
        void LoadHoldfOff();

        uint8 ValueForRange(Chan::E ch);
    };
}
