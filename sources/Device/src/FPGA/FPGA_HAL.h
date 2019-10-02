#pragma once


namespace FPGA
{
    namespace HAL
    {
        void LoadRegUPR();

        void ReadFlag();

        extern uint16 flag;

        
        struct Flag
        {
            enum E
            {
                _DATA_READY        = 0,  ///< ������ ������ ��� ���������� (��������� �������� ������������)
                _TRIG_READY        = 1,  ///< ���� ��������������
                _PRED              = 2,  ///< ���� 1, �� ���������� ��������, ����� ������ �������������� ������ (���������
                _P2P               = 3,  ///< 
                _FREQ_READY        = 4,  ///< ���� ���������� ��������� �������
                _PERIOD_READY      = 5,  ///< ���� ���������� ��������� �������
                _HOLD_OFF_FLAG     = 7,  ///< �������������� ������ ����� ������ ������ ����� ���� ���� � �������
                _FREQ_OVERFLOW     = 8,  ///< ������� ������������ �������� �������
                _PERIOD_OVERFLOW   = 9,  ///< ������� ������������ �������� �������
                _FREQ_IN_PROCESS   = 10, ///< ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
                _PERIOD_IN_PROCESS = 11  ///< ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
            } value;
        };

        struct GetFlag
        {
            static bool DATA_READY();
            static bool TRIG_READY();
            static bool PRED();
            static bool FREQ_READY();
            static bool HOLD_OFF();
            static bool PERIOD_READY();
            static bool FREQ_OVERFLOW();
            static bool PERIOD_OVERFLOW();
            static bool FREQ_IN_PROCESS();
            static bool PERIOD_IN_PROCESS();
        };


        
        namespace GPIO
        {
            void Init();

            struct Pin
            {
                enum E
                {
                    SPI3_SCK,
                    SPI3_DAT,
                    SPI3_CS1,
                    SPI3_CS2,
                    A1,
                    A2,
                    A3,
                    A4,
                    LF1,
                    LF2,
                    A1S,
                    A0S,
                    LFS,
                    Number
                } value;
                explicit Pin(E v) : value(v) {};
            };

            uint16 GetPin(Pin::E pin);

            void WritePin(Pin::E pin, int enable);

            void SetPin(Pin::E pin);

            void ResetPin(Pin::E pin);

            void WriteRegisters(Pin::E cs, uint16 value);
        }
    }
}
