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

struct Flag
{
    enum E
    {
        _DATA_READY = 0,  ///< ������ ������ ��� ���������� (��������� �������� ������������)
        _TRIG_READY = 1,  ///< ���� ��������������
        _PRED = 2,  ///< ���� 1, �� ���������� ��������, ����� ������ �������������� ������ (���������
        _P2P = 3,  ///< 
        _FREQ_READY = 4,  ///< ���� ���������� ��������� �������
        _PERIOD_READY = 5,  ///< ���� ���������� ��������� �������
        _HOLD_OFF_FLAG = 7,  ///< �������������� ������ ����� ������ ������ ����� ���� ���� � �������
        _FREQ_OVERFLOW = 8,  ///< ������� ������������ �������� �������
        _PERIOD_OVERFLOW = 9,  ///< ������� ������������ �������� �������
        _FREQ_IN_PROCESS = 10, ///< ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
        _PERIOD_IN_PROCESS = 11  ///< ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
    } value;
};

struct FPin
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
    explicit FPin(E v) : value(v) {};
};

struct VALUE
{
    static const uint8 AVE = 127;
    static const uint8 MIN = AVE - 125;
    static const uint8 MAX = AVE + 125;
    /// ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������
    static const uint8 NONE = 0;
};
