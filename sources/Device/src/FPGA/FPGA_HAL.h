#pragma once

namespace Address
{
    namespace WR
    {
        extern uint8 *START;        ///< �����
        extern uint8 *TBASE;        ///< ��������
        extern uint8 *PRED_LO;      ///< ����������
        extern uint8 *PRED_HI;
        extern uint8 *POST_LO;      ///< �����������
        extern uint8 *POST_HI;

        extern uint8 *UPR;          ///< ����������
#define BIT_UPR_RAND            0       ///< ���. ������������
#define BIT_UPR_PEAK            1       ///< ������� ��������
#define BIT_UPR_CALIBR_AC_DC    2       ///< ���������� AC/DC
#define BIT_UPR_CALIBR_ZERO     3       ///< ���������� 0/4�

        extern uint8 *FREQMETER;    ///< ���������� ������������
        extern uint8 *TRIG;         ///< ���������� ��������������/��������
#define BIT_TRIG_POLARITY       0       ///< ���������� ����������� �������������
#define BIT_TRIG_ENABLED        1       ///< ��� ��������� ������������. 0 - ���, 1 - ����

        extern uint8 *START_ADDR;               ///< \brief ����� ������. ����� ����������� �� ����� ������, ����� ������ ������ � 
                                                ///  ������������� ������ (������� �� ���������� � ����������)
        extern uint8 *RESET_COUNTER_FREQ;       ///< ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������
        extern uint8 *RESET_COUNTER_PERIOD;     ///< ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������

        extern uint8 *TRIG_HOLD_ENABLE;         ///< ���������� ��������� �������������. "0" - ����, "1" - ���
        extern uint8 *TRIG_HOLD_VALUE_LOW;      ///< ������� ���� �������� �������������
        extern uint8 *TRIG_HOLD_VALUE_MID;      ///<    �������� �������������� �� ������� N = 0 - (�������� / 100��). ������������ �������� - 1.5 ���
        extern uint8 *TRIG_HOLD_VALUE_HI;       ///< ������� ���� �������� �������������
    }

    namespace RD
    {
        extern uint8 *DATA_A;               ///< ������ ������� ������
        extern uint8 *DATA_A_PEAK_MAX;      ///< ������ �������� ��������� ������� ������
        extern uint8 *DATA_A_PEAK_MIN;
        extern uint8 *DATA_B;               ///< ������ ������� ������
        extern uint8 *DATA_B_PEAK_MAX;      ///< ������ ������� ��������� ������� ������
        extern uint8 *DATA_B_PEAK_MIN;
        extern uint8 *LAST_RECORD_LO;       ///< ����� ��������� ������
        extern uint8 *LAST_RECORD_HI;
        extern uint8 *FREQ_BYTE_0;          ///< ���������� �������� �������
        extern uint8 *FREQ_BYTE_1;
        extern uint8 *FREQ_BYTE_2;
        extern uint8 *FREQ_BYTE_3;
        extern uint8 *PERIOD_BYTE_0;        ///< ���������� �������� �������
        extern uint8 *PERIOD_BYTE_1;
        extern uint8 *PERIOD_BYTE_2;
        extern uint8 *PERIOD_BYTE_3;
        extern uint8 *FLAG_LO;              ///< ������� ���� �����
        extern uint8 *FLAG_HI;              ///< ������� ���� �����
    }
};


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
